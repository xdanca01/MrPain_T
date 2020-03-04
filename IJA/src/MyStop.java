package vut.fit.ija.homework1.myMaps;

public class MyStop implements Stop
{
    private String Id;
    private Coordinate souradnice;
    private Street ulice;
    

    public MyStop(String neco, Coordinate toto)
    {
        this.Id = neco;
        this.souradnice = toto;
        this.ulice = null;
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
}
