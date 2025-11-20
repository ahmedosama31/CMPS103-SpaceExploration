#include <iostream>
#include "MarsStation.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "RDY_NM.h"
#include "OUT_Missions.h"
#include "Mission.h"
#include "rover.h"
#include "Enums.h"
using namespace std;

// Test function for LinkedQueue
void testLinkedQueue() {
    cout << "========== Testing LinkedQueue ==========";
    LinkedQueue<Mission*> queue;
    
    // Fill with 10+ missions
    cout << "Enqueue 10 missions"<<endl;
    for (int i = 1; i <= 10; i++) {
        queue.enqueue(new Mission(i, MissionType::Normal, 1000 * i, 10 + i, i));
    }
    
    cout << "Queue count: " << queue.getCount() << endl;
    cout << "Queue contents: ";
    queue.print();
    
    // Dequeue elements one by one
    cout << "Dequeuing elements one by one:" <<endl;
    Mission* m;
    int count = 0;
    while (count< 5 &&queue.dequeue(m)) {
        cout << "Dequeued: " << *m << endl;
        delete m;
        count++;
    }
    
    cout << "Remaining count: " << queue.getCount() << endl;
    
    // Clean up remaining
    while (queue.dequeue(m)) {
        delete m;
    }
    cout << "LinkedQueue test completed." <<endl;
}

// Test function for priQueue
void testPriQueue() {
    cout << "========== Testing priQueue ==========" <<endl;
    priQueue<Mission*> pQueue;
    
    // Fill with 10+ missions with different priorities
    cout << "Enqueuing 12 missions with priorities"<<endl;
    for (int i = 1; i <= 12; i++) {
        pQueue.enqueue(new Mission(i, MissionType::Digging, 1000 * i, 10 + i, i), i * 10);
    }
    
    cout << "Priority Queue count: " << pQueue.getCount() << endl;
    cout << "Priority Queue contents: ";
    pQueue.print();
    
    // Dequeue elements one by one
    cout << "Dequeuing elements one by one:" <<endl;
    Mission* m;
    int pri;
    int count = 0;
    while (count < 5 && pQueue.dequeue(m, pri)) {
        cout << "Dequeued (priority " << pri << "): " << *m << endl;
        delete m;
        count++;
    }
    
    cout << "Remaining count: " << pQueue.getCount() << endl;
    
    // Clean up remaining
    while (pQueue.dequeue(m, pri)) {
        delete m;
    }
    cout << "priQueue test completed."<<endl;
}

// Test function for ArrayStack
void testArrayStack() {
    cout << "========== Testing ArrayStack ==========" <<endl;
    ArrayStack<Mission*> stack;
    
    // Push 10+ missions
    cout << "Pushing 12 missions..." << endl;
    for (int i = 1; i <= 12; i++) {
        stack.push(new Mission(i, MissionType::Polar, 1000 * i, 10 + i, i));
    }
    
    cout << "Stack count: " << stack.getCount() << endl;
    cout << "Stack contents: ";
    stack.print();
    
    // Pop elements one by one
    cout << "Popping elements one by one:" << endl;
    Mission* m;
    int count = 0;
    while (count < 5 && stack.pop(m)) {
        cout << "Popped: " << *m << endl;
        delete m;
        count++;
    }
    
    cout << "Remaining count: " << stack.getCount() << endl;
    
    // Clean up remaining
    while (stack.pop(m)) {
        delete m;
    }
    cout << "ArrayStack test completed." << endl;
}

// Test function for RDY_NM (derived class)
void testRDY_NM() {
    cout << "========== Testing RDY_NM (Derived Class) ==========" << endl;
    RDY_NM rdyNM;
    
    // Enqueue 10+ missions
    cout << "Enqueuing 12 normal missions..." << endl;
    for (int i = 1; i <= 12; i++) {
        rdyNM.enqueue(new Mission(100 + i, MissionType::Normal, 2000 * i, 15 + i, i));
    }
    
    cout << "RDY_NM count: " << rdyNM.getCount() << endl;
    cout << "RDY_NM contents: ";
    rdyNM.print();
    
    // Test Abortmission function
    cout << "Testing Abortmission function..." << endl;
    Mission* aborted = rdyNM.Abortmission(105);
    if (aborted) {
        cout << "Successfully aborted: " << *aborted << endl;
        delete aborted;
    } else {
        cout << "Mission not found." << endl;
    }
    
    cout << "After abort, count: " << rdyNM.getCount() << endl;
    cout << "Contents: ";
    rdyNM.print();
    
    // Clean up
    Mission* m;
    while (rdyNM.dequeue(m)) {
        delete m;
    }
    cout << "RDY_NM test completed." << endl;
}

// Test function for OUT_missions (derived class)
void testOUT_missions() {
    cout << "========== Testing OUT_missions (Derived Class) ==========" << endl;
    OUT_missions outMissions;
    
    // Enqueue 10+ missions with priorities
    cout << "Enqueuing 12 OUT missions with priorities..." << endl;
    for (int i = 1; i <= 12; i++) {
        outMissions.enqueue(new Mission(200 + i, MissionType::Normal, 3000 * i, 20 + i, i), i * 5);
    }
    
    cout << "OUT_missions count: " << outMissions.getCount() << endl;
    cout << "OUT_missions contents: ";
    outMissions.print();
    
    // Test Abortmission function
    cout << "Testing Abortmission function..." << endl;
    Mission* aborted = outMissions.Abortmission(205);
    if (aborted) {
        cout << "Successfully aborted: " << *aborted << endl;
        delete aborted;
    } else {
        cout << "Mission not found." << endl;
    }
    
    cout << "After abort, count: " << outMissions.getCount() << endl;
    cout << "Contents: ";
    outMissions.print();
    
    // Clean up
    Mission* m;
    int pri;
    while (outMissions.dequeue(m, pri)) {
        delete m;
    }
    cout << "OUT_missions test completed." << endl;
}

// Test function for Rover printing
void testRoverPrinting() {
    cout << "========== Testing Rover Printing ==========" << endl;
    LinkedQueue<Rover*> roverQueue;
    
    // Create 10+ rovers
    cout << "Creating 12 rovers..." << endl;
    for (int i = 1; i <= 4; i++) {
        roverQueue.enqueue(new Rover(i, RoverType::Digging, 20.0, 3, 9));
    }
    for (int i = 5; i <= 8; i++) {
        roverQueue.enqueue(new Rover(i, RoverType::Polar, 40.0, 3, 8));
    }
    for (int i = 9; i <= 12; i++) {
        roverQueue.enqueue(new Rover(i, RoverType::Normal, 60.0, 3, 7));
    }
    
    cout << "Rover queue count: " << roverQueue.getCount() << endl;
    cout << "Rover queue contents:" << endl;
    roverQueue.print();
    
    // Clean up
    Rover* r;
    while (roverQueue.dequeue(r)) {
        delete r;
    }
    cout << "Rover printing test completed." << endl;
}

int main()
{
    //cout << "Data Structures Testing: "<< endl;
    //cout << endl;

    //// Test all data structures
    //testLinkedQueue();
    //testPriQueue();
    //testArrayStack();
    //testRDY_NM();
    //testOUT_missions();
    //testRoverPrinting();

    //cout << "====================================================" << endl;
    //cout << "   All tests done!" << endl;
    //cout << "====================================================" << endl;
    //
    MarsStation ms;
    ms.Simulate();
    return 0;
}