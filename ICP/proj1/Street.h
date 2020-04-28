#pragma once

#include "Stop.h"
#include "Coordinate.h"
#include <String>
#include <List>

using namespace std;
class Street{
    String Id;
    list<Coordinate::Coordinate> Cords;
    list<Stop::Stop> Stops;
    public:
        Street(String name, Coordinate::Coordinate c1, Coordinate::Coordinate c2);
        Street(String name, Coordinate::Coordinate c1, Coordinate::Coordinate c2, Coordinate::Coordinate c3);
        Street();
        String getId();
        list<Coordinate::Coordinate> getCoordinates();
        Coordinate::Coordinate begin();
        Coordinate::Coordinate end();
        list<Stop::Stop> getStops();
        boolean addStop(Stop::Stop stop);
        boolean follows(Street::Street s);
        
    

}
