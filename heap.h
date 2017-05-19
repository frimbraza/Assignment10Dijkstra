#ifndef HEAP
#define HEAP

#include <iostream>

using namespace std;

template<typename T, typename C>
class Heap
{
private:
	T** HeapTree;
	int capacity;
	int lastNodeLocation;
	void upHeap(int nodeLocation);
	void downHeap(int nodeLocation);
	void addLevel();

public:
	Heap();
	~Heap();
	void insert(T value);
	T remove();
	bool isEmpty() { return !HeapTree[0]; }
	void printPreOrder(int start = 0);
	void printHeapArray();
};

#endif // !HEAP

template<typename T, typename C>
void Heap<T, C>::upHeap(int nodeLoc)
{
	C hiPri;								// Comparator, if first > second -> true, False otherwise
	T thisValue = *HeapTree[nodeLoc];
	int parentLoc = (nodeLoc - 1) / 2;		// Find your parent T.T *Note can be negative, but won't matter
	T parentValue = *HeapTree[parentLoc];
	if (nodeLoc == 0)						// Reached end -> return
		return;
	if (!hiPri(thisValue, parentValue))		// Correct place ->return
		return;

	T* tempNodePtr = HeapTree[parentLoc];
	HeapTree[parentLoc] = HeapTree[nodeLoc];		// Swap them
	HeapTree[nodeLoc] = tempNodePtr;

	upHeap(parentLoc);						// Call recursively
}

template<typename T, typename C>
void Heap<T, C>::downHeap(int nodeLoc)
{
	if ((2 * nodeLoc + 1) >= lastNodeLocation)		// you reached a leaf so return
		return;
	C hiPri;
	T thisValue = *HeapTree[nodeLoc];
	T leftChild = *HeapTree[2 * nodeLoc + 1];
	if ((2 * nodeLoc + 2) >= lastNodeLocation)			// If only the left child exists
	{
		if (hiPri(leftChild, thisValue))				// Compare the left and this value only
		{
			T* tempNodePtr = HeapTree[nodeLoc];
			HeapTree[nodeLoc] = HeapTree[2 * nodeLoc + 1];
			HeapTree[2 * nodeLoc + 1] = tempNodePtr;
			downHeap(2 * nodeLoc + 1);
		}
	}
	else
	{
		T rightChild = *HeapTree[2 * nodeLoc + 2];			// Both children exists
		if (hiPri(leftChild, rightChild))					// Compare left & right
		{
			if (hiPri(leftChild, thisValue))				// if left was bigger, compare left and this
			{
				T* tempNodePtr = HeapTree[nodeLoc];
				HeapTree[nodeLoc] = HeapTree[2 * nodeLoc + 1];
				HeapTree[2 * nodeLoc + 1] = tempNodePtr;
				downHeap(2 * nodeLoc + 1);
			}
		}
		else
		{
			if (hiPri(rightChild, thisValue))				// if right was bigger than left, compare right and this
			{
				T* tempNodePtr = HeapTree[nodeLoc];
				HeapTree[nodeLoc] = HeapTree[2 * nodeLoc + 2];
				HeapTree[2 * nodeLoc + 2] = tempNodePtr;
				downHeap(2 * nodeLoc + 2);
			}
		}
	}
}

template<typename T, typename C>
void Heap<T, C>::addLevel()
{
	int newCapacity = 2 * capacity + 1;
	T** newHeapTree = new T*[newCapacity];
	for (int i = 0; i < newCapacity; ++i)
	{
		if (i < capacity)
			newHeapTree[i] = this->HeapTree[i];
		else
			newHeapTree[i] = 0;
	}
	capacity = newCapacity;
	delete[] HeapTree;
	this->HeapTree = newHeapTree;
}

template<typename T, typename C>
Heap<T, C>::Heap()
{
	capacity = 31; // Note: level 0 = 1 elements, 1 = 3, 2 = 7, 3 = 15, 4 = 31 
	HeapTree = new T*[capacity];
	for (int i = 0; i < capacity; ++i)
	{
		HeapTree[i] = 0;
	}
	lastNodeLocation = 0;
}

template<typename T, typename C>
Heap<T, C>::~Heap()
{
	while (!isEmpty())
	{
		remove();
	}
}

template<typename T, typename C>
void Heap<T, C>::insert(T value)
{
	if (lastNodeLocation == capacity)
		addLevel();
	T* nodePtr = new T; // Trying to make a new ptr to the value(whatever it may be). Is this the right syntax?
	*nodePtr = value;	// Now I set the dereference ptr equal to the value.
	HeapTree[lastNodeLocation] = nodePtr;
	upHeap(lastNodeLocation);
	lastNodeLocation++;
}

template<typename T, typename C>
T Heap<T, C>::remove()
{
	T top;
	if (!isEmpty())
	{
		top = *HeapTree[0];
		T tempValue = *HeapTree[0];
		delete HeapTree[0];
		HeapTree[0] = HeapTree[lastNodeLocation - 1];	// set root to Last filled Node
		HeapTree[lastNodeLocation - 1] = 0;
		lastNodeLocation--;
		downHeap(0);

	}
	return top;
}

template<typename T, typename C>
void Heap<T, C>::printPreOrder(int start)
{
	if (start >= lastNodeLocation)
		return;
	cout << *HeapTree[start] << " ";
	printPreOrder(2 * start + 1);
	printPreOrder(2 * start + 2);
}

template<typename T, typename C>
void Heap<T, C>::printHeapArray()
{
	for (int i = 0; i < lastNodeLocation; ++i)
	{
		cout << *HeapTree[i] << " ";
	}
}


