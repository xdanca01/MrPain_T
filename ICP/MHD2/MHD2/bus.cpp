#include "bus.h"



bus::bus(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{


}

void bus::update(double Xx, double Yy)
{
    x = Xx;
    y = Yy;
}

void bus::start()
{
    this->garage = false;
}

void bus::end()
{
    this->garage = true;
}

bool bus::status()
{
    return this->garage;
}

double bus::getX()
{
    return x;
}

double bus::getY()
{
    return y;
}

