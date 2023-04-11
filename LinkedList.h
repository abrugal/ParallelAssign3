#include <iostream>
#include <mutex>
#include <unordered_set>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

struct Node {
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
	void lock() {
		m.lock();
	}
	void unlock() {
		m.unlock();
	}
};

class LinkedList {

private:
	Node* head;
	Node* tail;

public:
	LinkedList() {
		head = new Node(INT_MIN);
		tail = new Node(INT_MAX);
		head->next = tail;
	}

	~LinkedList()
	{
		while (head->next != NULL) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
		delete head;
	}
	
	bool add(int value) {
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

	bool remove(int value) {
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

	bool removeFront() {
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

	bool contains(int value) {
		int key = value;
		Node* curr = head;
		while (curr->hashKey < key)
			curr = curr->next;
		return curr->hashKey == key && !curr->marked;
	}

	bool everContained(int value) {
		int key = value;
		Node* curr = head;
		while (curr->hashKey < key)
			curr = curr->next;
		return curr->hashKey == key;
	}

	int size() {
		int counter = 0;
		Node* curr = head;
		while (curr->next != NULL) {
			curr = curr->next;
			if (!curr->marked && curr != tail)
				counter++;
		}
		return counter;
	}

	void print() {
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
