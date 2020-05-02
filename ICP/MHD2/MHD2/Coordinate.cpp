#include "Coordinate.h"

Coordinate::Coordinate(int Xx, int Yy)
{
    x = Xx;
    y = Yy;
}

int Coordinate::getX()
{
    return x;
}
int Coordinate::getY()
{
    return y;
}

bool Coordinate::equals(Coordinate *c)
{
    if(c == this) return true;
    if(c->getX() == this->getX() && c->getY() == this->getY())
        return true;
    return false;
}
