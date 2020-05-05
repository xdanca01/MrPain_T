#ifndef TRAFFIC_T_H
#define TRAFFIC_T_H

#include "Street.h"
#include "bus.h"
#include "Stop.h"
#include <vector>
#include <QTime>

class bus;

class traffic_t
{
    bus* bs = nullptr;
    string name;
    vector<Street*> stretts;
    vector<Stop*> stopp;
    vector<QTime*> times;
public:
    traffic_t(string nam);
    void addT(QTime* t);
    void addStop(vector<Stop*> stop);
    void addStreet(vector<Street*> stret);
    bus* getB();
    void addBus(bus *bb);
    vector<Street*> getS();
    vector<Stop*> getStop();
    vector<QTime*> getT();
    string getName();
};

#endif // TRAFFIC_T_H
