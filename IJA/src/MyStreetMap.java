package vut.fit.ija.homework1.myMaps;

import java.util.List;

public interface MyStreetMap {

    private List<Street> ulice;


    public MyStreetMap()
    {
        this.ulice = new ArrayList();
    }

    /**
     * Přidá ulici do mapy.
     * @param s Objekt reprezentující ulici.
     */
    public void addStreet(Street s)
    }
        this.ulice.add(s);
    }

    /**
     * Vrátí objekt reprezentující ulici se zadaným id.
     * @param id Identifikátor ulice.
     * @return Nalezenou ulici. Pokud ulice s daným identifikátorem není součástí mapy, vrací null.
     */
    public Street getStreet(String id)
    {
        for(Street street : this.ulice)
        {
            if(street)
        }
    }
}
