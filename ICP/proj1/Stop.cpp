#include "Stop.h"

using namespace std;

Stop::Stop(String str, Coordinate c)
{
    Id = str;
    cords = c;
}

Stop::Stop(String str)
{
    Id = str;

}

String getId()
{

    return Id;
}

Coordinate::Coordinate getCoordinate();
{
    return cords;
}

void setStreet(Street::Street s)
{
    street = s;
}

Street::Street getStreet()
{
    return street;
}

Stop::Stop clone()
{
    Coordinate::Coordinate c2 = this.getCoordinate();
    Coordinate::Coordinate c = Coordinate(c2.getX(), c2.getY());
    return new Stop(this.getId(), c);
}
