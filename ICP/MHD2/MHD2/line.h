#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>
#include <utility>
#include <Street.h>
#include "bus.h"
#include "traffic_t.h"

using namespace std;
class line
{
    string Id;
    vector<Street*>* streets = nullptr;
    vector<Stop*>* Stops = nullptr;
    vector<bus*>* buses = nullptr;
    vector<traffic_t*>* traff = nullptr;


public:
    line(string s);
    bool addStop(Stop* stop);
    bool addStreet(Street* s);
    vector<Street*>* getRoute();
    vector<Stop*>* getStops();
    vector<traffic_t*>* getTraf();
    bool addBus(bus* t);
    bool addTraf(traffic_t* t);

};

#endif // LINE_H
