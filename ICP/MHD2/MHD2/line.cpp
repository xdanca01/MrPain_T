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
        if(stop->equals(this->getStops()->at(i)))
            return true;
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
    if(this->getRoute() == nullptr) return false;


    int length = this->getRoute()->size();
    vector<Street*>* str = this->getRoute();
    for(int i = 0; i < length;++i)
    {
        Street* s2 = str->at(i);
        if(s2 == s) return true;
        if(s2->begin()->equals(s->begin()) || s2->begin()->equals(s->end()) || s2->end()->equals(s->begin()) || s2->end()->equals(s->end()))
        {
            for(i = i + 1;i < length;++i)
            {
                s2 = str->at(i);
                if(s2 == s) return true;
            }
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

vector<traffic_t *> *line::getTraf()
{
    return this->traff;
}

bool line::addBus(bus *t)
{
    if(t == nullptr) return false;
    if(this->buses == nullptr) this->buses = new vector<bus*>();
    this->buses->push_back(t);
    return true;
}


//přidání jízdního řádu spoje do linky
bool line::addTraf(traffic_t *t)
{
    if(t == nullptr) return false;
    if(this->traff == nullptr) this->traff = new vector<traffic_t*>();
    this->traff->push_back(t);


    vector<Street*> sstreet = t->getS();
    int len = sstreet.size();
    for(int i = 0;i< len;++i)
    {
        this->addStreet(sstreet.at(i));
    }

    vector<Stop*> sstop = t->getStop();
    len = sstop.size();
    for(int i = 0;i<len;++i)
    {
        this->addStop(sstop.at(i));
    }

    return this->addBus(t->getB());
}


