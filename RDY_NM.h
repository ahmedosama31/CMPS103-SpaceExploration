#pragma once
#include "LinkedQueue.h"
#include "mission.h"

class RDY_NM : public LinkedQueue<mission*>
{
public:
	mission* AbortMission(int ID) {}
};