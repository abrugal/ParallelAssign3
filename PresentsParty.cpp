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


#define MAX_PRESENTS 500000


using namespace std;
using namespace std::chrono;

int addCounter = 0;
int enteredAddCounter = 0;
int removeCounter = 0;

class Node {
public:
	int data;
	int hashKey;
	Node* next;
	bool marked;

private:
	mutex m;

public:
	Node(int data) {
		this->data = data;
		hashKey = data;
		next = NULL;
		marked = false;
	}

public:
	void lock() {
		m.lock();
	}
	void unlock() {
		m.unlock();
	}

public:
	bool add(Node* head, int value) {
		int key = value;
		while (true) {
			Node* pred = head;
			Node* curr = head->next;

			while (curr->hashKey < key) {
				pred = curr;
				curr = curr->next;
			}
			pred->lock();
			try {
				curr->lock();
				try {
					if (validate(pred, curr)) {
						if (curr->hashKey == key) {
							curr->unlock();
							pred->unlock();
							return false;
						}
						else {
							Node* node = new Node(value);
							node->next = curr;
							pred->next = node;
							curr->unlock();
							pred->unlock();
							return true;
						}
					}
				}
				catch (...) {
					cout << "Failed to add: " << value << endl;
				}
				curr->unlock();
			}
			catch (...) {
				cout << "Failed to add: " << value << endl;
			}
			pred->unlock();
		}
	}

public:
	bool remove(Node* head, int value) {
		int key = value;
		while (true) {
			Node* pred = head;
			Node* curr = head->next;

			while (curr->hashKey < key) {
				pred = curr;
				curr = curr->next;
			}

			pred->lock();
			try {
				curr->lock();
				try {
					if (validate(pred, curr)) {
						if (curr->hashKey != key) {
							curr->unlock();
							pred->unlock();
							return false;
						}
						else {
							curr->marked = true;
							pred->next = curr->next;
							curr->unlock();
							pred->unlock();
							return true;
						}
					}
				}
				catch (...) {
					cout << "Failed to remove: " << value << endl;
				}
				curr->unlock();
			}
			catch (...) {
				cout << "Failed to remove: " << value << endl;
			}
			pred->unlock();
			return false;
		}
	}

public:
	bool removeFront(Node* head) {
		while (true) {
			Node* pred = head;
			Node* curr = head->next;

			while (curr->marked) {
				pred = curr;
				curr = curr->next;
			}

			pred->lock();
			try {
				curr->lock();
				try {
					if (validate(pred, curr)) {
						if (curr->hashKey == INT_MAX) {
							curr->unlock();
							pred->unlock();
							return false;
						}
						else {
							curr->marked = true;
							pred->next = curr->next;
							curr->unlock();
							pred->unlock();
							return true;
						}
					}
				}
				catch (...) {
					cout << "Failed to remove front " << endl;
				}
				curr->unlock();
			}
			catch (...) {
				cout << "Failed to remove front " << endl;
			}
			pred->unlock();
		}
	}

public:
	bool contains(Node* head, int value) {
		int key = value;
		Node* curr = head;
		while (curr->hashKey < key)
			curr = curr->next;
		return curr->hashKey == key && !curr->marked;
	}

public:
	bool everContained(Node* head, int value) {
		int key = value;
		Node* curr = head;
		while (curr->hashKey < key)
			curr = curr->next;
		return curr->hashKey == key;
	}

public:
	int size(Node* head, Node* tail) {
		int counter = 0;
		Node* curr = head;
		while (curr->next != NULL) {
			curr = curr->next;
			if (!curr->marked && curr != tail)
				counter++;
		}
		return counter;
	}

public:
	void print(Node* head, Node* tail) {
		Node* curr = head;
		while (curr->next != NULL) {
			curr = curr->next;
			if (!curr->marked && curr != tail)
				cout << curr->data << " ";
		}
		cout << endl;
	}

private:
	bool validate(Node* pred, Node* curr) {
		return !pred->marked && !curr->marked && pred->next == curr;
	}
};


Node* deleteHead(Node* head) {
	if (head == NULL) {
		return NULL;
	}

	if (head->next == NULL) {
		delete head;
		return NULL;
	}

	Node* temp = head;
	head = head->next;
	delete temp;

	return head;
}

Node* head = new Node(INT_MIN);
Node* tail = new Node(INT_MAX);
atomic<int> numOfThankYouCards = 0;
mutex m;
list<int> unorderedBag;
int* presents = new int[MAX_PRESENTS];

void hookPresentToChain(int present) {
	head->add(head, present);
}

void unhookPresentFromChain() {
	if (head->removeFront(head)) {
		numOfThankYouCards++;
	}
}

bool checkIfPresentInChain(int present) {
	return head->contains(head, present);
}

int roll(int min, int max)
{
	// x is in [0,1[
	double x = rand() / static_cast<double>(RAND_MAX + 1);

	// [0,1[ * (max - min) + min is in [min,max[
	int that = min + static_cast<int>(x * (max - min));

	return that;
}

int removeFromBag() {
	m.lock();
	if (unorderedBag.size() == 0) {
		m.unlock();
		return INT_MIN;
	}
	int present = unorderedBag.front();
	unorderedBag.remove(present);
	m.unlock();
	return present;
}

void work(int job) {
	while (numOfThankYouCards < MAX_PRESENTS) {
		if (job == 0) {
			job = 1;
			if (unorderedBag.size() == 0)
				continue;
			int present = removeFromBag();
			if (present == INT_MIN) {
				continue;
			}
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
	head->next = tail;
	srand(time(0));

	unordered_map<int, int> map;


	// Create the unique tags for each present
	for (int i = 0; i < MAX_PRESENTS; i++) {

		int randomNum;

		// Make sure all numbers are unique
		while (true) {
			randomNum = rand() * (RAND_MAX + 1) + rand();
			if (map[randomNum] != randomNum) {
				presents[i] = randomNum;
				map[randomNum] = randomNum;
				break;
			}
		}
	}

	// Put presents in bag
	for (int i = 0; i < MAX_PRESENTS; i++) {
		unorderedBag.push_front(presents[i]);
	}

	cout << "Done initializing" << endl;

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

	delete[] presents;
	cout << diff.count() << " seconds " << endl;
	cout << "Num of thank you cards: " << numOfThankYouCards << endl;
	cout << "Linked list length: " << head->size(head, tail) << endl;
	head->print(head, tail);

	while (head != NULL) {
		head = deleteHead(head);
	}
}



