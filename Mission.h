#pragma once
#include <iostream>
//#include "rover.h"
using namespace std;
class rover; // forward declaration so we can use a pointer without including rover.h untill rover class is made

class mission {
private:
	int ID;
	char Type;
	int TargetLocation;
	int MissionDuration;
	int RequestedDay;
	int WaitingDays;
	int ExecutionDays;
	int CompletionDay;
	rover* AssignedRover;
public:
	// Constructor
	mission(int id = 0,char type = 'N', int targetlocation = 0, int missionduration = 0, int requestday = 0)
		: ID(id), Type(type), TargetLocation(targetlocation), MissionDuration(missionduration),
		RequestedDay(requestday), WaitingDays(0), ExecutionDays(0), CompletionDay(0), AssignedRover(nullptr) {
	}
	// Getters
	int getID() const { return ID; }
	char getType() const { return Type; }
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

	void print() const
	{
		cout << "[Mission ID: " << ID
			<< ", Type: " << Type
			<< ", Requested Day: " << RequestedDay
			<< ", Target: " << TargetLocation << " km"
			<< ", Duration: " << MissionDuration << " days]"
			<< endl;
	}
};