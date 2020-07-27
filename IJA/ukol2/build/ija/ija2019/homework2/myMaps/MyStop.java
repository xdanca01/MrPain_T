package ija.ija2019.homework2.myMaps;
import ija.ija2019.homework2.maps.*;
import java.util.List;



public class MyStop implements Stop
{
    private String Id;
    private Coordinate souradnice;
    private Street ulice;


    public String toString()
    {
        return "stop(" + this.getId() + ")";
    }

    public MyStop(String neco)
    {
        Id = neco;
        souradnice = null;
        ulice = null;
    }

    public MyStop(String neco, Coordinate toto)
    {
        Id = neco;
        souradnice = toto;
        ulice = null;
    }

    /**
     * Vrátí identifikátor zastávky.
     * @return Identifikátor zastávky.
     */
    public String getId()
    {
        return this.Id;
    }
    /**
     * Vrátí pozici zastávky.
     * @return Pozice zastávky. Pokud zastávka existuje, ale dosud nemá umístění, vrací null.
     */
    public Coordinate getCoordinate()
    {
        return this.souradnice;
    }

    /**
     * Nastaví ulici, na které je zastávka umístěna.
     * @param s Ulice, na které je zastávka umístěna.
     */
    public void setStreet(Street s)
    {
        this.ulice = s;
    }

    public Stop clone()
    {
        Coordinate c2 = this.getCoordinate();
        Coordinate c = Coordinate.create(c2.getX(), c2.getY());
        return new MyStop(this.getId(), c);
    }

    /**
     * Vrátí ulici, na které je zastávka umístěna.
     * @return Ulice, na které je zastávka umístěna. Pokud zastávka existuje, ale dosud nemá umístění, vrací null.
     */
    public Street getStreet()
    {
        return this.ulice;
    }

    public boolean equals(Object obj)
    {
        if(obj instanceof Stop)
        {
            Stop neco = (Stop) obj;
            return this.Id.equals(neco.getId());
        }
        return false;
    }

}
