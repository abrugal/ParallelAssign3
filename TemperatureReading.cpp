

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
#include <set>
#include <random>



using namespace std;

int lowerIndex;
int higherIndex;
mutex m;
unordered_map<int, vector<pair<int, int>>> temperatures;

int getTemperature() {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-100, 70); // define the range

    return distr(gen);
}

void print(int array[]) {
    for (int i = 0; i < 5; i++) {
        cout << array[i] << " " << ends;
    }
    cout << endl;
}


void getBottomFive() {
    int botFive[5] = {100, 100, 100, 100, 100};
    for (int i = 0; i < 8; i++) {
        vector<pair<int, int>> list = temperatures[i];

        for (int i = 0; i < list.size(); i++) {
            for (int j = 0; j < 5; j++) {
                if (list[i].second == botFive[j]) {
                    break;
                }
                if (list[i].second < botFive[j]) {
                    botFive[j] = list[i].second;
                    break;
                }
            }
        }
    }

    print(botFive);
}



void getTopFive() {
    int topFive[5] = { -200, -200, -200, -200, -200 };
    for (int i = 0; i < 8; i++) {
        vector<pair<int, int>> list = temperatures[i];

        for (int i = 0; i < list.size(); i++) {
            for (int j = 0; j < 5; j++) {
                if (list[i].second == topFive[j]) {
                    break;
                }
                if (list[i].second > topFive[j]) {
                    topFive[j] = list[i].second;
                    break;
                }
            }
        }
    }

    print(topFive);
}

void getBiggestDifference(int hour) {
    int biggestDifference = 0;
    int minuteStart;
    int minuteEnd;

    for (int i = 0; i < 8; i++) {
        vector<pair<int, int>> list = temperatures[i];

        for (int j = 0; j < 50; j++) {
            int startTemp = list[j].second;
            int endTemp = list[j + 10].second;

            int difference = abs(endTemp - startTemp);

            if (difference > biggestDifference) {
                biggestDifference = difference;
                minuteStart = list[j].first;
                minuteEnd = list[j + 10].first;
            }
        }
    }

    cout << "Biggest temp difference in hour " << hour << " was from " << minuteStart << " to " << minuteEnd << " minutes with a difference of " << biggestDifference;
}

void makeReport(int hour) {
    cout << "Hour " << hour << ": " << endl;
    getBottomFive();
    getTopFive();
    getBiggestDifference(hour);

    cout << "\n" << endl;
}

void work(int id) {

    for (int hour = 1; hour < 25; hour++) {
        vector<pair<int, int>> idTemperatures(60);
        for (int i = 1; i < 61; i++) {
            int temperature = getTemperature();
            int minute = i;
            pair<int, int> pair(minute, temperature);
            idTemperatures[i - 1] = pair;
        }

        temperatures[id] = idTemperatures;

        if (id == 0) {
            makeReport(hour);
        }
    }
    
}

int main()
{

    srand(time(0));

    thread worker1(work, 0);
    thread worker2(work, 1);
    thread worker3(work, 2);
    thread worker4(work, 3);
    thread worker5(work, 4);
    thread worker6(work, 5);
    thread worker7(work, 6);
    thread worker8(work, 7);

    worker1.join();
    worker2.join();
    worker3.join();
    worker4.join();
    worker5.join();
    worker6.join();
    worker7.join();
    worker8.join();

}
