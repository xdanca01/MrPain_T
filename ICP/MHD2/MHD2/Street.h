
#ifndef streeet
#define streeet

#include "Stop.h"
#include "Coordinate.h"
#include <QPen>
#include <QGraphicsLineItem>
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
    vector<Coordinate*>* Cords  = nullptr;
    vector<Stop*>* Stops  = nullptr;

    public:
        Street(QGraphicsItem *parent = nullptr,string name = "", Coordinate* c1 = nullptr, Coordinate* c2 = nullptr);
        string getId();
        vector<Coordinate*>* getCoordinates();
        Coordinate* begin();
        Coordinate* end();
        vector<Stop*>* getStops();
        bool addStop(Stop* s);
        bool follows(Street* s);
        void close();
        bool status();
        bool stop_on(Stop* s);


};
#endif

