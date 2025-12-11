#include "mission.h"


Mission::Mission(int id, MissionType type, int targetlocation,
                 int missionduration, int requestday)
    : ID(id), Type(type), TargetLocation(targetlocation),
      MissionDuration(missionduration), RequestedDay(requestday),
      WaitingDays(0), ExecutionDays(0), CompletionDay(0), Aborted(false), AssignedRover(nullptr) {}

int Mission::getID() const { return ID; }
MissionType Mission::getType() const { return Type; }
int Mission::getRequestedDay() const { return RequestedDay; }
int Mission::getTargetLocation() const { return TargetLocation; }
int Mission::getMissionDuration() const { return MissionDuration; }
int Mission::getWaitingDays() const { return WaitingDays; }
int Mission::getExecutionDays() const { return ExecutionDays; }
int Mission::getCompletionDay() const { return CompletionDay; }
bool Mission::isAborted() const { return Aborted; }
Rover* Mission::getAssignedRover() const { return AssignedRover; }

void Mission::setWaitingDays(int w) { WaitingDays = w; }
void Mission::setExecutionDays(int e) { ExecutionDays = e; }
void Mission::setCompletionDay(int c) { CompletionDay = c; }
void Mission::assignRover(Rover* r) { AssignedRover = r; }
void Mission::setAborted(bool a) { Aborted = a; }

std::ostream& operator<<(std::ostream& os, const Mission& m)
{
    os << "[Mission ID: " << m.ID
        << ", Type: ";

    switch (m.Type) {
    case MissionType::Digging: os << "D"; break;
    case MissionType::Polar:   os << "P"; break;
    case MissionType::Normal:  os << "N"; break;
    }

    os << ", RDay: " << m.RequestedDay
        << ", TLOC: " << m.TargetLocation
        << ", MDUR: " << m.MissionDuration;
        if (m.getAssignedRover()) {
            os << ", Rover: " << m.getAssignedRover()->getID();
        }
        else {
            os << ", Rover: -";
        }
    os << "]";

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