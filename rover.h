#pragma once
#include <iostream>
#include "Enums.h"
using namespace std;

class Mission; // Forward declaration

class Rover {
private:
    int ID;
    RoverType type;
    double speed;
    int missionsBeforeCheckup;
    int checkupDuration;
    int completedMissions;
    double totalDistance;
    bool inCheckup;
    int availableDay;

public:
    // Constructor
    Rover(int id, RoverType t, double spd, int missionsBC, int checkDur);

    // Getters
    int getID() const;
    RoverType getType() const;
    double getSpeed() const;
    int getMissionsBeforeCheckup() const;
    int getCheckupDuration() const;
    int getCompletedMissions() const;
    double getTotalDistance() const;
    bool isInCheckup() const;
    int getAvailableDay() const;

    // Setters
    void setSpeed(double spd);
    void setAvailableDay(int day);
    void setInCheckup(bool state);

    // Operations
    void assignMission(Mission* mission);
    void incrementCompletedMissions();
    void startCheckup(int currentDay);
    void finishCheckup();
    bool needsCheckup() const;

};

// Output operator
ostream& operator<<(ostream& os, const Rover& r);
