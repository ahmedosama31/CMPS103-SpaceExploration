#pragma once
#include <string>
#include "LinkedQueue.h"
#include "priQueue.h"              
#include "RDY_NM.h"
#include "OUT_Missions.h"
#include "rover.h"  
#include "mission.h"    
#include "Requests.h"
#include "ArrayStack.h"
#include "RoverPriQueue.h"

class MarsStation {
private:
    // Rover Lists
    RoverPriQueue AvailableDiggingRovers; 
    RoverPriQueue AvailablePolarRovers;   
    RoverPriQueue AvailableNormalRovers;  
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
    LinkedQueue<Mission*> FailedMissions;
  
    // Request List
    LinkedQueue<Requests*> RequestsList;

public:
    // Simulation Parameters
    int D = 0, P = 0, N = 0; 
    int SD = 0, SP = 0, SN = 0; 
    int M = 0; 
    int CD = 0, CP = 0, CN = 0; 

    void LoadFromFile(const std::string& filename);

    // Helper functions
    bool EnqueueAvailable(Rover* r);
    bool EnqueueCheckup(Rover* r);
    void InsertMission(Mission* M);
    bool AbortMission(int missionID, int currentDay);

    // Request handling
    void ExecuteRequests(int currentDay);
    
    // Rover maintenance
    void ReleaseRover(Rover* r, int currentDay);
    void ManageCheckups(int currentDay);

    // Auto Abort Polar Missions waiting too long
    void AutoAbortPolarMissions(int currentDay);

    // Mission assignment
    void AssignMissions(int currentDay);        
    void HandleRescueMissions(int currentDay);
 
    // Move missions lists
    void UpdateOUTMissions(int currentDay);
    void UpdateEXECMissions(int currentDay);
    void UpdateBACKMissions(int currentDay);

    void Simulate();

    // Getters
    LinkedQueue<Rover*>& getCheckupDiggingRovers();
    LinkedQueue<Rover*>& getCheckupPolarRovers();
    LinkedQueue<Rover*>& getCheckupNormalRovers();
    RoverPriQueue& getAvailableDiggingRovers();
    RoverPriQueue& getAvailablePolarRovers();
    RoverPriQueue& getAvailableNormalRovers();
    LinkedQueue<Requests*>& getRequestsList();
    LinkedQueue<Mission*>&  getReadyDiggingMissions();
    LinkedQueue<Mission*>&  getReadyPolarMissions();
    RDY_NM&                getReadyNormalMissions();
    OUT_missions&          getOUTMissions();
    priQueue<Mission*>&    getEXECMissions();  
    priQueue<Mission*>&    getBACKMissions();   
    LinkedQueue<Mission*>& getAbortedMissions();
    LinkedQueue<Mission*>& getFailedMissions();
    ArrayStack<Mission*>&  getDONEMissions();
};
