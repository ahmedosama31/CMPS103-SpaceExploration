#pragma once
#include "LinkedQueue.h"
#include "Mission.h"

class RDY_NM : public LinkedQueue<Mission*>
{
public:
	Mission* Abortmission(int ID) 
	{       
        if (this->isEmpty()) return nullptr;

        Node<Mission*>* previous = nullptr;
        Node<Mission*>* current = this->frontPtr;

        while (current != nullptr) {
            if (current->getItem()->getID() == ID) {
                Mission* tobeAborted = current->getItem();

                // Remove node
                if (current == this->frontPtr)
                    this->frontPtr = current->getNext();
                else
                    previous->setNext(current->getNext());

                // If last node was removed, update rear
                if (current == this->backPtr)
                    this->backPtr = previous;

                delete current;
                return tobeAborted;
            }
            previous = current;
            current = current->getNext();
        }
        return nullptr;  // not found
	}
};