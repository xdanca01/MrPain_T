
#ifndef stoop
#define stoop


#include <string>

#include "Coordinate.h"
#include "Street.h"

class Street;

using namespace std;
class Stop{
    
    string Id;
    Coordinate* cords = nullptr;
    Street* stret = nullptr;

    public:
        Stop(string str, Coordinate* c);
        string getId();
        Coordinate* getCoordinate();
        void setStreet(Street* s);
        Street* getStreet();
        Stop* clone();
        bool equals(Stop* s);
};


#endif
