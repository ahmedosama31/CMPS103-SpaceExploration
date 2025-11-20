#pragma once
#include <iostream>
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
    virtual void print(std::ostream& os) const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Requests& req) {
        req.print(os);
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const Requests* req) {
        if (req) req->print(os);
        else os << "[NULL Request]";
        return os;
    }

};