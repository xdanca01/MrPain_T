package ija.ija2019.homework2.myMaps;

import ija.ija2019.homework2.maps.*;
import java.util.ArrayList;
import java.util.List;
import java.util.AbstractMap.SimpleEntry;

public class MyLine implements Line{

    private String id;
    private ArrayList<Street> ulice;
    private ArrayList<Stop> zastavky;


    public MyLine(String s)
    {
        this.id = s;
        this.ulice = new ArrayList<Street>();
        this.zastavky = new ArrayList<Stop>();
    }



    public boolean addStop(Stop stop)
    {
        Stop s = stop;
        Street stret = s.getStreet();
        int delka = this.zastavky.size();
        if(delka == 0)
        {
            this.zastavky.add(s);
            this.ulice.add(stret);
            return true;
        }
        return false;
        

    }


    public List<SimpleEntry<Street,Stop>> getRoute()
    {
        
        List l = new ArrayList<SimpleEntry<Street,Stop>>();
        int delka = this.ulice.size();
        int cnt = 0;
        for(int i = 0; i < delka; ++i)
        {
            Street s1 = this.ulice.get(i);
            Stop stop = this.zastavky.get(cnt);
            Street s2 = stop.getStreet();
            System.out.println(stop);
            SimpleEntry Se;
            if(s1 != null && s2 != null && s1.getId() == s2.getId())
            {
                Se = new SimpleEntry<Street,Stop>(s1,stop);
                ++cnt;
            }
            else Se = new SimpleEntry<Street,Stop>(s1, null);
            l.add(Se);
        }
        return l;

    }

    /**
     * Přidá ulici do mapy.
     * @param s Objekt reprezentující ulici.
     */
    public boolean addStreet(Street s)
    {
        if(this.ulice.size() == 0)
        {
            if(s.getStops().size() == 0) return false;
            this.ulice.add(s);
            return true;
        }
        Street vzt = this.ulice.get(this.ulice.size() - 1);
        if(vzt.end() != s.begin() && vzt.end() != s.end() && vzt.begin() != s.begin() && vzt.begin() != s.end()) return false;
        this.ulice.add(s);
        return true;
    }

}
