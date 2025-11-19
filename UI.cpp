#include "UI.h"
#include "MarsStation.h"

void UI::PrintDay(int day, MarsStation* pS)
{
    cout << "\n===================== Day " << day << " =====================\n\n";

    cout << "============= Requests List =============\n";
    pS->getRequestsList().print();

    cout << "============= Ready Missions =============\n";
    cout << "Digging: ";   pS->getReadyDiggingMissions().print();
    cout << "Polar:   ";   pS->getReadyPolarMissions().print();
    cout << "Normal:  ";   pS->getReadyNormalMissions().print();

    cout << "============= Available Rovers =============\n";
    cout << "Digging: ";   pS->getAvailableDiggingRovers().print();
    cout << "Polar:   ";   pS->getAvailablePolarRovers().print();
    cout << "Normal:  ";   pS->getAvailableNormalRovers().print();

    cout << "============= OUT Missions =============\n";
    pS->getOUTMissions().print();

    cout << "============= EXEC Missions =============\n";
    pS->getEXECMissions().print();

    cout << "============= BACK Missions =============\n";
    pS->getBACKMissions().print();

    cout << "============= Aborted Missions =============\n";
    pS->getAbortedMissions().print();

    cout << "============= DONE Missions =============\n";
    pS->getDONEMissions().print();

    cout << "\n-----------------------------------------------------------\n\n";
}
