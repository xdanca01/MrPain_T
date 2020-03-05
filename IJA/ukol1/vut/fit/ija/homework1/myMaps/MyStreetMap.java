package vut.fit.ija.homework1.myMaps;

import vut.fit.ija.homework1.maps.*;
import java.util.ArrayList;


public class MyStreetMap implements StreetMap{

    private ArrayList<Street> ulice;


    public MyStreetMap()
    {
        this.ulice = new ArrayList<Street>();
    }

    /**
     * Přidá ulici do mapy.
     * @param s Objekt reprezentující ulici.
     */
    public void addStreet(Street s)
    {
        this.ulice.add(s);
    }

    /**
     * Vrátí objekt reprezentující ulici se zadaným id.
     * @param id Identifikátor ulice.
     * @return Nalezenou ulici. Pokud ulice s daným identifikátorem není součástí mapy, vrací null.
     */
    public Street getStreet(String id)
    {
        for(Street street : ulice)
        {
            if(street.getId().equals(id))
            {
                return street;
            }
        }
        return null;
    }
}
