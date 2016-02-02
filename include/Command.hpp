#pragma once

#include "typedefs.hpp"

#include <vector>

class Unit;

enum CommandType{
	CMD_IDLE, 
	CMD_GOTOCOORD,
	CMD_ATTACK,
	//CMD_AMOVE,
};

enum QueueSetting{
	QUEUE_OVERWRITE,
	QUEUE_FRONT,
	QUEUE_BACK
};

class Command
{
public:
	Command(CommandType cmdtype_);
	CommandType cmdtype;
	QueueSetting queueSetting;
	std::vector<UnitID> commanded;
	UnitID targetID;
	Coordinate targetCoord;
};