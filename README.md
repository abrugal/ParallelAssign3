# ParallelAssign3

## Problem 1 Approach
I used the lazy linked list implementation from the book.

## Problem 2 Approach
Each thread measures the temperature and this data is stored in an unordered_map where the key is the id of the thread and the value is a list of pairs of the minute and temperature. Each thread sleeps for 10 ms after measuring the temperature to simulate a minute passing. Finally there is one specific thread that will make a report after every hour, which will include the bottom five and top five temperatures, and the biggest difference of temperature in a 10 minute interval.

## How to run the programs

PresentsParty.cpp and LinkedList.h is problem 1 and TemperatureReading.cpp is problem 2.

To run the programs all you have to do is download the files, and then in the terminal run the command:

To compile: g++ \<header file>\ \<source file\> -o \<you name this file\>

To execute: ./\<name you gave\>
