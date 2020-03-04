package vut.fit.ija.homework1.myMaps;
import vut.fit.ija.homework1.maps.*;
import java.util.List;



public class MyStop implements Stop
{
    private String Id;
    private Coordinate souradnice;
    private Street ulice;
    

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
