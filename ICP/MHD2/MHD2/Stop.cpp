#include "Stop.h"

using namespace std;





Stop::Stop(QGraphicsEllipseItem *parent, string str, Coordinate *c) : QGraphicsEllipseItem(c->getX()-2,c->getY()-2,4,4,parent)
{
    cords = c;
    Id = str;
    this->setPen(QPen({Qt::green}, 4));
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
    return new Stop(nullptr, this->getId(), c);
}

bool Stop::equals(Stop* s)
{

    if(this == s) return true;
    if(this->getId() == s->getId()) return true;
    if(s->getCoordinate()->equals(this->getCoordinate()) && 0 == s->getId().compare(this->getId()))
        return true;
    return false;
}
