#ifndef BUS_H
#define BUS_H

#include "Coordinate.h"
#include <QGraphicsLineItem>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTableWidget>
#include "traffic_t.h"

class traffic_t;

class bus : public QGraphicsLineItem
{
    int id;
    double x;
    double y;
    traffic_t* traf = nullptr;
    bool garage = true;
    QTableWidget* tab;
    //vector<>

public:
    bus(QGraphicsItem *parent = nullptr, QTableWidget* table = nullptr);
    void update(double Xx, double Yy);
    double getX();
    double getY();
    void start();
    void end();
    bool status();
    void add_T(traffic_t* t);

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BUS_H