#include "Coordinate.h"

Coordinate::Coordinate(double Xx, double Yy)
{
    x = Xx;
    y = Yy;
}

double Coordinate::getX()
{
    return this->x;
}
double Coordinate::getY()
{
    return this->y;
}

bool Coordinate::equals(Coordinate *c)
{
    if(c == this) return true;
    if((c->getX() >= this->getX() && c->getX()-0.0001 <= this->getX()) || (c->getX() <= this->getX() && c->getX()+0.0001 >= this->getX()))
    {
        if((c->getY() >= this->getY() && c->getY()-0.0001 <= this->getY()) || (c->getY() <= this->getY() && c->getY()+0.0001 >= this->getY()))
            return true;
    }
        return false;
}
