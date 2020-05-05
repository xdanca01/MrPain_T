#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>
#include <utility>
#include <Street.h>
#include "bus.h"
#include "traffic_t.h"
#include <QDebug>

using namespace std;
class line
{
    string Id;
    vector<Street*> streets;
    vector<Stop*> Stops;
    vector<bus*> buses;
    vector<traffic_t*> traff;
    vector<int> stops_on;


public:
    line(string s);
    bool addStop(Stop* stop,int on);
    bool addStop(Stop* stop);
    bool addStreet(Street* s);
    string getId();
    vector<Street*> getRoute();
    vector<Stop*> getStops();
    vector<traffic_t*> getTraf();
    bool addBus(bus* t);
    bool addTraff(traffic_t* t);
    int stop_on(int y);

};

#endif // LINE_H
