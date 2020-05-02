#ifndef cooord
#define cooord

class Coordinate{
    //Coordinates
    int x,y;
    public:
        Coordinate(int x, int y);
        int getX();
        int getY();
        bool equals(Coordinate* c);
            
};
#endif
