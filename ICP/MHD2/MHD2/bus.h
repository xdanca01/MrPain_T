#ifndef BUS_H
#define BUS_H

#include "Coordinate.h"
#include <QGraphicsLineItem>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

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

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BUS_H
