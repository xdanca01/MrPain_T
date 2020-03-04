public class MyStreet implements Street
{
    private String Id;
    private List<Coordinate> seznam;
    private List<Stop> zastavky;



    public MyStreet(String jmeno, Coordinate souradnice1, Coordinate souradnice2)
    {
        MyStreet A = new MyStreet();
        A.Id = jmeno;
        A.List.add(souradnice1);
        A.List.add(souradnice2);
        return A;
    }
    public MyStreet()
    {
        MyStreet A = new MyStreet();
        return A;
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
