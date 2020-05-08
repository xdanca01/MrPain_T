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
    //jmeno autobusu
    string iid;
    //aktuální souřadnice autobusu
    double x;
    double y;
    //jízdní řád autobusu s časy/ulicemi a stopkami
    traffic_t* traf = nullptr;
    //tabulka pro vypsání informací o autobusu
    QTableWidget* tab;

public:
    //předá se table a jízdní řád pro zápis trasy
    bus(QGraphicsItem *parent = nullptr, QTableWidget* table = nullptr, traffic_t* t = nullptr, string name = "");
    //aktualizace souřadnic na mapě
    void update(double Xx, double Yy);
    //vrátí aktuální pozice
    double getX();
    double getY();
    //zobrazí autobus na mapě
    void start();
    //schová autobus na mapě
    void end();
    //vrátí jméno autobusu
    string getId();


protected:

    //event pro zachycení kliknutí myši na autobus a vypsání infa do tabulky
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //po spuštění smaže data z tabulky
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BUS_H
