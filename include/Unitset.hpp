#pragma once

#include <set>

#include "typedefs.hpp"

class Command;
class UnitTemplate;

class Unitset {
public:
	Unitset();
	Unitset(const Unitset&);

	void handleCommand(Command c);
	void startBuilding(UnitTemplateID unitTemplateID);

	// iterator stuff

	typedef std::set<UnitID>::iterator iterator;
	typedef std::set<UnitID>::const_iterator const_iterator;

	iterator begin() { return set.begin(); }

	const_iterator begin() const { return set.begin(); }

	iterator end() { return set.end(); }

	const_iterator end() const { return set.end(); }

	// UI stuff
	const UnitTemplate* getWHATTOCALLIT();
private:
	std::set<UnitID> set;
};