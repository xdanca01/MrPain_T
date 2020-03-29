package ija.ija2019.homework2.myMaps;
import ija.ija2019.homework2.maps.*;
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

    public MyStreet(String jmeno, Coordinate souradnice1, Coordinate souradnice2, Coordinate c3)
    {
        this.Id = jmeno;
        this.seznam = new ArrayList<Coordinate>();
        this.zastavky = new ArrayList<Stop>();
        this.seznam.add(souradnice1);
        this.seznam.add(souradnice2);
        this.seznam.add(c3);
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

    //Vrátí souřadnice začátku ulice
    public Coordinate begin()
    {
        return this.seznam.get(0);
    }

    //Vrátí souřadnice konce ulice
    public Coordinate end()
    {
        return this.seznam.get(seznam.size() - 1);
    }

    /**
     * Vrátí seznam zastávek na ulici.
     * @return Seznam zastávek na ulici. Pokud ulice nemá žádnou zastávku, je seznam prázdný.
     */
    public ArrayList<Stop> getStops()
    {
        return this.zastavky;
    }

    /**
     * Přidá do seznamu zastávek novou zastávku.
     * @param stop Nově přidávaná zastávka.
     */
    public boolean addStop(Stop stop)
    {   
        int delka = this.seznam.size();
        Coordinate c3 = stop.getCoordinate();
        for(int i = 1; i < delka; ++i)
        {
            Coordinate c1 = this.seznam.get(i - 1);
            Coordinate c2 = this.seznam.get(i);
            int diffx = c1.getX() - c2.getX();
            int diffy = c1.getY() - c2.getY();
            if(diffx == 0)
            {
                if(c3.getX() == c1.getX() && ((c3.getY() >= c1.getY() && c3.getY() <= c2.getY()) || (c3.getY() <= c1.getY() && c3.getY() >= c2.getY()) ))
                {
                    stop.setStreet(this);
                    this.zastavky.add(stop);
                    return true;
                }
            }
            else if(diffy == 0)
            {
                if(c3.getY() == c1.getY() && ((c3.getX() >= c1.getX() && c3.getX() <= c2.getX()) || (c3.getX() <= c1.getX() && c3.getX() >= c2.getX()) ))
                {
                    stop.setStreet(this);
                    this.zastavky.add(stop);
                    return true;
                }
            }

        }
        return false;
    }


    public boolean follows(Street s)
    {
        if(this.end().equals(s.begin()) || this.begin().equals(s.end()) || this.end().equals(s.end()) || this.begin().equals(s.begin())) return true;
        return false;
    }

    public boolean equals(Object obj)
    {
        if (obj == this) return true;

        else if (obj == null || obj.getClass() != this.getClass()) return false;

        Street test = (Street) obj;
        return test.getId() == this.getId();

    }

    public Street clone()
    {
        Street s;
        if(this.seznam.size() == 2)
        {
            s = new MyStreet(this.getId(), this.seznam.get(0).clone(), this.seznam.get(1).clone());
        }
        else
        {
            s = new MyStreet(this.getId(), this.seznam.get(0).clone(), this.seznam.get(1).clone(), this.seznam.get(2).clone());
        }
        int delka = this.zastavky.size();
        for(int i = 0; i < delka; ++i)
        {
            s.addStop(this.zastavky.get(i).clone());
        }

        return s;
        
    }

}
