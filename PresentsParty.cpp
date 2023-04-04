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


using namespace std; 

/* Link list node */
struct Node {
    int data;
    struct Node* next;
};

//delete first node in the linked list
Node* deleteFirstNode(struct Node* head)
{
    if (head == NULL)
        return NULL;

    // Move the head pointer to the next node
    Node* tempNode = head;
    head = head->next;
    delete tempNode;

    return head;
}

//delete last node from linked list
Node* removeLastNode(struct Node* head)
{
    if (head == NULL)
        return NULL;

    if (head->next == NULL) {
        delete head;
        return NULL;
    }

    // first find second last node
    Node* second_last = head;
    while (second_last->next->next != NULL)
        second_last = second_last->next;

    // Delete the last node
    delete (second_last->next);

    // set next of second_last to null
    second_last->next = NULL;

    return head;
}

// create linked list by adding nodes at head
void push(struct Node** head, int new_data)
{
    struct Node* newNode = new Node;
    newNode->data = new_data;
    newNode->next = (*head);
    (*head) = newNode;
}



Node* head = NULL;

void hookPresentToChain(int present) {
    if (head == NULL || head->data > present) {
        push(&head, present);
    }
    else {
        Node* previousNode = head;

        while (previousNode->next != NULL) {
            int previousData = previousNode->data;
            int followingData = previousNode->next->data;

            if (previousData < present && present < followingData) {
                Node* tempNode = previousNode->next;
                Node* newNode = new Node;
                newNode->data = present;
                newNode->next = tempNode;

                previousNode->next = newNode;

                return;
            }

            previousNode = previousNode->next;
        }

        Node* newNode = new Node;
        newNode->data = present;
        newNode->next = NULL;
        previousNode->next = newNode;
    }
}

bool unhookPresentFromChain(int present) {
    if (head == NULL) {
        return false;
    }
    else if (head->data == present) {
        head = deleteFirstNode(head);
        return true;
    }
    else {
        Node* currentNode = head;

        while (currentNode->next != NULL) {
            int nextData = currentNode->next->data;

            if (nextData == present) {
                Node* tempNode = currentNode->next;
                currentNode->next = tempNode->next;

                delete tempNode;

                return true;
            }

            currentNode = currentNode->next;
        }

        return false;
    }
}

bool checkIfPresentInChain(int present) {
    if (head == NULL) {
        return false;
    }
    else if (head->data == present) {
        return true;
    }
    else {
        Node* currentNode = head;

        while (currentNode->next != NULL) {
            int nextData = currentNode->next->data;

            if (nextData == present) {
                return true;
            }

            currentNode = currentNode->next;
        }
        return false;
    }
}

int main()
{
    srand(time(0));

    int* presents = new int[500];
    unordered_map<int, int> map;
    int numOfThankYouCards = 0;



    // Create the unique tags for each present
    for (int i = 0; i < 500; i++) {

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


    for (int i = 0; i < 500; i++) {
        hookPresentToChain(presents[i]);
    }


    delete[] presents;
    
    // Delete linked list
    while (head != NULL) {
        head = deleteFirstNode(head);
    }
}



