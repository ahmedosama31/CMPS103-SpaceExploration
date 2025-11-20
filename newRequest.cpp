#include "newRequest.h"
#include "MarsStation.h"

void newRequest::Operate(MarsStation& marsStation) {
    Mission* M = new Mission(getMissionID(), getMissionType(),
                             getTargetLocation(), getMissionDuration(),
                             getRday());
    marsStation.InsertMission(M);
}
void newRequest::print(std::ostream& os) const
{
    os << "[R, "
        << MissionTypeToChar(MType) << ", "
        << RDay << ", M" << missionID
        << "]";
}