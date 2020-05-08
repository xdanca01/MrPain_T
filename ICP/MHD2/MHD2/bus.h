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

//třída symbolizující autobus na mapě
class bus : public QGraphicsEllipseItem
{
    string iid;
    double x;
    double y;
    traffic_t* traf = nullptr;
    QTableWidget* tab;

public:
    //předá se table a jízdní řád pro zápis trasy
    bus(QGraphicsItem *parent = nullptr, QTableWidget* table = nullptr, traffic_t* t = nullptr, string name = "");
    //aktualizace souřadnic na mapě
    void update(double Xx, double Yy);
    double getX();
    double getY();
    //zobrazí autobus na mapě
    void start();
    //schová autobus na mapě
    void end();
    string getId();


protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BUS_H
