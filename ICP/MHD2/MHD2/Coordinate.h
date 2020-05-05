#ifndef cooord
#define cooord

class Coordinate{
    //Coordinates
    double x,y;
    public:
        Coordinate(double x,double y);
        double getX();
        double getY();
        bool equals(Coordinate* c);
            
};
#endif
