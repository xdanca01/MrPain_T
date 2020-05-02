#include "Stop.h"

using namespace std;





Stop::Stop(string str, Coordinate *c)
{
    cords = c;
    Id = str;
}

string Stop::getId()
{
    return Id;
}

Coordinate* Stop::getCoordinate()
{
    return cords;
}

void Stop::setStreet(Street *s)
{
    this->stret = s;
}

Street *Stop::getStreet()
{
    return this->stret;
}



Stop* Stop::clone()
{
    Coordinate* c2 = this->getCoordinate();
    Coordinate *c = new Coordinate(c2->getX(), c2->getY());
    return new Stop(this->getId(), c);
}

bool Stop::equals(Stop* s)
{
    if(s->getCoordinate()->equals(this->getCoordinate()) && 0 == s->getId().compare(this->getId()))
        return true;
    return false;
}
