#include "Team.hpp"

#include "Game.hpp"
#include "Unit.hpp"
#include "UnitTemplate.hpp"
#include "enums.hpp"

Team::Team(Game& _game, TeamID tID):
game(_game),
teamID(tID)
{
	//for testing
	//unitTemplates.emplace(1, new UnitTemplate("testUnit", 100, 20, 20, GROUND_ONLY, WEAPONTEMPLATEVECTOR_GOHERE);
};


void Team::loadUnitTemplate( std::string filename ){
	unitTemplates.emplace( 3, UnitTemplate(std::ifstream(filename), (TeamColor)teamID) ); // this is so broken. fix later. 3 is not good
};