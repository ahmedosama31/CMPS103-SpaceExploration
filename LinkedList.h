#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T> *Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll(); 
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{		
		cout<<"\nprinting list contents:\n\n";
		Node<T> *p = Head;

		while(p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	* 
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T &data)
	{
		Node<T> *R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T> *P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}

	void InsertEnd(const T &data)
	{
		Node<T>* newNode = new Node<T>(data);
		if(!Head)
		{
			Head = newNode;
			return;
		}
		Node<T>* p = Head;
		while(p->getNext())
			p = p->getNext();
		p->setNext(newNode);
	}

	// DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	//Needed for AbortMission
	bool DeleteNode(const T &value)
	{
		if(!Head) return false; // empty list
		// if head matches
		if(Head->getItem() == value)
		{
			Node<T>* toDel = Head;
			Head = Head->getNext();
			delete toDel;
			return true;
		}
		// traverse keeping previous
		Node<T>* prev = Head;
		Node<T>* curr = Head->getNext();
		while(curr)
		{
			if(curr->getItem() == value)
			{
				prev->setNext(curr->getNext());
				delete curr;
				return true;
			}
			prev = curr;
			curr = curr->getNext();
		}
		return false; // not found
	}
	bool DeleteFirst(T& out)  //Needed for Dequeue in LinkedQueue implementation
	{
		if (!Head) return false;
		Node<T>* n = Head;
		out = n->getItem();
		Head = Head->getNext();
		delete n;
		return true;
	}

	bool IsEmpty()
	{
		return Head == nullptr;
	}

	/*
	// ************************************
	//    LAB CODE(not needed)
	// ************************************

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	//bool Find(int Key);

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	//int CountOccurance(const T &value);


	//[5] DeleteLast
	//Deletes the last node in the list
	//void DeleteLast();

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	//bool DeleteNodes(const T &value);	

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	//void Merge(const LinkedList& L);

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	//void Reverse();
	*/
	
};

#endif
