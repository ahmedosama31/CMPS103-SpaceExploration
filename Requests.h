#pragma once
#include "MarsStation.h"
class Requests
{
protected:
	int RDay;
	int missionID;
public:
	Requests(int rday, int id) : RDay(rday), missionID(id) {}
	virtual void Operate(MarsStation& marsStation) = 0;
	int getRday() const { return RDay; }
	int getMissionID() const { return missionID; }
	void setMday(int Rday) { this->RDay = Rday; }	
	void setMissionID(int missionID) { this->missionID = missionID; }
};

