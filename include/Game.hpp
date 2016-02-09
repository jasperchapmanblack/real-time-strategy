#pragma once

#include <vector>
#include <map>
#include <memory>

#include "typedefs.hpp"
#include "InhabitedGrid.hpp"
#include "Team.hpp"

class Unit;
class InhabitedGrid;

class Game
{
public:
	Game();
	TeamID createTeam();
	UnitID createUnit(TeamID team, UnitTemplateID unitTemplateID, Coordinate pos);
	void deleteUnit(UnitID);
	Unit& getUnit(UnitID i);
	Team& getTeam(TeamID i);

	void tick();

	bool teamsAreFriendly(TeamID, TeamID);
	bool unitsAreFriendly(UnitID a, UnitID b);

	InhabitedGrid inhabitedGrid;
	std::map<UnitID, Unit> unitsByID;
	std::map<TeamID, Team> teamsByID;

private:
	void resolveCollisions();
	UnitID smallestUnusedUnitID();
	TeamID smallestUnusedTeamID();

	TeamID smallestUnusedTeamID_;
	UnitID smallestUnusedUnitID_;
};
