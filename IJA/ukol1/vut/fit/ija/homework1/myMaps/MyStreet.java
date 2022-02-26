package vut.fit.ija.homework1.myMaps;
import vut.fit.ija.homework1.maps.*;
import java.util.ArrayList;


public class MyStreet implements Street
{
    private String Id;
    private ArrayList<Coordinate> seznam;
    private ArrayList<Stop> zastavky;

    public MyStreet(String jmeno, Coordinate souradnice1, Coordinate souradnice2)
    {
        this.Id = jmeno;
        this.seznam = new ArrayList<Coordinate>();
        this.zastavky = new ArrayList<Stop>();
        this.seznam.add(souradnice1);
        this.seznam.add(souradnice2);
    }

    public MyStreet()
    {
        this.Id = null;
        this.seznam = new ArrayList<Coordinate>();
        this.zastavky = new ArrayList<Stop>();
    }

    /**
     * Vrátí identifikátor ulice.
     * @return Identifikátor ulice.
     */
    public String getId()
    {
        return this.Id;
    }

    /**
     * Vrátí seznam souřadnic definujících ulici. První v seznamu je vždy počátek a poslední v seznamu konec ulice.
     * @return Seznam souřadnic ulice.
     */
    public ArrayList<Coordinate> getCoordinates()
    {
        return this.seznam;
    }

    /**
     * Vrátí seznam zastávek na ulici.
     * @return Seznam zastávek na ulici. Pokud ulize nemá žádnou zastávku, je seznam prázdný.
     */
    public ArrayList<Stop> getStops()
    {
        return this.zastavky;
    }

    /**
     * Přidá do seznamu zastávek novou zastávku.
     * @param stop Nově přidávaná zastávka.
     */
    public void addStop(Stop stop)
    {   stop.setStreet(this);
        this.zastavky.add(stop);
    }
}
