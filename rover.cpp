#include "Rover.h"

Rover::Rover(int id, RoverType t, double spd, int missionsBC, int checkDur)
    : ID(id), type(t), speed(spd), missionsBeforeCheckup(missionsBC),
    checkupDuration(checkDur), completedMissions(0),
    totalDistance(0), inCheckup(false), availableDay(0) {
}


int Rover::getID() const { return ID; }
RoverType Rover::getType() const { return type; }
double Rover::getSpeed() const { return speed; }
int Rover::getMissionsBeforeCheckup() const { return missionsBeforeCheckup; }
int Rover::getCheckupDuration() const { return checkupDuration; }
int Rover::getCompletedMissions() const { return completedMissions; }
double Rover::getTotalDistance() const { return totalDistance; }
bool Rover::isInCheckup() const { return inCheckup; }
int Rover::getAvailableDay() const { return availableDay; }


void Rover::setSpeed(double spd) { speed = spd; }
void Rover::setAvailableDay(int day) { availableDay = day; }
void Rover::setInCheckup(bool state) { inCheckup = state; }


void Rover::assignMission(double distance) {
    totalDistance += distance;
    completedMissions++;
}

void Rover::startCheckup(int currentDay) {
    inCheckup = true;
    availableDay = currentDay + checkupDuration;
}

void Rover::finishCheckup() {
    inCheckup = false;
    completedMissions = 0;
}

bool Rover::needsCheckup() const {
    return completedMissions >= missionsBeforeCheckup;
}

// ----- Output operator for Rover& -----
ostream& operator<<(ostream& os, const Rover& r)
{
    os << "[Rover ID: " << r.ID << ", Type: ";

    if (r.type == RoverType::Digging)
        os << "D";
    else if (r.type == RoverType::Polar)
        os << "P";
    else
        os << "N";

    os << ", Speed: " << r.speed << " km/h"
        << ", Missions Before Checkup: " << r.missionsBeforeCheckup
        << ", Completed Missions: " << r.completedMissions
        << ", Total Distance: " << r.totalDistance << " km"
        << ", In Checkup: " << (r.inCheckup ? "Yes" : "No")
        << ", Available Day: " << r.availableDay
        << "]";

    return os;
}

ostream& operator<<(ostream& os, const Rover* r)
{
    if (r)
        os << *r;      // Call the operator<< for Rover&
    else
        os << "[NULL Rover]";

    return os;
}
