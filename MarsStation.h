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

    void Simulate();

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







