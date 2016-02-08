#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Game.hpp"
#include "UserInterface.hpp"
#include "Unit.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"
#include "globals.hpp"

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int FRAMERATE = 20;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "I totally know how SDL works", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
		}
	}
	return success;
}

void cleanup_SDL()
{
	SDL_DestroyWindow( gWindow );
	SDL_Quit();
}

void draw_all(Game &g, UserInterface& ui){
	SDL_SetRenderDrawColor(gRenderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(gRenderer);

	ui.renderSelection( gRenderer );

	for (auto &u : g.unitsByID){
		u.second.draw( gRenderer );
	}

	ui.renderHUD( gRenderer );

	SDL_RenderPresent( gRenderer );
}

int main(){
	debugLog(" Testing...");

	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		Game g = Game();
		UserInterface userInterface = UserInterface(g);

		TeamID tID1 = g.createTeam();
		TeamID tID2 = g.createTeam();

		Team& t1 = g.getTeam(tID1);
		Team& t2 = g.getTeam(tID2);
		
		t1.loadUnitTemplate("../conf/units/marine");
		t2.loadUnitTemplate("../conf/units/mutalisk");
		//UnitTemplate& p1 = t1.unitTemplates.begin()->second;
		//UnitTemplate& p2 = t2.unitTemplates.begin()->second;

		UnitID a,b;
		for (int i=5; i<15; i++){
			for (int j=5; j<15; j++){
				//g.createUnit(tID2, (UnitTemplateID)3, Coordinate(50*i, 50*j));
				g.createUnit(tID1, (UnitTemplateID)3, Coordinate(25*i, 25*j));

			}
		}

		//Command cmd1(CMD_GOTOCOORD);
		//cmd1.targetID = b;

		//cmd1.queueSetting=QUEUE_OVERWRITE;

		//g.getUnit(a).handleCommand(cmd1);

		//Command cmd2(CMD_GOTOCOORD);
		//cmd2.targetCoord = Coordinate(300, 300);
		//cmd2.queueSetting=QUEUE_OVERWRITE;

		//g.getUnit(b).handleCommand(cmd2);


		for (int i=0; i<200; i++){
			if (userInterface.quit)
				break;

			SDL_Event event;

			while(SDL_PollEvent(&event)) {
				userInterface.handleInputEvent(event);
			}
			g.tick();
			draw_all(g, userInterface);
			SDL_Delay( 1000/FRAMERATE );
		}
	}

	//Free resources and close SDL
	cleanup_SDL();

	debugLog(" Done testing.");
	
	return 0;
}