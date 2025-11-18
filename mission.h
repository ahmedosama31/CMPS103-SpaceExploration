#pragma once
#include <iostream>
//#include "rover.h"
#include "MissionType.h"
using namespace std;
class rover; // forward declaration so we can use a pointer without including rover.h untill rover class is made
class mission {
private:
	int ID;
	MissionType Type;
	int TargetLocation;
	int MissionDuration;
	int RequestedDay;
	int WaitingDays;
	int ExecutionDays;
	int CompletionDay;
	rover* AssignedRover;
public:
	// Constructor
	mission(int id = 0, MissionType type = MissionType::Normal, int targetlocation = 0, int missionduration = 0, int requestday = 0)
		: ID(id), Type(type), TargetLocation(targetlocation), MissionDuration(missionduration),
		RequestedDay(requestday), WaitingDays(0), ExecutionDays(0), CompletionDay(0), AssignedRover(nullptr) {
	}
	// Getters
	int getID() const { return ID; }
	MissionType getType() const { return Type; }
	int getRequestedDay() const { return RequestedDay; }
	int getTargetLocation() const { return TargetLocation; }
	int getMissionDuration() const { return MissionDuration; }
	int getWaitingDays() const { return WaitingDays; }
	int getExecutionDays() const { return ExecutionDays; }
	int getCompletionDay() const { return CompletionDay; }
	rover* getAssignedRover() const { return AssignedRover; }

	// Setters
	void setWaitingDays(int w) { WaitingDays = w; }
	void setExecutionDays(int e) { ExecutionDays = e; }
	void setCompletionDay(int c) { CompletionDay = c; }
	void assignRover(rover* r) { AssignedRover = r; }

	friend ostream& operator<<(ostream& os, const mission& m) {
		os << "[Mission ID: " << m.ID << ", Type: ";

		if (m.Type == MissionType::Digging)
			os << "D";
		else if (m.Type == MissionType::Polar)
			os << "P";
		else
			os << "N";

		os << ", Requested Day: " << m.RequestedDay
			<< ", Target: " << m.TargetLocation << " km"
			<< ", Duration: " << m.MissionDuration << " days]";

		return os;
	}
};
