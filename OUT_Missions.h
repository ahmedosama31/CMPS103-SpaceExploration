#pragma once
#include "priQueue.h"
#include "mission.h"
class OUT_Missions : public priQueue<mission*>
{
public:
	mission* AbortMission(int ID) {}
};
