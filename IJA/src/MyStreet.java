package vut.fit.ija.homework1.myMaps;

public class MyStreet implements Street
{
    private String Id;
    private List<Coordinate> seznam;
    private List<Stop> zastavky;



    public MyStreet(String jmeno, Coordinate souradnice1, Coordinate souradnice2)
    {
        this.Id = jmeno;
        this.seznam = new ArrayList();
        this.zastavky = new ArrayList();
        this.seznam.add(souradnice1);
        this.seznam.add(souradnice2);
    }
    public MyStreet()
    {
        this.Id = null;
        this.seznam = new ArrayList();
        this.zastavky = new ArrayList();
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
    public List<Coordinate> getCoordinates()
    {
        return this.seznam;
    }

    /**
     * Vrátí seznam zastávek na ulici.
     * @return Seznam zastávek na ulici. Pokud ulize nemá žádnou zastávku, je seznam prázdný.
     */
    public List<Stop> getStops()
    {
        return this.zastavky;
    }

    /**
     * Přidá do seznamu zastávek novou zastávku.
     * @param stop Nově přidávaná zastávka.
     */
    public void addStop(Stop stop)
    {
        this.zastavky.add(stop);
    }
}
