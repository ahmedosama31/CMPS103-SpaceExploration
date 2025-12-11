#pragma once
#include "priQueue.h"
#include "Mission.h"
class OUT_missions : public priQueue<Mission*>
{
public:
	Mission* Abortmission(int ID) 
	{
		if (this->isEmpty()) 
			return nullptr;

		priNode<Mission*>* previous = nullptr;
		priNode<Mission*>* current = this->head;
		int pri;
		while (current) {
			Mission* m = current->getItem(pri);
			if (m->getID() == ID) {
				if (current == this->head)
					this->head = current->getNext();
				else
					previous->setNext(current->getNext());

				delete current; // delete node only
				--this->count; // decrement count
				return m;
			}
			previous = current;
			current = current->getNext();
		}
		return nullptr;
	}
};
