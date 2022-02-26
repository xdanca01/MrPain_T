#pragma once

#include <list>
#include <string>
#include "Coordinate.h"
#include "Street.h"

using namespace std;
class Stop{
    
    String Id;
    Coordinate::Coordinate cords;
    Street::Street street;

    public:
        Stop(String str, Coordinate c);
        Stop(String str);
        String getId();
        Coordinate::Coordinate getCoordinate();
        void setStreet(Street::Street s);
        Street::Street getStreet();
        Stop clone();
};
