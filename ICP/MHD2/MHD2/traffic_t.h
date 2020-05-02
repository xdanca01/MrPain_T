#ifndef TRAFFIC_T_H
#define TRAFFIC_T_H

#include "Street.h"
#include "bus.h"
#include "Stop.h"
#include <vector>
#include <QTime>

class traffic_t
{
    bus* b = nullptr;
    vector<Street*> stretts;
    vector<Stop*> stopp;
    vector<QTime*> times;
public:
    traffic_t(bus* b,vector<Street*> stret,vector<QTime*> times,vector<Stop*> s);
    bus* getB();
    vector<Street*> getS();
    vector<Stop*> getStop();
    vector<QTime*> getT();
};

#endif // TRAFFIC_T_H
