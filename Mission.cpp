#include "mission.h"


Mission::Mission(int id, MissionType type, int targetlocation,
                 int missionduration, int requestday)
    : ID(id), Type(type), TargetLocation(targetlocation),
      MissionDuration(missionduration), RequestedDay(requestday),
      WaitingDays(0), ExecutionDays(0), CompletionDay(0), LaunchDay(0), Aborted(false), Failed(false), Rescued(false), AssignedRover(nullptr) {}

int Mission::getID() const { return ID; }
MissionType Mission::getType() const { return Type; }
int Mission::getRequestedDay() const { return RequestedDay; }
int Mission::getTargetLocation() const { return TargetLocation; }
int Mission::getMissionDuration() const { return MissionDuration; }
int Mission::getWaitingDays() const { return WaitingDays; }
int Mission::getExecutionDays() const { return ExecutionDays; }
int Mission::getLaunchDay() const { return LaunchDay; }
int Mission::getCompletionDay() const { return CompletionDay; }
int Mission::getTurnaroundDays() const { return CompletionDay - LaunchDay; }
bool Mission::isAborted() const { return Aborted; }
bool Mission::isFailed() const { return Failed; }
bool Mission::isRescued() const { return Rescued; }
Rover* Mission::getAssignedRover() const { return AssignedRover; }

void Mission::setWaitingDays(int w) { WaitingDays = w; }
void Mission::setExecutionDays(int e) { ExecutionDays = e; }
void Mission::setLaunchDay(int l) { LaunchDay = l; }
void Mission::setCompletionDay(int c) { CompletionDay = c; }
void Mission::assignRover(Rover* r) { AssignedRover = r; }
void Mission::setAborted(bool a) { Aborted = a; }
void Mission::setFailed(bool f) { Failed = f; }
void Mission::setRescued(bool r) { Rescued = r; }

std::ostream& operator<<(std::ostream& os, const Mission& m)
{
    if (m.getAssignedRover()) {
        os << m.ID << "/" << m.getAssignedRover()->getID();
    }
    else {
        os << m.ID;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Mission* m)
{
    if (m)
        os << *m;   // call Mission& overload
    else
        os << "[NULL Mission]";
    return os;
}