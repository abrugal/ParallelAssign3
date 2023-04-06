

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

using namespace std; 

list<int> topFive(5, -200);
list<int> botFive(5, 100);

int getTemperature() {
    int max = 70;
    int min = -100;
    int range = max - min + 1;
    return rand() % range + min;
}

void checkIfBotFive(int temp) {
    for (list<int>::iterator i = botFive.begin(); i != botFive.end(); ++i) {
        if (temp < *i) {
            botFive.insert(i, temp);
            if (botFive.size() == 6) {
                botFive.pop_back();
            }
            return;
        }
    }
}

void checkIfTopFive(int temp) {
    for (list<int>::iterator i = topFive.begin(); i != topFive.end(); ++i) {
        if (temp > *i) {
            topFive.insert(i, temp);
            if (topFive.size() == 6) {
                topFive.pop_back();
            }
            return;
        }
    }
}

void print(list<int> omg)
{


    for (int num: omg) {
        cout << num << ends;
    }
}

int main()
{

    srand(time(0));
    print(topFive);
    print(botFive);
    for (int i = 0; i < 30; i++) {
        int temp = getTemperature();
        cout << temp << endl;
        checkIfBotFive(temp);
        checkIfTopFive(temp);
    }
    print(topFive);
    print(botFive);
}
