#pragma once

#include <iostream>
#include <fstream>
using namespace std;
#include "LinkedQueue.h"
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
    
    //Requests List
    LinkedQueue<Requests*> RequestsList;

    // Mission Lists
    
    LinkedQueue<Mission*> ReadyDiggingMissions;
    LinkedQueue<Mission*> ReadyPolarMissions;
    RDY_NM ReadyNormalMissions;
    OUT_missions OUTMissions;
    LinkedQueue<Mission*> EXECMissions;
    LinkedQueue<Mission*> BACKMissions;
    LinkedQueue<Mission*> AbortedMissions;
    ArrayStack<Mission*> DONEMissions;
   
    
public:
    int D = 0, P = 0, N = 0;
    int SD = 0, SP = 0, SN = 0;
    int M = 0;
    int CD = 0, CP = 0, CN = 0;
    void LoadFromFile(const string& filename)
    {
        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) { return; }

        inputFile >> D >> P >> N;
        inputFile >> SD >> SP >> SN;
        inputFile >> M >> CD >> CP >> CN;
        int id = 1;
        for (int i=0; i < D; i++)
            AvailableDiggingRovers.enqueue(
                new Rover(id++, RoverType::Digging, SD, M, CD));
        for (int i=0; i < P; i++)
            AvailablePolarRovers.enqueue(
                new Rover(id++, RoverType::Polar, SP, M, CP));
        for (int i=0; i < N; i++)
            AvailableNormalRovers.enqueue(
                new Rover(id++, RoverType::Normal, SN, M, CN));

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

    bool EnqueueAvailable(Rover* r)
    {
        if (!r)
            return false;
        switch (r->getType()) {
        case RoverType::Digging:
            AvailableDiggingRovers.enqueue(r);
            break;
        case RoverType::Polar:
            AvailablePolarRovers.enqueue(r);
            break;
        case RoverType::Normal:
            AvailableNormalRovers.enqueue(r);
            break;
        }
        return true;
    }
    bool EnqueueCheckup(Rover* r)
    {
        if (!r)
            return false;
        switch (r->getType()) {
        case RoverType::Digging:
            CheckupDiggingRovers.enqueue(r);
            break;
        case RoverType::Polar:
            CheckupPolarRovers.enqueue(r);
            break;
        case RoverType::Normal:
            CheckupNormalRovers.enqueue(r);
            break;
        }
        return true;
    }




    void InsertMission(Mission* M)
    {
        switch (M->getType()) {
    case MissionType::Digging:
        ReadyDiggingMissions.enqueue(M);
        break;
    case MissionType::Polar:
        ReadyPolarMissions.enqueue(M);
        break;
    case MissionType::Normal:
        ReadyNormalMissions.enqueue(M);
        break;
    }
}
    void AbortMission(int missionID)
    {

    }

    void Simulate()
    {
        int currentDay = 1;
        LoadFromFile("input.txt");

        UI ui;
        const int MAX_DAYS = 200;  

        while (true)
        {
            // Stop when everything is empty 
            // no requests no rdy and no out,exec,back
          

            if (RequestsList.getCount() == 0 && ReadyDiggingMissions.getCount() == 0 &&
                ReadyPolarMissions.getCount() == 0 &&
                ReadyNormalMissions.getCount() == 0 && OUTMissions.getCount() == 0 &&
                EXECMissions.getCount() == 0 &&
                BACKMissions.getCount() == 0)
            {
                ui.PrintDay(currentDay, this);
                break;
            }

            ExecuteRequests(currentDay);

            // 70% move one rover from checkup -> available

            int Y = rand() % 100;
            if (Y < 70)
            {
                Rover* R = nullptr;
                if (CheckupNormalRovers.dequeue(R))
                    EnqueueAvailable(R);
                else if (CheckupDiggingRovers.dequeue(R))
                    EnqueueAvailable(R);
                else if (CheckupPolarRovers.dequeue(R))
                    EnqueueAvailable(R);
            }


            // BACK -> DONE, return rover

            Mission* m = nullptr;
            if (BACKMissions.dequeue(m))
            {
                DONEMissions.push(m);

                Rover* r = m->getAssignedRover();
                if (r)
                {
                    int x = rand() % 100;
                    if (x < 20) EnqueueCheckup(r);
                    else        EnqueueAvailable(r);
                }
            }


            // EXEC -> BACK (2 missions)

            Mission* m1 = nullptr;
            Mission* m2 = nullptr;

            if (EXECMissions.dequeue(m1)) BACKMissions.enqueue(m1);
            if (EXECMissions.dequeue(m2)) BACKMissions.enqueue(m2);


            // OUT -> EXEC (1 mission)

            Mission* outMission = nullptr;
            int dummyPri = 0;
            if (OUTMissions.dequeue(outMission, dummyPri))
                EXECMissions.enqueue(outMission);


            // RDY Polar -> OUT  (ONLY if any rover is available)

            if (AvailablePolarRovers.getCount() > 0 ||
                AvailableNormalRovers.getCount() > 0 ||
                AvailableDiggingRovers.getCount() > 0)
            {
                Mission* PM = nullptr;
                if (ReadyPolarMissions.dequeue(PM))
                {
                    Rover* r = nullptr;
                    if (AvailablePolarRovers.dequeue(r) ||
                        AvailableNormalRovers.dequeue(r) ||
                        AvailableDiggingRovers.dequeue(r))
                    {
                        PM->assignRover(r);
                        OUTMissions.enqueue(PM, 1);
                    }
                    else ReadyPolarMissions.enqueue(PM); // safety
                }
            }


            // RDY Digging -> OUT

            if (AvailableDiggingRovers.getCount() > 0 ||
                AvailablePolarRovers.getCount() > 0 ||
                AvailableNormalRovers.getCount() > 0)
            {
                Mission* DM = nullptr;
                if (ReadyDiggingMissions.dequeue(DM))
                {
                    Rover* r = nullptr;
                    if (AvailablePolarRovers.dequeue(r) ||
                        AvailableNormalRovers.dequeue(r) ||
                        AvailableDiggingRovers.dequeue(r))
                    {
                        DM->assignRover(r);
                        OUTMissions.enqueue(DM, 1);
                    }
                    else ReadyDiggingMissions.enqueue(DM);
                }
            }

            // RDY Normal -> OUT
            if (AvailableNormalRovers.getCount() > 0 ||
                AvailablePolarRovers.getCount() > 0 ||
                AvailableDiggingRovers.getCount() > 0)
            {
                Mission* NM = nullptr;
                if (ReadyNormalMissions.dequeue(NM))
                {
                    Rover* r = nullptr;
                    if (AvailablePolarRovers.dequeue(r) ||
                        AvailableNormalRovers.dequeue(r) ||
                        AvailableDiggingRovers.dequeue(r))
                    {
                        NM->assignRover(r);
                        OUTMissions.enqueue(NM, 1);
                    }
                    else ReadyNormalMissions.enqueue(NM);
                }
            }
            // PRINT
            ui.PrintDay(currentDay, this);

            currentDay++;
            if (currentDay > MAX_DAYS)
            {
                cerr << "Reached MAX_DAYS, stopping simulation (Phase1.2).\n";
                break;
            }
        }
    }
    // Getters for lists (for testing and UI)
    LinkedQueue<Rover*>& getCheckupDiggingRovers() { return CheckupDiggingRovers; }
    LinkedQueue<Rover*>& getCheckupPolarRovers() { return CheckupPolarRovers; }
    LinkedQueue<Rover*>& getCheckupNormalRovers() { return CheckupNormalRovers; }
    LinkedQueue<Rover*>& getAvailableDiggingRovers() { return AvailableDiggingRovers; }
    LinkedQueue<Rover*>& getAvailablePolarRovers() { return AvailablePolarRovers; }
    LinkedQueue<Rover*>& getAvailableNormalRovers() { return AvailableNormalRovers; }
    LinkedQueue<Requests*>& getRequestsList() { return RequestsList; }
    LinkedQueue<Mission*>& getReadyDiggingMissions() { return ReadyDiggingMissions; }
    LinkedQueue<Mission*>& getReadyPolarMissions() { return ReadyPolarMissions; }
    RDY_NM& getReadyNormalMissions() { return ReadyNormalMissions; }
    OUT_missions& getOUTMissions() { return OUTMissions; }
    LinkedQueue<Mission*>& getEXECMissions() { return EXECMissions; }
    LinkedQueue<Mission*>& getBACKMissions() { return BACKMissions; }
    LinkedQueue<Mission*>& getAbortedMissions() { return AbortedMissions; }
    ArrayStack<Mission*>& getDONEMissions() { return DONEMissions; }
};