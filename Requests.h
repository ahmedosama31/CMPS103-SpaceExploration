#pragma once

class MarsStation; // Forward declaration because using #include "MarsStation" leads to errors as MarsStation has #include "Requests.h"

class Requests {
protected:
    int RDay;
    int missionID;
public:
    Requests(int rday, int id);
    virtual ~Requests() = default;

    virtual void Operate(MarsStation& marsStation) = 0;

    int getRday() const;
    int getMissionID() const;
    void setMday(int Rday);
    void setMissionID(int missionID);
};