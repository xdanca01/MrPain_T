package vut.fit.ija.homework1.myMaps;

public class MyCoordinate implements Coordinate
{
    private int x;
    private int y;

    public MyCoordinate Create(int x, int y)
    {
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


}
