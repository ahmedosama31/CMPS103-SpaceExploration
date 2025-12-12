#include "MarsStation.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include "newRequest.h"
#include "abortRequest.h"
#include "UI.h"
#include "mission.h"



using namespace std;

void MarsStation::LoadFromFile(const string& filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) { 
        cout << "Error: Cannot open input file." << endl;
        return; 
    }

    inputFile >> D >> P >> N;
    inputFile >> SD >> SP >> SN;
    inputFile >> M >> CD >> CP >> CN;

    int id = 1;
    for (int i = 0; i < D; i++) {
        Rover* r = new Rover(id++, RoverType::Digging, SD, M, CD);
        AvailableDiggingRovers.enqueue(r, RoverPriQueue::calculatePriority(r));
    }
    for (int i = 0; i < P; i++) {
        Rover* r = new Rover(id++, RoverType::Polar, SP, M, CP);
        AvailablePolarRovers.enqueue(r, RoverPriQueue::calculatePriority(r));
    }
    for (int i = 0; i < N; i++) {
        Rover* r = new Rover(id++, RoverType::Normal, SN, M, CN);
        AvailableNormalRovers.enqueue(r, RoverPriQueue::calculatePriority(r));
    }

    int K;
    inputFile >> K;
    for (int i = 0; i < K; i++) {
        char RType; inputFile >> RType;
        if (RType == 'R')
        {
            char MType;
            int RDay, ID, TLOC, MDUR;
            inputFile >> MType >> RDay >> ID >> TLOC >> MDUR;
            RequestsList.enqueue(new newRequest(RDay, ID, CharToMissionType(MType), TLOC, MDUR));
        }
        else if (RType == 'X') {
            int XDay, ID;
            inputFile >> XDay >> ID;
            RequestsList.enqueue(new abortRequest(XDay, ID));
        }
    }
    inputFile.close();
}

bool MarsStation::EnqueueAvailable(Rover* r)
{
    if (!r) return false;
    int pri = RoverPriQueue::calculatePriority(r);
    switch (r->getType()) {
    case RoverType::Digging: AvailableDiggingRovers.enqueue(r, pri); break;
    case RoverType::Polar:   AvailablePolarRovers.enqueue(r, pri);   break;
    case RoverType::Normal:  AvailableNormalRovers.enqueue(r, pri);  break;
    }
    return true;
}

bool MarsStation::EnqueueCheckup(Rover* r)
{
    if (!r) return false;
    switch (r->getType()) {
    case RoverType::Digging: CheckupDiggingRovers.enqueue(r); break;
    case RoverType::Polar:   CheckupPolarRovers.enqueue(r);   break;
    case RoverType::Normal:  CheckupNormalRovers.enqueue(r);  break;
    }
    return true;
}

void MarsStation::InsertMission(Mission* M)
{
    switch (M->getType()) {
    case MissionType::Digging: ReadyDiggingMissions.enqueue(M); break;
    case MissionType::Polar:   ReadyPolarMissions.enqueue(M);   break;
    case MissionType::Normal:  ReadyNormalMissions.enqueue(M);  break;
    }
}

bool MarsStation::AbortMission(int missionID, int currentDay)
{
    Mission* RDYAbortM = ReadyNormalMissions.Abortmission(missionID);
    if (RDYAbortM) 
    {
        RDYAbortM->setAborted(true);
        AbortedMissions.enqueue(RDYAbortM);
        return true;
    }
    
    Mission* OUTAbortM = OUTMissions.Abortmission(missionID);
    if (OUTAbortM) 
    {
        OUTAbortM->setAborted(true);
        // Mission aborted while in OUT list
        // Rover should start the back journey immediately.
        int traveledDays = currentDay - OUTAbortM->getLaunchDay();
        int returnDay = currentDay + traveledDays;
        BACKMissions.enqueue(OUTAbortM, -returnDay);
        return true;
    }

    return false;
}

void MarsStation::ExecuteRequests(int currentDay)
{
    Requests* req = nullptr;
    while (RequestsList.peek(req)) {
        if (req->getRday() > currentDay)
            return;
        RequestsList.dequeue(req);
        req->Operate(*this);
        delete req;
    }
}

void MarsStation::ReleaseRover(Rover* r, int currentDay)
{
    if(r) {
        r->incrementCompletedMissions();
        
        if (r->needsCheckup())
        {
            r->startCheckup(currentDay);
            EnqueueCheckup(r);
        }
        else
        {
            EnqueueAvailable(r);
        }
    }
}

void MarsStation::ManageCheckups(int currentDay)
{
    Rover* r = nullptr;

    // Digging
    while (CheckupDiggingRovers.peek(r))
    {
        if (r->getAvailableDay() <= currentDay) {
            CheckupDiggingRovers.dequeue(r);
            r->finishCheckup();
            EnqueueAvailable(r);
        }
        else break;
    }

    // Polar
    while (CheckupPolarRovers.peek(r))
    {
        if (r->getAvailableDay() <= currentDay) {
            CheckupPolarRovers.dequeue(r);
            r->finishCheckup();
            EnqueueAvailable(r);
        }
        else break;
    }

    // Normal
    while (CheckupNormalRovers.peek(r))
    {
        if (r->getAvailableDay() <= currentDay) {
            CheckupNormalRovers.dequeue(r);
            r->finishCheckup();
            EnqueueAvailable(r);
        }
        else break;
    }
}

void MarsStation::AutoAbortPolarMissions(int currentDay)
{
    LinkedQueue<Mission*> tempQueue;
    Mission* m = nullptr;

    while (ReadyPolarMissions.dequeue(m))
    {
        if ((currentDay - m->getRequestedDay()) > (2 * m->getMissionDuration()))
        {
            AbortedMissions.enqueue(m);
        }
        else
        {
            tempQueue.enqueue(m);
        }
    }

    while (tempQueue.dequeue(m))
    {
        ReadyPolarMissions.enqueue(m);
    }
}

void MarsStation::AssignMissions(int currentDay)        
{
    Mission* m = nullptr;
    Rover* r = nullptr;

    // POLAR Missions
    while (ReadyPolarMissions.peek(m))
    {
        r = nullptr;
        int pri;
        if (AvailablePolarRovers.dequeue(r, pri))
        {
            r->assignMission(m); 
        }
        else if (AvailableNormalRovers.dequeue(r, pri))
        {
            r->assignMission(m); 
        }
        else if (AvailableDiggingRovers.dequeue(r, pri))
        {
            r->assignMission(m); 
        }
        
        if (r != nullptr)
        {
            ReadyPolarMissions.dequeue(m);
            m->setLaunchDay(currentDay);
            m->setWaitingDays(currentDay - m->getRequestedDay());
            double travelHours = (double)m->getTargetLocation() / r->getSpeed();
            int travelDays = ceil(travelHours / 25.0);
            int arrivalDay = currentDay + travelDays;
            OUTMissions.enqueue(m, -arrivalDay);
        }
        else
        {
            break; 
        }
    }

    // DIGGING Missions
    while (ReadyDiggingMissions.peek(m))
    {
        r = nullptr; 
        int pri;
        if (AvailableDiggingRovers.dequeue(r, pri))
        {
            r->assignMission(m); 
            ReadyDiggingMissions.dequeue(m);
            m->setLaunchDay(currentDay);
            m->setWaitingDays(currentDay - m->getRequestedDay());
            double travelHours = (double)m->getTargetLocation() / r->getSpeed();
            int travelDays = ceil(travelHours / 25.0);
            int arrivalDay = currentDay + travelDays;
            OUTMissions.enqueue(m, -arrivalDay);
        }
        else
        {
            break;
        }
    }

    // NORMAL Missions
    while (ReadyNormalMissions.peek(m))
    {
        r = nullptr; 
        int pri;
        if (AvailableNormalRovers.dequeue(r, pri))
        {
            r->assignMission(m); 
        }
        else if (AvailablePolarRovers.dequeue(r, pri))
        {
            r->assignMission(m); 
        }

        if (r != nullptr)
        {
            ReadyNormalMissions.dequeue(m);
            m->setLaunchDay(currentDay);
            m->setWaitingDays(currentDay - m->getRequestedDay());
            double travelHours = (double)m->getTargetLocation() / r->getSpeed();
            int travelDays = ceil(travelHours / 25.0);
            int arrivalDay = currentDay + travelDays;
            OUTMissions.enqueue(m, -arrivalDay);
        }
        else
        {
            break;
        }
    }
}  

void MarsStation::UpdateOUTMissions(int currentDay)
{
    Mission* m = nullptr;
    int pri = 0;

    while (OUTMissions.peek(m, pri))
    {
        int arrivalDay = -pri;
        if (arrivalDay <= currentDay)
        {
            OUTMissions.dequeue(m, pri);
            int executionDuration = m->getMissionDuration();
            int Fday = currentDay + executionDuration;
            m->setExecutionDays(executionDuration); 
            EXECMissions.enqueue(m, -Fday);
        }
        else
            break;

    }
}

void MarsStation::UpdateEXECMissions(int currentDay)
{
    Mission* m = nullptr;
    int pri = 0;

    while (EXECMissions.peek(m, pri))
    {
        int Fday = -pri;
        if (Fday <= currentDay) 
        {
            EXECMissions.dequeue(m, pri);
            Rover* assignedRover = m->getAssignedRover();
            if (!assignedRover) {
                continue;
            }
            double speed = assignedRover->getSpeed();
            double targetloc = m->getTargetLocation();
            
            double travelHours = targetloc / speed;
            int travelDays =ceil(travelHours / 25.0); 
            int returnDay = currentDay + travelDays;

            BACKMissions.enqueue(m, -returnDay);
        }
        else
            break;
    }
}

void MarsStation::UpdateBACKMissions(int currentDay)
{
    Mission* m = nullptr;
    int pri = 0;

    while (BACKMissions.peek(m, pri))
    {
        int returnDay = -pri;
        if (returnDay <= currentDay)
        {
            BACKMissions.dequeue(m, pri);
            // Check if the mission was aborted (and moved to BACK as part of abort)
            // Check if the mission was aborted (and moved to BACK as part of abort)
            if (m->isAborted()) {
                AbortedMissions.enqueue(m);
            } else {
                DONEMissions.push(m);
                m->setCompletionDay(currentDay); 
            }
            Rover* assignedRover = m->getAssignedRover();
            if (assignedRover)
                ReleaseRover(assignedRover, currentDay);
        }
        else
            break;
    }
}

void MarsStation::Simulate()
{
    UI ui;
    
    // Prompt for Mode
    int mode;
    cout << "Select Mode (1: Interactive, 2: Silent): ";
    cin >> mode;
    cin.ignore(); 
    
    if (mode == 2) {
        cout << "Silent Mode" << endl;
    }
    cout << "Simulation Starts..." << endl;

    // Initialize
    int currentDay = 1; 
    LoadFromFile("input.txt"); 

    if (mode == 1)
    {
        ui.PrintDay(0, this);
        cout << "Press Enter to start simulation...";
        cin.get();
    }

    while (true)
    {
        ExecuteRequests(currentDay);
        AutoAbortPolarMissions(currentDay);
        ManageCheckups(currentDay);
        AssignMissions(currentDay);
        UpdateOUTMissions(currentDay);
        UpdateEXECMissions(currentDay);
        UpdateBACKMissions(currentDay);

        // Output / Interface 
        if (mode == 1)
        {
            ui.PrintDay(currentDay, this);
            cout << "Press Enter to continue...";
            cin.get(); 
        }


        //Termination Condition
        //Break if all missions are DONE (and all other lists empty)
        //Simple check for now:
        if (RequestsList.getCount() == 0 &&
            ReadyDiggingMissions.getCount() == 0 &&
            ReadyPolarMissions.getCount() == 0 &&
            ReadyNormalMissions.getCount() == 0 &&
            OUTMissions.getCount() == 0 &&
            EXECMissions.getCount() == 0 &&
            BACKMissions.getCount() == 0)
        {
            break;
        }

        currentDay++;
    }
    
    cout << "Simulation ends, Output file created" << endl;
    // GenerateOutputFile(); // Member 4
}

LinkedQueue<Rover*>& MarsStation::getCheckupDiggingRovers() { return CheckupDiggingRovers; }
LinkedQueue<Rover*>& MarsStation::getCheckupPolarRovers()   { return CheckupPolarRovers; }
LinkedQueue<Rover*>& MarsStation::getCheckupNormalRovers()  { return CheckupNormalRovers; }
RoverPriQueue& MarsStation::getAvailableDiggingRovers(){ return AvailableDiggingRovers; }
RoverPriQueue& MarsStation::getAvailablePolarRovers() { return AvailablePolarRovers; }
RoverPriQueue& MarsStation::getAvailableNormalRovers(){ return AvailableNormalRovers; }
LinkedQueue<Requests*>& MarsStation::getRequestsList()      { return RequestsList; }
LinkedQueue<Mission*>&  MarsStation::getReadyDiggingMissions(){ return ReadyDiggingMissions; }
LinkedQueue<Mission*>&  MarsStation::getReadyPolarMissions() { return ReadyPolarMissions; }
RDY_NM&                MarsStation::getReadyNormalMissions(){ return ReadyNormalMissions; }
OUT_missions&          MarsStation::getOUTMissions()        { return OUTMissions; }
priQueue<Mission*>&    MarsStation::getEXECMissions()       { return EXECMissions; }  
priQueue<Mission*>&    MarsStation::getBACKMissions()       { return BACKMissions; }   
LinkedQueue<Mission*>& MarsStation::getAbortedMissions()    { return AbortedMissions; }
ArrayStack<Mission*>&  MarsStation::getDONEMissions()       { return DONEMissions; }
