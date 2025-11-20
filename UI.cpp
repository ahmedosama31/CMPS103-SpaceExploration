#include "UI.h"
#include "MarsStation.h"

void UI::PrintDay(int day, MarsStation* ms)
{
    cout << "\n===================== Day " << day << " =====================\n\n";

    cout << "============= Requests List =============\n";
    ms->getRequestsList().print();

    cout << "============= Ready Missions =============\n";
    cout << "Digging: ";   ms->getReadyDiggingMissions().print();
    cout << "Polar:   ";   ms->getReadyPolarMissions().print();
    cout << "Normal:  ";   ms->getReadyNormalMissions().print();

    cout << "============= Available Rovers =============\n";
    cout << "Digging: ";   ms->getAvailableDiggingRovers().print();
    cout << "Polar:   ";   ms->getAvailablePolarRovers().print();
    cout << "Normal:  ";   ms->getAvailableNormalRovers().print();

    cout << "============= OUT Missions =============\n";
    ms->getOUTMissions().print();

    cout << "============= EXEC Missions =============\n";
    ms->getEXECMissions().print();

    cout << "============= BACK Missions =============\n";
    ms->getBACKMissions().print();

    cout << "============= Aborted Missions =============\n";
    ms->getAbortedMissions().print();
    
    cout << "============= Checkup Rovers =============\n";
    cout << "Digging: ";   ms->getCheckupDiggingRovers().print();
    cout << "Polar:   ";   ms->getCheckupPolarRovers().print();
    cout << "Normal:  ";   ms->getCheckupNormalRovers().print();
    cout << "============= DONE Missions =============\n";
    ms->getDONEMissions().print();

    cout << "\n-----------------------------------------------------------\n\n";
}
