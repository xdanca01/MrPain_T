#include "traffic_t.h"

traffic_t::traffic_t(string nam)
{
    this->name = nam;
}

void traffic_t::addT(QTime *t)
{
    this->times.push_back(t);
}

void traffic_t::addStop(vector<Stop *> stop)
{
    this->stopp = stop;
}

void traffic_t::addStreet(vector<Street *> stret)
{
    this->stretts = stret;
}

bus *traffic_t::getB()
{
    return this->bs;
}

void traffic_t::addBus(bus *bb)
{
    this->bs = bb;
}

vector<Street*> traffic_t::getS()
{
    return this->stretts;
}

vector<QTime*> traffic_t::getT()
{
    return this->times;
}

string traffic_t::getName()
{
    return this->name;
}

vector<Stop*> traffic_t::getStop()
{
    return this->stopp;
}
