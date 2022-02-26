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
    //jméno linky
    string Id;
    //ulice linky (s duplikáty)
    vector<Street*> streets;
    //stopky linky (s duplikáty)
    vector<Stop*> Stops;
    //autobusy na lince
    vector<bus*> buses;
    //jizdní řády linky
    vector<traffic_t*> traff;
    //na které ulici stopka leží
    vector<int> stops_on;


public:
    line(string s);
    //add stop only without streets
    bool addStop(Stop* stop, int on);
    //add streets only without stops
    bool addStreet(Street* s);
    string getId();
    //vrátí ulice linky
    vector<Street*> getRoute();
    //vrátí stopky linky
    vector<Stop*> getStops();
    //vrátí jízdní řády linky
    vector<traffic_t*> getTraf();
    vector<int> Stops_pos();
    bool addBus(bus* t);
    bool addTraff(traffic_t* t);
    //vrátí index ulice na které leží index stopky
    int stop_on(int y);


};

#endif // LINE_H
