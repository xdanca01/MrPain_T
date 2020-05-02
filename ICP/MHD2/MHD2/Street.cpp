#include "Street.h"
#include <math.h>
#include <QDebug>

Street::Street(string name, Coordinate *c1, Coordinate *c2)
{
    this->Id = name;
    this->Cords = new vector<Coordinate*>();
    this->Cords->push_back(c1);
    this->Cords->push_back(c2);
}

string Street::getId()
{
    return this->Id;
}

vector<Coordinate *>* Street::getCoordinates()
{
    return this->Cords;
}

Coordinate *Street::begin()
{
    if(this->Cords)
        return this->Cords[0][0];
    return nullptr;
}

Coordinate *Street::end()
{
    if(this->Cords)
        return this->Cords->at(this->Cords->size()-1);
    return nullptr;
}

vector<Stop *>* Street::getStops()
{
    return this->Stops;
}

bool Street::addStop(Stop *s)
{
    unsigned int length = this->Cords->size();
    Coordinate* c3 = s->getCoordinate();
    for(unsigned i = 1; i < length; ++i)
    {
        Coordinate* c1 = this->Cords->at(i - 1);
        Coordinate* c2 = this->Cords->at(i);
        double d1 = sqrt(pow(c3->getX()-c2->getX(),2.0)+pow(c3->getY()-c2->getY(),2.0));
        double d2 = sqrt(pow(c1->getX()-c3->getX(),2.0)+pow(c1->getY()-c3->getY(),2.0));
        double d3 = sqrt(pow(c1->getX()-c2->getX(),2.0)+pow(c1->getY()-c2->getY(),2.0));
        if((d3 >= d2+d1 && d3-0.001 <= d2+d1) || (d3 <= d2+d1 && d3+0.001 >= d2+d1))
        {
            if(this->Stops == nullptr) this->Stops = new vector<Stop*>();
            this->Stops->push_back(s);
            s->setStreet(this);
            return true;
        }
    }
    return false;
}

bool Street::follows(Street *s)
{
    if(this->end()->equals(s->begin()) || this->end()->equals(s->end()) || this->begin()->equals(s->begin()) || this->begin()->equals(s->end()))
    {
        return true;
    }
    return false;
}

void Street::close()
{
    this->closed = true;
}

bool Street::status()
{
    return this->closed;
}

bool Street::stop_on(Stop* s)
{
    if(this->Stops == nullptr) return false;
    int len = this->Stops->size();
    for(int i = 0; i < len;++i)
    {
        if(this->Stops->at(i)->getId() == s->getId()) return true;
    }
    return false;
}


