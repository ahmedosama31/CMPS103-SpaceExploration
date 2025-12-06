#pragma once

#include <iostream>
#include <fstream>
#include <cmath> // Added for ceil()
using namespace std;

#include "LinkedQueue.h"
#include "priQueue.h"              
#include "RDY_NM.h"
#include "OUT_Missions.h"
#include "rover.h"  
#include "mission.h"    
#include "Requests.h"
#include "newRequest.h"
#include "abortRequest.h"
#include "Enums.h"
#include "ArrayStack.h"
#include "UI.h"

class MarsStation {
private:
    // Rover Lists
    LinkedQueue<Rover*> CheckupDiggingRovers;
    LinkedQueue<Rover*> CheckupPolarRovers;
    LinkedQueue<Rover*> CheckupNormalRovers;
    
    LinkedQueue<Rover*> AvailableDiggingRovers;
    LinkedQueue<Rover*> AvailablePolarRovers; 
    LinkedQueue<Rover*> AvailableNormalRovers;
    
    // Requests List
    LinkedQueue<Requests*> RequestsList;

    // Mission Lists
    LinkedQueue<Mission*> ReadyDiggingMissions;
    LinkedQueue<Mission*> ReadyPolarMissions;
    RDY_NM               ReadyNormalMissions;
    LinkedQueue<Mission*> AbortedMissions;
  
    priQueue<Mission*>   EXECMissions;
    priQueue<Mission*>   BACKMissions;
    OUT_missions         OUTMissions;

    ArrayStack<Mission*>  DONEMissions;
   
public:
    // Simulation Parameters
    int D = 0, P = 0, N = 0;
    int SD = 0, SP = 0, SN = 0;
    int M = 0;
    int CD = 0, CP = 0, CN = 0;

    // =============================================================
    // File Loading (Phase 1.2 - Already Implemented)
    // =============================================================
    void LoadFromFile(const string& filename)
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
        for (int i = 0; i < D; i++)
            AvailableDiggingRovers.enqueue(new Rover(id++, RoverType::Digging, SD, M, CD));
        for (int i = 0; i < P; i++)
            AvailablePolarRovers.enqueue(new Rover(id++, RoverType::Polar, SP, M, CP));
        for (int i = 0; i < N; i++)
            AvailableNormalRovers.enqueue(new Rover(id++, RoverType::Normal, SN, M, CN));

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

    // =============================================================
    // Helper Functions
    // =============================================================

    // Helper: Execute events for the current day
    void ExecuteRequests(int currentDay)
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

    // Helper: Move Rover to Available List
    bool EnqueueAvailable(Rover* r)
    {
        if (!r) return false;
        switch (r->getType()) {
        case RoverType::Digging: AvailableDiggingRovers.enqueue(r); break;
        case RoverType::Polar:   AvailablePolarRovers.enqueue(r);   break;
        case RoverType::Normal:  AvailableNormalRovers.enqueue(r);  break;
        }
        return true;
    }

    // Helper: Move Rover to Checkup List
    bool EnqueueCheckup(Rover* r)
    {
        if (!r) return false;
        switch (r->getType()) {
        case RoverType::Digging: CheckupDiggingRovers.enqueue(r); break;
        case RoverType::Polar:   CheckupPolarRovers.enqueue(r);   break;
        case RoverType::Normal:  CheckupNormalRovers.enqueue(r);  break;
        }
        return true;
    }

    // Helper: Insert new mission into appropriate Ready list
    void InsertMission(Mission* M)
    {
        switch (M->getType()) {
        case MissionType::Digging: ReadyDiggingMissions.enqueue(M); break;
        case MissionType::Polar:   ReadyPolarMissions.enqueue(M);   break;
        case MissionType::Normal:  ReadyNormalMissions.enqueue(M);  break;
        }
    }

    // Placeholder: Abort Mission (Member 3)
    void AbortMission(int missionID)
    {
        // To be implemented by Member 3
    }

    // =============================================================
    // MEMBER 1 TASKS: Mission Lifecycle Management
    // =============================================================
    
    // Move missions from OUT -> EXEC
    void UpdateOUTMissions(int currentDay)
    {
        Mission* m = nullptr;
        int pri = 0;

        while (OUTMissions.peek(m, pri))
        {
            
            
            int arrivalDay = -pri;

            
            if (arrivalDay <= currentDay) //it arrived today or a day before
            {
                
                OUTMissions.dequeue(m, pri);

                int executionDuration = m->getMissionDuration();

                
                // Fday = Today + Duration
                int Fday = currentDay + executionDuration;
                
                
                m->setExecutionDays(executionDuration); 

                // Move to EXEC List
                // exec-> back will move based on who finished exec first, so priority is Fday.
                // earliest fday comes out first so pri is -fday.
                EXECMissions.enqueue(m, -Fday);
            }
            else
            {
                // The mission at the front hasn't arrived yet.
                break;
            }
        }
    }

    // Move missions from EXEC -> BACK
    void UpdateEXECMissions(int currentDay)
    {
        Mission* m = nullptr;
        int pri = 0;

        while (EXECMissions.peek(m, pri))
        {

            int Fday = -pri;


            if (Fday <= currentDay) 
            {

                EXECMissions.dequeue(m, pri);
                double speed = m->getAssignedRover()->getSpeed();
                double targetloc = m->getTargetLocation();
                
                // Fixed: Distance / Speed = Time
                double travelHours = targetloc / speed;
                int travelDays = ceil(travelHours / 25.0); 

                int returnDay = currentDay + travelDays;

                BACKMissions.enqueue(m, -returnDay);
            }
            else
            {
                // The mission at the front hasn't arrived yet.
                break;
            }
        }
    }

    // Move missions from BACK -> DONE
    void UpdateBACKMissions(int currentDay)
    {
        Mission* m = nullptr;
        int pri = 0;

        while (BACKMissions.peek(m, pri))
        {
            int returnDay = -pri;
            if (returnDay <= currentDay)
            {
                BACKMissions.dequeue(m, pri);
                DONEMissions.push(m);
                // ------------------------- Rover should be set to available ------------------
            }
            else
            {
                break;
            }
        }


    }

    // =============================================================
    // MEMBER 2 TASKS: Rover Logic & Assignment
    // =============================================================

    void AssignMissions(int currentDay)
    {
        // Placeholder for Member 2
        // Logic: Move missions from RDY -> OUT by assigning available rovers
    }

    void ManageCheckups(int currentDay)
    {
        // Placeholder for Member 2/3
        // Logic: Move rovers from Checkup -> Available when done
    }
    
    // Called when a mission is DONE to decide rover fate
    void ReleaseRover(Rover* r)
    {
        // Placeholder for Member 2
        // Logic: Check missions count vs M. Move to Checkup or Available.
        // For now, let's just put it back to available so simulation doesn't stall.
        if(r) EnqueueAvailable(r); 
    }

    // =============================================================
    // MAIN SIMULATION LOOP (Member 4 + Member 1 Interface logic)
    // =============================================================

    void Simulate()
    {
        UI ui;
        
        // Prompt for Mode
        int mode;
        cout << "Select Mode (1: Interactive, 2: Silent): ";
        cin >> mode;
        
        // Initialize
        int currentDay = 1;
        LoadFromFile("input.txt"); 

        cout << "Simulation Starts..." << endl;

        while (true)
        {
            // 1. Execute new requests (Phase 1)
            ExecuteRequests(currentDay);

            // 2. Assign Missions (RDY -> OUT) - Member 2
            AssignMissions(currentDay);

            // 3. Update Mission States (OUT->EXEC->BACK->DONE) - Member 1
            UpdateOUTMissions(currentDay);
            UpdateEXECMissions(currentDay);
            UpdateBACKMissions(currentDay);

            // 4. Manage Rovers (Checkups) - Member 2/3
            ManageCheckups(currentDay);

            // 5. Output / Interface - Member 1
            if (mode == 1)
            {
                ui.PrintDay(currentDay, this);
                cout << "Press Enter to continue...";
                cin.ignore(); cin.get(); // Simple pause
            }

            // 6. Termination Condition
            // Break if all missions are DONE (and all other lists empty)
            // Simple check for now:
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

    // =============================================================
    // Getters (Existing)
    // =============================================================
    LinkedQueue<Rover*>& getCheckupDiggingRovers() { return CheckupDiggingRovers; }
    LinkedQueue<Rover*>& getCheckupPolarRovers()   { return CheckupPolarRovers; }
    LinkedQueue<Rover*>& getCheckupNormalRovers()  { return CheckupNormalRovers; }
    LinkedQueue<Rover*>& getAvailableDiggingRovers(){ return AvailableDiggingRovers; }
    LinkedQueue<Rover*>& getAvailablePolarRovers() { return AvailablePolarRovers; }
    LinkedQueue<Rover*>& getAvailableNormalRovers(){ return AvailableNormalRovers; }
    LinkedQueue<Requests*>& getRequestsList()      { return RequestsList; }
    LinkedQueue<Mission*>&  getReadyDiggingMissions(){ return ReadyDiggingMissions; }
    LinkedQueue<Mission*>&  getReadyPolarMissions() { return ReadyPolarMissions; }
    RDY_NM&                getReadyNormalMissions(){ return ReadyNormalMissions; }
    OUT_missions&          getOUTMissions()        { return OUTMissions; }
    priQueue<Mission*>&    getEXECMissions()       { return EXECMissions; }  
    priQueue<Mission*>&    getBACKMissions()       { return BACKMissions; }   
    LinkedQueue<Mission*>& getAbortedMissions()    { return AbortedMissions; }
    ArrayStack<Mission*>&  getDONEMissions()       { return DONEMissions; }
};
