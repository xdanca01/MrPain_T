#ifndef BUS_H
#define BUS_H

#include "Coordinate.h"
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTableWidget>
#include "traffic_t.h"

class traffic_t;

class bus : public QGraphicsEllipseItem
{
    int id;
    double x;
    double y;
    traffic_t* traf = nullptr;
    bool garage = true;
    QTableWidget* tab;

public:
    bus(QGraphicsItem *parent = nullptr, QTableWidget* table = nullptr, traffic_t* t = nullptr);
    void update(double Xx, double Yy);
    double getX();
    double getY();
    void start();
    void end();
    bool status();
    void add_T(traffic_t* t);

protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BUS_H
