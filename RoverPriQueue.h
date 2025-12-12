#pragma once
#include "priQueue.h"
#include "rover.h"
#include <iostream>

class RoverPriQueue : public priQueue<Rover*> {
public:
    // Override print to match UI requirement: "1, 2, 3" (no brackets, no newlines)
    void print() const {
        priNode<Rover*>* cur = head;
        while (cur) {
            int p;
            Rover* r = cur->getItem(p);
            std::cout << r->getID();
            
            if (cur->getNext()) {
                std::cout << ", ";
            }
            cur = cur->getNext();
        }
    }

    // Helper to calculate priority
    // Formula: (Speed * 100) - TotalDistance
    // Higher speed = Higher Priority
    // Higher distance = Lower Priority
    static int calculatePriority(Rover* r) {
        if (!r) return 0;
        return (int)(r->getSpeed() * 100 - r->getTotalDistance());
    }
};
