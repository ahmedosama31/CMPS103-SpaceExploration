#include "Requests.h"

Requests::Requests(int rday, int id)
    : RDay(rday), missionID(id) {}

int Requests::getRday() const { return RDay; }
int Requests::getMissionID() const { return missionID; }
void Requests::setMday(int Rday) { this->RDay = Rday; }
void Requests::setMissionID(int missionID) { this->missionID = missionID; }
