
#ifndef streeet
#define streeet

#include "Stop.h"
#include "Coordinate.h"
#include <QPen>
#include <QGraphicsLineItem>
#include <QSpinBox>
#include <string>
#include <vector>
#include <math.h>
#include <QDebug>

using namespace std;

class Stop;

class Street : public QGraphicsLineItem
{

    bool closed = false;
    string Id;
    vector<Coordinate*> Cords;
    vector<Stop*> Stops;
    int delay = 10;
    QSpinBox* spin;

    public:
        Street(QGraphicsItem *parent = nullptr, QSpinBox* sp = nullptr, string name = "", Coordinate* c1 = nullptr, Coordinate* c2 = nullptr);
        string getId();
        vector<Coordinate*> getCoordinates();
        Coordinate* begin();
        Coordinate* end();
        vector<Stop*> getStops();
        int getDelay();
        bool addStop(Stop* s);
        void setDelay(int d);
        bool follows(Street* s);
        void close();
        bool status();
        bool stop_on(Stop* s);


        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

};
#endif

