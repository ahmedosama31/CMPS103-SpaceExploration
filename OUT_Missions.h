#pragma once
#include "priQueue.h"
#include "mission.h"
class OUT_missions : public priQueue<mission*>
{
public:
	mission* Abortmission(int ID) 
	{
		if (this->isEmpty()) 
			return nullptr;


		priNode<mission*>* previous = nullptr;
		priNode<mission*>* current = this->head;


		while (current) {
			int pri;
			mission* m = current->getItem(pri);
			if (m->getID() == ID) {
				if (current == this->head)
					this->head = current->getNext();
				else
					previous->setNext(current->getNext());


				delete current; // delete node only
				return m;
			}
			previous = current;
			current = current->getNext();
		}
		return nullptr;
	}
};
