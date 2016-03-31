#include "StateAttackMove.hpp"
#include "StateAttack.hpp"
#include "Unit.hpp"
#include "InhabitedGrid.hpp"
#include "Game.hpp"
#include "Logging.hpp"

StateAttackMove::StateAttackMove(Coordinate destination):
	gotostate(StateGotoCoordinate(destination)),
	atkstate(StateAttack(0)),
	attacking(false)
	{}

StateExitCode StateAttackMove::update(Unit* unit) {
	// If we're already targeting someone, behave like we're attacking them
	// If we're in attacking range of an enemy, behave like we're attacking them
	// Otherwise, behave like we're walking
	// otherwise do nothing
	
	if (!this->attacking){ // if we're not targeting anyone
		this->updateTarget(unit);
	}

	if (this->attacking){
		//debugLog("attackstate");
		StateExitCode ret = this->atkstate.update(unit);
		if (ret == STATE_EXIT_COMPLETE){
			this->attacking = false;
			unit->animationState = ANIMSTATE_IDLE;
		}
		return STATE_EXIT_INCOMPLETE;
	}
	else {
		//debugLog("walkstate");
		unit->animationState = ANIMSTATE_WALKING;
		return this->gotostate.update(unit);
	}
}

bool StateAttackMove::updateTarget(Unit* unit) {
	auto potentialIDs = unit->game->inhabitedGrid.unitsInCircle(unit->xy, unit->getAttackRange());
	UnitID closestTargetID = 0;
	Distance closestTargetDistance = MAX_DISTANCE;
	for (auto &i : potentialIDs){
		Unit* potentialUnit = unit->game->getUnit(i);
		Distance r = pythagoreanDistance(unit->xy, potentialUnit->xy);
		if (unit->canAttack(potentialUnit) && r<closestTargetDistance ) {
			closestTargetID = i;
			closestTargetDistance = r;
		}
	}

	if (closestTargetID) {
		this->atkstate.targetID = closestTargetID;
		this->attacking = true;
		return true;
	}
	else {
		this->attacking = false;
		return false;
	}
}

std::vector<Coordinate> StateAttackMove::getStateWaypoints() {
	if (attacking) {
		std::vector<Coordinate> ret = this->atkstate.getStateWaypoints();
		std::vector<Coordinate> finalwaypoint = this->gotostate.getStateWaypoints();
		ret.insert(ret.end(), finalwaypoint.begin(), finalwaypoint.end());
		return ret;
	}
	return this->gotostate.getStateWaypoints();
}
