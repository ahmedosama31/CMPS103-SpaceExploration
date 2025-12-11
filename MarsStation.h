#pragma once
using namespace std;

#include <iostream>
#include <fstream>
#include <cmath>
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
    LinkedQueue<Rover*> AvailableDiggingRovers; 
    LinkedQueue<Rover*> AvailablePolarRovers;   
    LinkedQueue<Rover*> AvailableNormalRovers;  
    LinkedQueue<Rover*> CheckupDiggingRovers;   
    LinkedQueue<Rover*> CheckupPolarRovers;     
    LinkedQueue<Rover*> CheckupNormalRovers;    
    
    // Mission Lists
    LinkedQueue<Mission*> ReadyDiggingMissions; 
    LinkedQueue<Mission*> ReadyPolarMissions;   
    RDY_NM                ReadyNormalMissions;    
    OUT_missions          OUTMissions;        
    priQueue<Mission*>    EXECMissions;       
    priQueue<Mission*>    BACKMissions;       
    ArrayStack<Mission*>  DONEMissions;       
    LinkedQueue<Mission*> AbortedMissions;    
  
    // Request List
    LinkedQueue<Requests*> RequestsList;

public:
    // Simulation Parameters
    int D = 0, P = 0, N = 0; 
    int SD = 0, SP = 0, SN = 0; 
    int M = 0; 
    int CD = 0, CP = 0, CN = 0; 

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


    // Helper functions
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

    // Request handling
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
    
    void AbortMission(int missionID)
    {
        // To be implemented by Member 3
    }

    // when rover completes mission, check if it needs checkup and if not move it to available
    void ReleaseRover(Rover* r, int currentDay)
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

    void ManageCheckups(int currentDay)
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

        

    // Member 1 Task: Auto Abort Polar Missions
    void AutoAbortPolarMissions(int currentDay)
    {
        Mission* m = nullptr;
        while (ReadyPolarMissions.peek(m))
        {
            if (currentDay - m->getRequestedDay() > 2 * m->getMissionDuration())
            {
                ReadyPolarMissions.dequeue(m);
                AbortedMissions.enqueue(m);
            }
            else 
            {
                break;
            }
        }
    }

        

            // Mission assignment (RDY -> OUT)

            void AssignMissions(int currentDay)        {
            Mission* m = nullptr;
            Rover* r = nullptr;
    
            // POLAR Missions
            while (ReadyPolarMissions.peek(m))
            {
                r = nullptr;
                if (AvailablePolarRovers.dequeue(r))
                {
                    r->assignMission(m); 
                }
                else if (AvailableNormalRovers.dequeue(r))
                {
                    r->assignMission(m); 
                }
                else if (AvailableDiggingRovers.dequeue(r))
                {
                    r->assignMission(m); 
                }
                
                if (r != nullptr)
                {
                    ReadyPolarMissions.dequeue(m);
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
                if (AvailableDiggingRovers.dequeue(r))
                {
                    r->assignMission(m); 
                    ReadyDiggingMissions.dequeue(m);
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
                if (AvailableNormalRovers.dequeue(r))
                {
                    r->assignMission(m); 
                }
                else if (AvailablePolarRovers.dequeue(r))
                {
                    r->assignMission(m); 
                }
    
                if (r != nullptr)
                {
                    ReadyNormalMissions.dequeue(m);
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
 
    
    // Move missions from OUT -> EXEC
    void UpdateOUTMissions(int currentDay)
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
                
                double travelHours = targetloc / speed;
                int travelDays =ceil(travelHours / 25.0); 
                int returnDay = currentDay + travelDays;

                BACKMissions.enqueue(m, -returnDay);
            }
            else
                break;
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
                m->setCompletionDay(currentDay); 
                ReleaseRover(m->getAssignedRover(), currentDay);
            }
            else
                break;
        }
    }

    void Simulate()
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