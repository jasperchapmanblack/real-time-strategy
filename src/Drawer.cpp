#include <SDL2/SDL.h>

#include "Drawer.hpp"
#include "Spritesheet.hpp"
#include "Logging.hpp"
#include "UnitTemplate.hpp"
#include "globals.hpp"
#include "Unit.hpp"
#include "UserInterface.hpp"
#include "ResourceManager.hpp"

SDL_Texture* hpBarEmpty = NULL;
SDL_Texture* hpBarFull = NULL;


int AIRBORNE_RENDER_HEIGHT = 40;

const int PIXEL_WIDTH = 100;
const int PIXEL_HEIGHT = 130;

int positiveRemainder(int a, int b) {
	// returns the unique integer 0<=r<b such that mb+r = a for some integer n
	return ((a%b)+b)%b;
}

Drawer::Drawer(std::shared_ptr<Spritesheet> sp):
	spritesheet(sp)
	{
	}

Drawer::Drawer(std::ifstream& is, TeamColor teamColor):
	idleCycleStart(0),
	idleCycleLength(1),
	walkCycleStart(0),
	walkCycleLength(0),
	attackCycleStart(0),
	attackCycleLength(0),
	deathCycleStart(0),
	deathCycleLength(0),
	deathCycleVertical(false),
	spritesheet(NULL),
	shadowsheet(NULL)
{
	std::string spritefilename, shadowfilename;
	bool hasShadowsheet=false, hasSpritesheet=false;
	std::string s;

	while (is>>s) {
		if (s=="#") {
			getline(is, s); // this is a comment. we ignore comments.
		}
		else if (s=="spritesheet") {
			is>>spritefilename;
			hasSpritesheet=true;
		}

		else if (s=="shadowsheet") {
			is>>shadowfilename;
			hasShadowsheet=true;
		}
		
		else if (s=="numFacingDirections") {
			is>>numFacingDirections;
		}

		else if (s=="idleCycleStart") {
			is>>idleCycleStart;
		}

		else if (s=="idleCycleLength") {
			is>>idleCycleLength;
		}

		else if (s=="walkCycleStart") {
			is>>walkCycleStart;
		}

		else if (s=="walkCycleLength") {
			is>>walkCycleLength;
		}

		else if (s=="attackCycleLength") {
			is>>attackCycleLength;
		}

		else if (s=="attackCycleStart") {
			is>>attackCycleStart;
		}

		else if (s=="deathCycleLength") {
			is>>deathCycleLength;
		}

		else if (s=="deathCycleStart") {
			is>>deathCycleStart;
		}

		else if (s=="deathCycleVertical") {
			is>>deathCycleVertical;
		}

		else if (s=="}") {
			if (hasSpritesheet) {
				spritesheet = gResourceManager->get(spritefilename, teamColor);
			}
			if (hasShadowsheet)
				shadowsheet = gResourceManager->get(shadowfilename, teamColor);
			return;
		}
		else{
			debugLog("Error: Drawer constructor ifstream contained unknown token:"+s);
		}
	}
}

Drawer::~Drawer() {
}

void Drawer::drawIdle(SDL_Renderer* renderer, Unit& unit, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (NULL != shadowsheet)
		shadowsheet->render(renderer,
			( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
			this->idleCycleStart + frame,
			drawPos.first,
			drawPos.second,
			ui->viewMagnification);
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
			idleCycleStart + frame,
			drawPos.first,
			drawPos.second + dy,
			ui->viewMagnification);
}

void Drawer::drawWalking(SDL_Renderer* renderer, Unit& unit, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (NULL != shadowsheet)
		shadowsheet->render(renderer,
			( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
			this->walkCycleStart + frame,
			drawPos.first,
			drawPos.second,
			ui->viewMagnification);
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
			this->walkCycleStart + frame,
			drawPos.first,
			drawPos.second + dy,
			ui->viewMagnification);
}

void Drawer::drawAttacking(SDL_Renderer* renderer, Unit& unit, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (NULL != shadowsheet)
		shadowsheet->render(renderer,
			( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
			this->attackCycleStart + frame,
			drawPos.first,
			drawPos.second,
			ui->viewMagnification);
	if (NULL != spritesheet)
		spritesheet->render(renderer,
			( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
			this->attackCycleStart + frame,
			drawPos.first,
			drawPos.second + dy,
			ui->viewMagnification);
}

void Drawer::drawDying(SDL_Renderer* renderer, Unit& unit, UserInterface* ui, Coordinate drawPos, int frame, int dy) {
	if (this->deathCycleVertical) {
		if (NULL != spritesheet)
			spritesheet->render(renderer,
				( (unit.drawFacingAngle+90+360)*2*this->numFacingDirections/360) % (2*this->numFacingDirections),
				this->deathCycleStart + frame,
				drawPos.first,
				drawPos.second + dy,
				ui->viewMagnification);
	} else {
		if (NULL != spritesheet)
			spritesheet->render(renderer,
				frame,
				this->deathCycleStart,
				drawPos.first,
				drawPos.second + dy,
				ui->viewMagnification);
	}
}

void Drawer::draw(SDL_Renderer* renderer, Unit& unit, UserInterface* ui /*, Coordinate cameraposition */) {
	// Draws the unit to the given surface.
	//spritesheet->render(renderer, 0, 0 , unit.xy.first, unit.xy.second);
	int dy = unit.dimension.air ? -AIRBORNE_RENDER_HEIGHT : 0;
	dy += (unit.animationState == ANIMSTATE_DYING && unit.dimension.air) ? 0.03*(unit.drawAnimationStep+2)*(unit.drawAnimationStep+2) : 0;
	dy *= ui->viewMagnification;
	Coordinate pos = ui->screenCoordinateFromObjective(unit.xy);

	switch (unit.animationState) {
		case ANIMSTATE_IDLE: {
			this->drawIdle(renderer, unit, ui, pos, positiveRemainder(unit.drawAnimationStep, this->idleCycleLength), dy);
			break;
		}
		case ANIMSTATE_WALKING: {
			this->drawWalking(renderer, unit, ui, pos, positiveRemainder(unit.drawAnimationStep, this->walkCycleLength), dy);
			break;
		}
		case ANIMSTATE_ATTACKING:
			if (unit.drawAnimationStep < 0)
				this->drawIdle(renderer, unit, ui, pos, positiveRemainder(unit.drawAnimationStep, this->idleCycleLength), dy);
			else
				this->drawAttacking(renderer, unit, ui, pos, std::max(0, positiveRemainder(unit.drawAnimationStep, this->attackCycleLength) ), dy);
			break;

		case ANIMSTATE_DYING:
			this->drawDying(renderer, unit, ui, pos, std::min(this->deathCycleLength -1, std::abs(unit.drawAnimationStep)), dy);
			break;
	}
	if (unit.animationState != ANIMSTATE_DYING) {
		draw_HP_bar(renderer, unit, pos.first, pos.second, ui->viewMagnification);
	}
}

void draw_HP_bar(SDL_Renderer* renderer, Unit& unit, const int renderX, const int renderY, const float magnification) {
	// Draws an HP bar centered at (renderX, renderY)
	const int granularity = 25;
	int dy = unit.dimension.air ? -AIRBORNE_RENDER_HEIGHT : 0;

	int HP = unit.hp;
	int maxHP = unit.getUnitTemplate().maxHP();

	if (hpBarEmpty==NULL)
		hpBarEmpty=loadTexture(renderer, "../resources/graphics/ui/tpbrempt.png");
	if (hpBarFull==NULL)
		hpBarFull=loadTexture(renderer, "../resources/graphics/ui/tpbrfull.png");

	SDL_Rect fullclip, emptyclip, tclip;

	int barlength = 5+3*maxHP/granularity;

	fullclip.w = 3 + 3*HP/granularity;
	fullclip.h = 10;
	fullclip.x = 0;
	fullclip.y = 0;
	
	emptyclip.w = barlength-fullclip.w;
	emptyclip.h = 10;
	emptyclip.x = 107-emptyclip.w;
	emptyclip.y = 0;

	tclip.w = magnification*(3 + 3*HP/granularity);
	tclip.h = magnification*10;
	tclip.x=renderX-(magnification*barlength)/2;
	tclip.y=renderY+ (dy - 25) * magnification;
	
	SDL_RenderCopy(renderer, hpBarFull, &fullclip, &tclip);
	tclip.x += magnification*fullclip.w;
	tclip.w = magnification*emptyclip.w;
	SDL_RenderCopy(renderer, hpBarEmpty, &emptyclip, &tclip);
}
