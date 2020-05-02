#include "traffic_t.h"

traffic_t::traffic_t(bus *b, vector<Street*> stret, vector<QTime*> times, vector<Stop*> s)
{
    this->b = b;
    this->stretts = stret;
    this->times = times;
    this->stopp = s;
}

bus *traffic_t::getB()
{
    return this->b;
}

vector<Street*> traffic_t::getS()
{
    return this->stretts;
}

vector<QTime*> traffic_t::getT()
{
    return this->times;
}

vector<Stop*> traffic_t::getStop()
{
    return this->stopp;
}
