
#ifndef stoop
#define stoop


#include <string>

#include "Coordinate.h"
#include "Street.h"
#include <QGraphicsEllipseItem>

class Street;

using namespace std;
class Stop : public QGraphicsEllipseItem{
    
    string Id;
    Coordinate* cords = nullptr;
    Street* stret = nullptr;

    public:
        Stop(QGraphicsEllipseItem *parent = nullptr, string str = "", Coordinate* c = nullptr);
        string getId();
        Coordinate* getCoordinate();
        void setStreet(Street* s);
        Street* getStreet();
        Stop* clone();
        bool equals(Stop* s);
};


#endif
