#pragma once

class Requests
{
protected:
	int Rday;
	int missionID;
public:
	Requests(int rday, int id) : Rday(rday), missionID(id) {}
	virtual void Operate() = 0;
	int getRday() const { return Rday; }
	int getMissionID() const { return missionID; }
	void setRday(int Rday) { this->Rday = Rday; }	
	void setMissionID(int missionID) { this->missionID = missionID; }
};

