#include "abortRequest.h"
#include "MarsStation.h"

void abortRequest::Operate(MarsStation& marsStation)
{
    marsStation.AbortMission(getMissionID());
}
