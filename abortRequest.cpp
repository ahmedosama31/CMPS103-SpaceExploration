#include "abortRequest.h"
#include "MarsStation.h"

void abortRequest::Operate(MarsStation& marsStation)
{
    marsStation.AbortMission(getMissionID(), RDay);
}
void abortRequest::print(std::ostream& os) const
{
    os << "[X, " << RDay << ", M" << missionID << "]";
}