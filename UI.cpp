#include "UI.h"
#include "MarsStation.h"

void UI::PrintDay(int day, MarsStation* ms)
{
    cout << "Current Day:" << day << endl;

    cout << "================ Requests List =================" << endl;
    cout << ms->getRequestsList().getCount() << " requests remaining: ";
    ms->getRequestsList().print(10); 
    cout << endl;
    
    cout << "================ Ready List(s) =================" << endl;
    int rdyCount = ms->getReadyNormalMissions().getCount() + 
                   ms->getReadyPolarMissions().getCount() + 
                   ms->getReadyDiggingMissions().getCount();
    cout << rdyCount << " Missions: ";
    
    cout << "NMs["; ms->getReadyNormalMissions().print(); cout << "] ";
    cout << "PMs["; ms->getReadyPolarMissions().print(); cout << "] ";
    cout << "DMs["; ms->getReadyDiggingMissions().print(); cout << "]" << endl;

    cout << "================ Available Rovers List(s) =================" << endl;
    int availCount = ms->getAvailableNormalRovers().getCount() + 
                     ms->getAvailablePolarRovers().getCount() + 
                     ms->getAvailableDiggingRovers().getCount();
    cout << availCount << " Available Rovers: ";
    cout << "NR["; ms->getAvailableNormalRovers().print(); cout << "]  ";
    cout << "PR["; ms->getAvailablePolarRovers().print(); cout << "]  ";
    cout << "DR["; ms->getAvailableDiggingRovers().print(); cout << "]" << endl;

    cout << "================ OUT List(s) =================" << endl;
    cout << ms->getOUTMissions().getCount() << " Missions/Rovers: ";
    ms->getOUTMissions().print(day);
    cout << endl;

    cout << "================ EXEC List(s) =================" << endl;
    cout << ms->getEXECMissions().getCount() << " Missions/Rovers: ";
    ms->getEXECMissions().print(day);
    cout << endl;

    cout << "================ BACK List(s) =================" << endl;
    cout << ms->getBACKMissions().getCount() << " Missions/Rovers: ";
    ms->getBACKMissions().print(day);
    cout << endl;

    cout << "================ Aborted List(s) =================" << endl;
    cout << ms->getAbortedMissions().getCount() << " Missions: [";
    ms->getAbortedMissions().print(); 
    cout << "]" << endl;

    cout << "================ Checkup List(s) =================" << endl;
    int checkupCount = ms->getCheckupNormalRovers().getCount() + 
                       ms->getCheckupPolarRovers().getCount() + 
                       ms->getCheckupDiggingRovers().getCount();
    cout << checkupCount << " Rovers: [";
    
    // Checkup list needs careful comma handling between lists
    bool printed = false;
    if (ms->getCheckupNormalRovers().getCount() > 0) {
        ms->getCheckupNormalRovers().print();
        printed = true;
    }
    
    if (ms->getCheckupPolarRovers().getCount() > 0) {
        if (printed) cout << ", ";
        ms->getCheckupPolarRovers().print();
        printed = true;
    }

    if (ms->getCheckupDiggingRovers().getCount() > 0) {
        if (printed) cout << ", ";
        ms->getCheckupDiggingRovers().print();
    }
    
    cout << "]" << endl;

    cout << "================ DONE List(s) =================" << endl;
    cout << ms->getDONEMissions().getCount() << " Missions: ";
    ms->getDONEMissions().print();
    cout << endl;
    
    cout << endl;
}
