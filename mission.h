#pragma once
#include <iostream>
using namespace std;
#include "Enums.h"
#include "rover.h"
class Mission {
private:
    int ID;
    MissionType Type;
    int TargetLocation;
    int MissionDuration;
    int RequestedDay;
    int WaitingDays;
    int ExecutionDays;
    int LaunchDay;
    bool Aborted;
    bool Failed;
    bool Rescued;
    int CompletionDay;
    Rover* AssignedRover;
public:
    Mission(int id = 0, MissionType type = MissionType::Normal, int targetlocation = 0,
            int missionduration = 0, int requestday = 0);

    // Getters
    int getID() const;
    MissionType getType() const;
    int getRequestedDay() const;
    int getTargetLocation() const;
    int getMissionDuration() const;
    int getWaitingDays() const;
    int getExecutionDays() const;
    int getLaunchDay() const;
    int getCompletionDay() const;
    int getTurnaroundDays() const;
    bool isAborted() const;
    bool isFailed() const;
    bool isRescued() const;
    Rover* getAssignedRover() const;

    // Setters
    void setWaitingDays(int w);
    void setExecutionDays(int e);
    void setLaunchDay(int l);
    void setCompletionDay(int c);
    void assignRover(Rover* r);
    void setAborted(bool a);
    void setFailed(bool f);
    void setRescued(bool r);
    
};

// Output operator 
std::ostream& operator<<(std::ostream& os, const Mission& m);