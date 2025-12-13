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

    // ========== INPUT FILE FORMAT ==========
    // Line 1: D P N R (number of Digging, Polar, Normal, Rescue rovers)
    // Line 2: M CR FP (missions before checkup, checkup duration, failure probability %)
    // Line 3: Individual speeds for each Digging rover (D values)
    // Line 4: Individual speeds for each Polar rover (P values)
    // Line 5: Individual speeds for each Normal rover (N values)
    // Line 6: Individual speeds for each Rescue rover (R values)
    // Line 7: K (total number of requests)
    // Lines 8+: Request lines (R = new mission, X = abort mission)
    // =======================================

    // Read rover counts for each type
    inputFile >> D >> P >> N >> R;
    
    // Read shared parameters: missions before checkup, checkup duration, failure probability
    inputFile >> M >> CR >> FP;

    int roverID = 1;  // Unique ID counter for all rovers
    
    // === CREATE DIGGING ROVERS ===
    // Read individual speed for each Digging rover
    for (int i = 0; i < D; i++) {
        int roverSpeed;
        inputFile >> roverSpeed;
        Rover* digRover = new Rover(roverID++, RoverType::Digging, roverSpeed, M, CR);
        AvailableDiggingRovers.enqueue(digRover, RoverPriQueue::calculatePriority(digRover));
    }
    
    // === CREATE POLAR ROVERS ===
    // Read individual speed for each Polar rover
    for (int i = 0; i < P; i++) {
        int roverSpeed;
        inputFile >> roverSpeed;
        Rover* polarRover = new Rover(roverID++, RoverType::Polar, roverSpeed, M, CR);
        AvailablePolarRovers.enqueue(polarRover, RoverPriQueue::calculatePriority(polarRover));
    }
    
    // === CREATE NORMAL ROVERS ===
    // Read individual speed for each Normal rover
    for (int i = 0; i < N; i++) {
        int roverSpeed;
        inputFile >> roverSpeed;
        Rover* normalRover = new Rover(roverID++, RoverType::Normal, roverSpeed, M, CR);
        AvailableNormalRovers.enqueue(normalRover, RoverPriQueue::calculatePriority(normalRover));
    }
    
    // === CREATE RESCUE ROVERS ===
    // Read individual speed for each Rescue rover
    for (int i = 0; i < R; i++) {
        int roverSpeed;
        inputFile >> roverSpeed;
        Rover* rescueRover = new Rover(roverID++, RoverType::Rescue, roverSpeed, M, CR);
        AvailableRescueRovers.enqueue(rescueRover, RoverPriQueue::calculatePriority(rescueRover));
    }

    // === READ MISSION/ABORT REQUESTS ===
    int totalRequests;
    inputFile >> totalRequests;  // K: total number of request lines
    
    for (int i = 0; i < totalRequests; i++) {
        char requestType;
        inputFile >> requestType;
        
        if (requestType == 'R')  // New Mission Request
        {
            char missionType;  // D=Digging, P=Polar, N=Normal
            int requestDay, missionID, targetLocation, missionDuration;
            inputFile >> missionType >> requestDay >> missionID >> targetLocation >> missionDuration;
            RequestsList.enqueue(new newRequest(requestDay, missionID, CharToMissionType(missionType), targetLocation, missionDuration));
        }
        else if (requestType == 'X')  // Abort Mission Request
        {
            int abortDay, missionID;
            inputFile >> abortDay >> missionID;
            RequestsList.enqueue(new abortRequest(abortDay, missionID));
        }
    }
    
    inputFile.close();
}

void MarsStation::EnqueueAvailable(Rover* r)
{
    if (!r) return;
    int pri = RoverPriQueue::calculatePriority(r);
    if (r->getType() == RoverType::Digging)
        AvailableDiggingRovers.enqueue(r, pri);
    else if (r->getType() == RoverType::Polar)
        AvailablePolarRovers.enqueue(r, pri);
    else if (r->getType() == RoverType::Normal)
        AvailableNormalRovers.enqueue(r, pri);
    else if (r->getType() == RoverType::Rescue)
        AvailableRescueRovers.enqueue(r, pri);
}

bool MarsStation::EnqueueCheckup(Rover* r)
{
    if (!r) return false;
    switch (r->getType()) {
    case RoverType::Digging: CheckupDiggingRovers.enqueue(r); break;
    case RoverType::Polar:   CheckupPolarRovers.enqueue(r);   break;
    case RoverType::Normal:  CheckupNormalRovers.enqueue(r);  break;
    case RoverType::Rescue:  CheckupRescueRovers.enqueue(r);  break;
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

    // Rescue
    while (CheckupRescueRovers.peek(r))
    {
        if (r->getAvailableDay() <= currentDay) {
            CheckupRescueRovers.dequeue(r);
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

void MarsStation::HandleRescueMissions(int currentDay)
{
    LinkedQueue<Mission*> tempQueue;
    Mission* m = nullptr;
    Rover* r = nullptr;
    int pri;

    while(FailedMissions.dequeue(m))
    {
        r = nullptr;
        
        // RESCUE ROVERS ONLY 
        AvailableRescueRovers.dequeue(r, pri);

        if (r) {
            r->assignMission(m);
            m->setRescued(true); // Mark as rescued
            
            // Recalculate travel
            double travelHours = (double)m->getTargetLocation() / r->getSpeed();
            int travelDays = ceil(travelHours / 25.0);
            int arrivalDay = currentDay + travelDays;
            
            // Set launch day again
            m->setLaunchDay(currentDay);
            m->setWaitingDays(currentDay - m->getRequestedDay());
            
            OUTMissions.enqueue(m, -arrivalDay);
        } else {
            // No rescue rover available - mission waits in FailedMissions
            tempQueue.enqueue(m);
        }
    }
    
    while(tempQueue.dequeue(m)) {
        FailedMissions.enqueue(m);
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
        // Probability of failure: FP%
        int randFail = rand() % 100;
        if (randFail < FP) 
        {
            OUTMissions.dequeue(m, pri);
            m->setFailed(true);
            
            // Store the failed rover in StrandedRovers list
            Rover* failedRover = m->getAssignedRover();
            if (failedRover) {
                StrandedRovers.enqueue(failedRover);
                m->assignRover(nullptr); // Unassign rover from mission
            }
            
            // Mission waits for rescue
            FailedMissions.enqueue(m);
            continue; 
        }

        int arrivalDay = -pri;
        if (arrivalDay <= currentDay)
        {
            OUTMissions.dequeue(m, pri);
            
            // Check if this is a RESCUE mission arriving at failed mission location
            if (m->isRescued()) {
                // Rescue rover has arrived - send stranded rover back
                Rover* strandedRover = nullptr;
                if (StrandedRovers.dequeue(strandedRover)) {
                    // Calculate return journey for stranded rover
                    double travelHours = (double)m->getTargetLocation() / strandedRover->getSpeed();
                    int travelDays = ceil(travelHours / 25.0);
                    int returnDay = currentDay + travelDays;
                    
                    // Create dummy mission to bring stranded rover home
                    Mission* returnM = new Mission(-1, m->getType(), m->getTargetLocation(), 0, currentDay);
                    returnM->assignRover(strandedRover);
                    BACKMissions.enqueue(returnM, -returnDay);
                }
            }
            
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
        // Probability of failure: FP%
        int randFail = rand() % 100;
        if (randFail < FP) 
        {
            EXECMissions.dequeue(m, pri);
            m->setFailed(true);
            
            // Store the failed rover in StrandedRovers list
            Rover* failedRover = m->getAssignedRover();
            if (failedRover) {
                StrandedRovers.enqueue(failedRover);
                m->assignRover(nullptr); // Unassign rover from mission
            }
            
            // Mission waits for rescue
            FailedMissions.enqueue(m);
            continue;
        }

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

// Output File Generation
void MarsStation::GenerateOutputFile(int totalDays)
{
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cout << "Error: Cannot open output file." << endl;
        return;
    }
    // Header
    outputFile << "Simulation Output File\n" << endl;
    outputFile << "----------------------\n" << endl;
    outputFile << "Fday\tID\tRday\tWdays\tMdur\tTdays\n";
    
    long Total_Wdays = 0;
	long Total_Tdays = 0;
	long Total_Mdur = 0;
    
    int polarDoneCount = 0;
    int polarAbortedCount = 0;
    int rescuedCount = 0;  // Track rescued missions

    // Preserve DONEMissions while iterating for output
    ArrayStack<Mission*> tempDONEMissions;
    Mission* m;
    int MissionDoneCount = 0;
    
    while (!DONEMissions.isEmpty()) {
		DONEMissions.pop(m);
        
        // Skip dummy return missions (ID == -1) from statistics and output
        if (m->getID() == -1) {
            tempDONEMissions.push(m);
            continue;
        }

        MissionDoneCount++;
        if (m->getType() == MissionType::Polar) polarDoneCount++;
        if (m->isRescued()) rescuedCount++;
        
		tempDONEMissions.push(m);
        
        outputFile << m->getCompletionDay() << "\t"
                   << m->getID() << "\t"
                   << m->getRequestedDay() << "\t"
                   << m->getWaitingDays() << "\t"
                   << m->getMissionDuration() << "\t"
                   << m->getTurnaroundDays() << "\n";
		Total_Wdays += m->getWaitingDays();
		Total_Tdays += m->getTurnaroundDays();
		Total_Mdur += m->getMissionDuration();
	}
    // Restore DONEMissions
    while (!tempDONEMissions.isEmpty()) {
        tempDONEMissions.pop(m);
		DONEMissions.push(m);
    }

	outputFile << "\n----------------------\n" << endl;
	
    // Statistics
    int Mission_Aborted = 0;
    LinkedQueue<Mission*> tempAbortedMissions; 
    
    // Counters for mission types (Done + Aborted)
    int nN = 0, nP = 0, nD = 0;

    ArrayStack<Mission*> tempStackForCounts;
    while (!DONEMissions.isEmpty()) {
        DONEMissions.pop(m);
        if (m->getType() == MissionType::Normal) nN++;
        else if (m->getType() == MissionType::Polar) nP++;
        else if (m->getType() == MissionType::Digging) nD++;
        tempStackForCounts.push(m);
    }
    while (!tempStackForCounts.isEmpty()) {
        tempStackForCounts.pop(m);
        DONEMissions.push(m);
    }

    while (!AbortedMissions.isEmpty()) {
        AbortedMissions.dequeue(m);
        Mission_Aborted++;
        if (m->getType() == MissionType::Normal) nN++;
        else if (m->getType() == MissionType::Polar) {
            nP++;
            polarAbortedCount++;
        }
        else if (m->getType() == MissionType::Digging) nD++;
        
        tempAbortedMissions.enqueue(m);
    }
    // Restore AbortedMissions
    while (!tempAbortedMissions.isEmpty()) {
        tempAbortedMissions.dequeue(m);
        AbortedMissions.enqueue(m);
    }
    
    int MissionTotalCount = MissionDoneCount + Mission_Aborted;
    
    double Average_Wdays = (MissionDoneCount > 0) ? ((double)Total_Wdays / MissionDoneCount) : 0.0;
    double Average_Tdays = (MissionDoneCount > 0) ? ((double)Total_Tdays / MissionDoneCount) : 0.0;
	double Avg_Mdur = (MissionDoneCount > 0) ? ((double)Total_Mdur / MissionDoneCount) : 0.0;
    
    int totalPolar = polarDoneCount + polarAbortedCount;
    double autoAbortedPercentage = (totalPolar > 0) ? ((double)polarAbortedCount / totalPolar) * 100.0 : 0.0;

    outputFile << "Missions: " << MissionTotalCount << " [N: " << nN << ", P: " << nP << ", D: " << nD << "]"
               << " [Done: " << MissionDoneCount << ", Aborted: " << Mission_Aborted << ", Failed: " << FailedMissions.getCount() << "]"
               << "\nRovers: " << (D + P + N + R) << " [D: " << D << ", P: " << P << ", N: " << N << ", R: " << R << "]"
               << "\nRescue Missions: " << rescuedCount
               << "\nFailure Probability: " << FP << "%"
               << "\nAvg Wdays = " << Average_Wdays 
               << ", Avg MDUR = " << Avg_Mdur 
               << ", Avg Tdays = " << Average_Tdays << endl;
    
    outputFile << "% Avg_Wdays / Avg_MDUR = " << (Avg_Mdur > 0 ? (Average_Wdays / Avg_Mdur) * 100 : 0) << "%"
               << ", Auto-aborted = " << autoAbortedPercentage << "%" << endl;
    
    outputFile << "Total Simulation Days = " << totalDays << endl; 

    outputFile.close();
}

void MarsStation::Simulate()
{
    UI ui;
    
    int mode;
    cout << "Select Mode (1: Interactive, 2: Silent): ";
    cin >> mode;
    cin.ignore(); 
    
    if (mode == 2) {
        cout << "Silent Mode" << endl;
        cout << "Simulation Starts..." << endl;
    } else {
        cout << "Interactive Mode" << endl;
        cout << "Simulation Starts..." << endl;
    }

    int currentDay = 1; 
    LoadFromFile("input.txt"); 

    if (mode == 1)
    {
        ui.PrintDay(1, this);
        cout << "Press Enter to start simulation...";
        cin.get();
    }

    while (true)
    {
        ExecuteRequests(currentDay);
        AutoAbortPolarMissions(currentDay);
        ManageCheckups(currentDay);
        HandleRescueMissions(currentDay);
        UpdateBACKMissions(currentDay);
        AssignMissions(currentDay);
        UpdateOUTMissions(currentDay);
        UpdateEXECMissions(currentDay);

        if (mode == 1)
        {
            ui.PrintDay(currentDay, this);
            cout << "Press Enter to continue...";
            cin.get(); 
        }

        // Termination Condition
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
    GenerateOutputFile(currentDay);
}


MarsStation::~MarsStation()
{
    
    Rover* r = nullptr;
    
    while (AvailableDiggingRovers.dequeue(r)) { delete r; }
    while (AvailablePolarRovers.dequeue(r))   { delete r; }
    while (AvailableNormalRovers.dequeue(r))  { delete r; }
    while (CheckupDiggingRovers.dequeue(r))   { delete r; }
    while (CheckupPolarRovers.dequeue(r))     { delete r; }
    while (CheckupNormalRovers.dequeue(r))    { delete r; }
    
    // === DELETE ALL MISSIONS ===
    // Missions can be in Ready, OUT, EXEC, BACK, DONE, or Aborted lists
    Mission* m = nullptr;
    int pri = 0;
    
    // Ready lists (LinkedQueue and derived)
    while (ReadyDiggingMissions.dequeue(m)) { delete m; }
    while (ReadyPolarMissions.dequeue(m))   { delete m; }
    while (ReadyNormalMissions.dequeue(m))  { delete m; }
    
    // Priority queue lists (OUT, EXEC, BACK)
    while (OUTMissions.dequeue(m, pri))  { delete m; }
    while (EXECMissions.dequeue(m, pri)) { delete m; }
    while (BACKMissions.dequeue(m, pri)) { delete m; }
    
    // Stack (DONE)
    while (DONEMissions.pop(m)) { delete m; }
    
    // Aborted missions
    while (AbortedMissions.dequeue(m)) { delete m; }
    
    // === DELETE ANY REMAINING REQUESTS ===
    // (Should normally be empty after simulation, but clean up just in case)
    Requests* req = nullptr;
    while (RequestsList.dequeue(req)) { delete req; }
}

LinkedQueue<Rover*>& MarsStation::getCheckupDiggingRovers() { return CheckupDiggingRovers; }
LinkedQueue<Rover*>& MarsStation::getCheckupPolarRovers()   { return CheckupPolarRovers; }
LinkedQueue<Rover*>& MarsStation::getCheckupNormalRovers()  { return CheckupNormalRovers; }
LinkedQueue<Rover*>& MarsStation::getCheckupRescueRovers()  { return CheckupRescueRovers; }
RoverPriQueue& MarsStation::getAvailableDiggingRovers(){ return AvailableDiggingRovers; }
RoverPriQueue& MarsStation::getAvailablePolarRovers() { return AvailablePolarRovers; }
RoverPriQueue& MarsStation::getAvailableNormalRovers(){ return AvailableNormalRovers; }
RoverPriQueue& MarsStation::getAvailableRescueRovers(){ return AvailableRescueRovers; }
LinkedQueue<Requests*>& MarsStation::getRequestsList()      { return RequestsList; }
LinkedQueue<Mission*>&  MarsStation::getReadyDiggingMissions(){ return ReadyDiggingMissions; }
LinkedQueue<Mission*>&  MarsStation::getReadyPolarMissions() { return ReadyPolarMissions; }
RDY_NM&                MarsStation::getReadyNormalMissions(){ return ReadyNormalMissions; }
OUT_missions&          MarsStation::getOUTMissions()        { return OUTMissions; }
priQueue<Mission*>&    MarsStation::getEXECMissions()       { return EXECMissions; }  
priQueue<Mission*>&    MarsStation::getBACKMissions()       { return BACKMissions; }   
LinkedQueue<Mission*>& MarsStation::getAbortedMissions()    { return AbortedMissions; }
LinkedQueue<Mission*>& MarsStation::getFailedMissions()     { return FailedMissions; }
ArrayStack<Mission*>&  MarsStation::getDONEMissions()       { return DONEMissions; }
