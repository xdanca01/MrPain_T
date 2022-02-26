package vut.fit.ija.homework1.myMaps;
import vut.fit.ija.homework1.maps.Coordinate;



public class MyCoordinate implements Coordinate
{
    private int x;
    private int y;

    public static MyCoordinate create(int x, int y)
    {
        if(x < 0 || y < 0) return null;
        MyCoordinate A = new MyCoordinate();
        A.x = x;
        A.y = y;
        return A;
    }

    /**
     * Vrací hodnotu souřadnice x.
     * @return Souřadnice x.
     */
    public int getX()
    {
        return this.x;
    }

    /**
     * Vrací hodnotu souřadnice y.
     * @return Souřadnice y.
     */
    public int getY()
    {
        return this.y;
    }
    
    public boolean equals(Object obj)
    {
        if (obj == this)
        {
            return true;
        }
        else if (obj == null || obj.getClass() != this.getClass())
        {
            return false;
        }
        MyCoordinate test = (MyCoordinate) obj;
        return test.getX() == this.getX() && test.getY() == this.getY();
    }

}
