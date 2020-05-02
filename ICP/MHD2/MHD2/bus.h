#ifndef BUS_H
#define BUS_H

#include "Coordinate.h"
#include <QGraphicsLineItem>
#include <QPen>

class bus : public QGraphicsLineItem
{
    int id;
    double x;
    double y;
    bool garage = true;
    //vector<>

public:
    bus(QGraphicsItem *parent = nullptr);
    void update(double Xx, double Yy);
    double getX();
    double getY();
    void start();
    void end();
    bool status();
};

#endif // BUS_H
