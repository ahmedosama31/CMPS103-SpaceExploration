#include "mission.h"


Mission::Mission(int id, MissionType type, int targetlocation,
                 int missionduration, int requestday)
    : ID(id), Type(type), TargetLocation(targetlocation),
      MissionDuration(missionduration), RequestedDay(requestday),
      WaitingDays(0), ExecutionDays(0), CompletionDay(0), AssignedRover(nullptr) {}

int Mission::getID() const { return ID; }
MissionType Mission::getType() const { return Type; }
int Mission::getRequestedDay() const { return RequestedDay; }
int Mission::getTargetLocation() const { return TargetLocation; }
int Mission::getMissionDuration() const { return MissionDuration; }
int Mission::getWaitingDays() const { return WaitingDays; }
int Mission::getExecutionDays() const { return ExecutionDays; }
int Mission::getCompletionDay() const { return CompletionDay; }
Rover* Mission::getAssignedRover() const { return AssignedRover; }

void Mission::setWaitingDays(int w) { WaitingDays = w; }
void Mission::setExecutionDays(int e) { ExecutionDays = e; }
void Mission::setCompletionDay(int c) { CompletionDay = c; }
void Mission::assignRover(Rover* r) { AssignedRover = r; }

std::ostream& operator<<(std::ostream& os, const Mission& m) {
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