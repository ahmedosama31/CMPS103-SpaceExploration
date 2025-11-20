#pragma once
#include "Requests.h"
#include "Enums.h"
#include "mission.h"
class MarsStation; // forward declaration

class newRequest : public Requests {
private:
    MissionType MType;
    int TLOC;
    int MDUR;
public:
    newRequest(int RDay, int ID, MissionType MType, int TLOC, int MDUR)
        : Requests(RDay, ID), MType(MType), TLOC(TLOC), MDUR(MDUR) {}

    MissionType getMissionType() const { return MType; }
    int getTargetLocation() const { return TLOC; }
    int getMissionDuration() const { return MDUR; }
    void setMissionType(MissionType t) { MType = t; }
    void setTargetLocation(int loc) { TLOC = loc; }
    void setMissionDuration(int dur) { MDUR = dur; }
    void print(std::ostream& os) const override;
    void Operate(MarsStation& marsStation) override;
};

