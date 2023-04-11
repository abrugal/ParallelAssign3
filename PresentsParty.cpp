// PresentsParty.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>
#include <functional>
#include <mutex>
#include <chrono>         // std::chrono::seconds
#include <queue>
#include <cstdlib>
#include <unordered_map>
#include <list>
#include <atomic>
#include <unordered_set>
#include "LinkedList.h"

#define MAX_PRESENTS 500000


using namespace std;
using namespace std::chrono;

LinkedList* linkedList = new LinkedList();
atomic<int> numOfThankYouCards = 0;
mutex m;
list<int> unorderedBag;
vector<int> presents(MAX_PRESENTS);
atomic<int> addPos{ 0 };

void hookPresentToChain(int present) {
	linkedList->add(present);
}

bool unhookPresentFromChain() {
	if (linkedList->removeFront()) {
		numOfThankYouCards++;
		return true;
	}
	return false;
}

bool checkIfPresentInChain(int present) {
	return linkedList->contains(present);
}

int roll(int min, int max)
{
	double x = rand() / static_cast<double>(RAND_MAX + 1);

	int that = min + static_cast<int>(x * (max - min));

	return that;
}

void work(int job) {
	while (numOfThankYouCards < MAX_PRESENTS) {
		if (job == 0) {
			job = 1;
			if (addPos == MAX_PRESENTS) continue;
			int present = presents[addPos++];
			hookPresentToChain(present);			
		}
		else if (job == 1) {
			job = 2;
			unhookPresentFromChain();
		}
		else {
			job = 0;
			int randomPresent = presents[roll(0, MAX_PRESENTS)];
			checkIfPresentInChain(randomPresent);
		}
	}
}

int main()
{
	srand(time(0));

	// Create the unique tags for each present
	for (int i = 0; i < MAX_PRESENTS; i++) {
		presents[i] = i + 1;
	}

	// shuffle the presents
	for (int i = MAX_PRESENTS - 1; i >= 0; i--)
	{
		int j = rand() % (i + 1);
		swap(presents[i], presents[j]);
	}

	auto start = high_resolution_clock::now();


	thread worker1(work, 0);
	thread worker2(work, 1);
	thread worker3(work, 2);
	thread worker4(work, 0);

	worker1.join();
	worker2.join();
	worker3.join();
	worker4.join();

	auto stop = high_resolution_clock::now();
	duration<double> diff = stop - start;


	cout << diff.count() << " seconds with " << MAX_PRESENTS << " presents" << endl;
	cout << "Num of thank you cards: " << numOfThankYouCards << endl;
	cout << "Linked list length: " << linkedList->size() << endl;
	linkedList->print();

	delete linkedList;
}



