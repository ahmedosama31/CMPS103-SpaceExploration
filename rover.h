#pragma once
#include <iostream>
#include <string>
#include "Enums.h"
using namespace std;

class Rover {
private:
    int ID;
    RoverType type;
    double speed;              // in km/h
    int missionsBeforeCheckup; // number of missions before checkup
    int checkupDuration;       // number of days in checkup
    int completedMissions;     // missions done since last checkup
    double totalDistance;      // total distance traveled
    bool inCheckup;            // flag if rover is in checkup
    int availableDay;          // next available day for use

public:
    // ----- Constructors -----

    Rover(int id, RoverType t, double spd, int missionsBC, int checkDur)
        : ID(id), type(t), speed(spd), missionsBeforeCheckup(missionsBC),
        checkupDuration(checkDur), completedMissions(0),
        totalDistance(0), inCheckup(false), availableDay(0) {
    }

    // ----- Getters -----
    int getID() const { return ID; }
    RoverType getType() const { return type; }
    double getSpeed() const { return speed; }
    int getMissionsBeforeCheckup() const { return missionsBeforeCheckup; }
    int getCheckupDuration() const { return checkupDuration; }
    int getCompletedMissions() const { return completedMissions; }
    double getTotalDistance() const { return totalDistance; }
    bool isInCheckup() const { return inCheckup; }
    int getAvailableDay() const { return availableDay; }

    // ----- Setters -----
    void setSpeed(double spd) { speed = spd; }
    void setAvailableDay(int day) { availableDay = day; }
    void setInCheckup(bool state) { inCheckup = state; }

    // ----- Operations -----
    void assignMission(double distance) {
        totalDistance += distance;
        completedMissions++;
    }

    void startCheckup(int currentDay) {
        inCheckup = true;
        availableDay = currentDay + checkupDuration;
    }

    void finishCheckup() {
        inCheckup = false;
        completedMissions = 0; // reset after checkup
    }

    bool needsCheckup() const {
        return completedMissions >= missionsBeforeCheckup;
    }

    friend ostream& operator<<(ostream& os, const Rover& r) {
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
};
