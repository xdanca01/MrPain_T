#include "line.h"
#include <QDebug>


line::line(string s)
{
    this->Id = s;
}

bool line::addStop(Stop *stop)
{
    int length = 0;
    bool ret = false;
    if(this->getRoute()) length = this->getRoute()->size();
    else this->streets = new vector<Street*>();
    int len = 0;
    if(this->getStops()) len = this->getStops()->size();
    else this->Stops = new vector<Stop*>();
    for(int i = 0; i < len;++i)
    {
        if(stop->equals(this->getStops()[0][i]))
            return false;
    }
    for(int i = 0; i < length;++i)
    {
        if(this->streets->at(i)->addStop(stop) == true)
        {
            this->Stops->push_back(stop);
            ret = true;
        }
    }
    return ret;
}

bool line::addStreet(Street* s)
{
    int length = 0;
    if(this->getRoute()) length = this->getRoute()->size();
    for(int i = 0; i < length;++i)
    {
        Street* s2 = this->streets->at(i);
        if(s2->begin()->equals(s->begin()) || s2->begin()->equals(s->end()) || s2->end()->equals(s->begin()) || s2->end()->equals(s->end()))
        {
            this->streets->push_back(s);
            if(s->getStops() == nullptr) return true;
            int len = s->getStops()->size();
            for(int y = 0;y < len;++y)
            {
                this->addStop(s->getStops()->at(y));
            }
            return true;
        }
    }
    if(length == 0)
    {
        this->streets = new vector<Street*>();
        this->streets->push_back(s);
        if(s->getStops() == nullptr) return true;
        int len = s->getStops()->size();
        for(int y = 0;y < len;++y)
        {
            this->addStop(s->getStops()->at(y));
        }
        return true;
    }
    return false;
}

vector<Street *> *line::getRoute()
{
    return this->streets;
}

vector<Stop *> *line::getStops()
{
    return this->Stops;
}

bool line::addBus(traffic_t *t)
{
    if(this->traff == nullptr) this->traff = new vector<traffic_t*>();
    this->traff->push_back(t);
}


