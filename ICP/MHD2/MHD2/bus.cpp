#include "bus.h"



bus::bus(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{

    this->setPen(QPen({Qt::black}, 8));
    this->setFlag(QGraphicsItem::ItemIsSelectable);
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

void bus::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << this;
    QGraphicsLineItem::mouseDoubleClickEvent(event);
}

double bus::getX()
{
    return x;
}

double bus::getY()
{
    return y;
}



