#pragma once
#include "priNode.h"
#include <iostream>     


//This class implements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
protected:
    priNode<T>* head;
    int count; 
public:
    priQueue() : head(nullptr), count(0) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp, p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {

            newNode->setNext(head);
            head = newNode;
            ++count;
            return;
        }

        priNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
        ++count;
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        --count;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int getCount() const;  

    void print(int currentDay = -1) const;      
};

template <typename T>
int priQueue<T>::getCount() const { return count; }

template <typename T>
void priQueue<T>::print(int currentDay) const {
    priNode<T>* cur = head;
    while (cur) {
        int p;
        T item = cur->getItem(p);
        
        if (currentDay != -1) {
             // Priority is negative of the target day (Arrival, Finish, or Return)
             int targetDay = -p;
             int daysLeft = targetDay - currentDay;
             if (daysLeft < 0) daysLeft = 0; 
             std::cout << "[" << *item << ", " << daysLeft << "days]";
        } else {
             std::cout << "[" << *item << "]";
        }

        if (cur->getNext()) std::cout << ", ";
        cur = cur->getNext();
    }
}
