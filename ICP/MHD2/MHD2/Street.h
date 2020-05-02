
#ifndef streeet
#define streeet

#include "Stop.h"
#include "Coordinate.h"
#include <string>
#include <vector>

using namespace std;

class Stop;

class Street{
    bool closed = false;
    string Id;
    vector<Coordinate*>* Cords  = nullptr;
    vector<Stop*>* Stops  = nullptr;
    public:
        Street(string name, Coordinate* c1, Coordinate* c2);
        string getId();
        vector<Coordinate*>* getCoordinates();
        Coordinate* begin();
        Coordinate* end();
        vector<Stop*>* getStops();
        bool addStop(Stop* s);
        bool follows(Street* s);
        void close();
        bool status();



};
#endif

