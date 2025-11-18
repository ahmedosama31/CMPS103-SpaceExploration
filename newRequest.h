#pragma once
#include "Requests.h"
#include "MissionType.h"
class newRequest :public Requests
{
private:
	MissionType MType;
	int TLOC;
	int MDUR;
public:
	newRequest(int RDay, int ID, MissionType MType, int TLOC, int MDUR)
		: Requests(RDay, ID),MType(MType), TLOC(TLOC), MDUR(MDUR) {}
	MissionType getMissionType() const { return MType; }
	int getTargetLocation() const { return TLOC; }
	int getMissionDuration() const { return MDUR; }
	void setMissionType(MissionType MType) { this->MType = MType; }
	void setTargetLocation(int TLOC) { this->TLOC = TLOC; }
	void setMissionDuration(int MDUR) { this->MDUR = MDUR; }


};

