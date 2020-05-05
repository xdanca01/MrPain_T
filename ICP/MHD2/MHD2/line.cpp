#include "line.h"
#include <QDebug>


line::line(string s)
{
    this->Id = s;
}

bool line::addStop(Stop *stop, int on)
{

    int length = this->getRoute().size();
    bool ret = false;
    int len = this->getStops().size();
    for(int i = 0; i < len;++i)
    {
        if(stop->equals(this->getStops().at(i)) == true)
        {
            return true;
        }
    }
    for(int i = 0; i < length;++i)
    {
        if(this->streets.at(i)->addStop(stop) == true)
        {
            this->Stops.push_back(stop);
            this->stops_on.push_back(on);
            ret = true;
        }
    }
    return ret;
}

bool line::addStop(Stop *stop)
{

    int length = this->getRoute().size();
    bool ret = false;
    int len = this->getStops().size();
    for(int i = 0; i < len;++i)
    {
        if(stop->equals(this->getStops().at(i)) == true)
        {
            return true;
        }
    }
    for(int i = 0; i < length;++i)
    {
        if(this->streets.at(i)->addStop(stop) == true)
        {
            this->Stops.push_back(stop);
            ret = true;
        }
    }
    return ret;
}

bool line::addStreet(Street* s)
{
    int length = this->getRoute().size();
    vector<Street*> str = this->getRoute();
    for(int i = 0; i < length;++i)
    {
        Street* s2 = str.at(i);
        if(s2 == s) return true;
        if(s2->begin()->equals(s->begin()) || s2->begin()->equals(s->end()) || s2->end()->equals(s->begin()) || s2->end()->equals(s->end()))
        {
            for(i = i + 1;i < length;++i)
            {
                s2 = str.at(i);
                if(s2 == s) return true;
            }
            this->streets.push_back(s);
            int len = s->getStops().size();
            for(int y = 0;y < len;++y)
            {
                this->addStop(s->getStops().at(y));
            }
            return true;
        }
    }
    if(length == 0)
    {
        this->streets.push_back(s);
        int len = s->getStops().size();
        for(int y = 0;y < len;++y)
        {
            this->addStop(s->getStops().at(y));
        }
        return true;
    }
    return false;
}

string line::getId()
{
    return this->Id;
}

vector<Street*> line::getRoute()
{
    return this->streets;
}

vector<Stop *> line::getStops()
{
    return this->Stops;
}

vector<traffic_t *> line::getTraf()
{
    return this->traff;
}

bool line::addBus(bus *t)
{
    if(t == nullptr) return false;
    this->buses.push_back(t);
    return true;
}


//přidání jízdního řádu spoje do linky
bool line::addTraff(traffic_t *t)
{
    if(t == nullptr) return false;
    this->traff.push_back(t);
    return true;
}

int line::stop_on(int y)
{
    return this->stops_on.at(y);
}


