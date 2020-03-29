package ija.ija2019.homework2.maps;

import ija.ija2019.homework2.maps.*;
import ija.ija2019.homework2.myMaps.*;
import java.util.List;

public interface Street{

	public String getId();
	
	public List<Coordinate> getCoordinates();

	public Coordinate begin();

	public Coordinate end();

	public List<Stop> getStops();

	public boolean addStop(Stop stop);

	public boolean follows (Street s);

    public Street clone();

    static Street defaultStreet(String id, Coordinate c1, Coordinate c2)
    {
        return new MyStreet(id,c1,c2);
    }

    static Street defaultStreet(String id, Coordinate c1, Coordinate c2, Coordinate c3)
    {
        if(c1.getX() != c2.getX() && c1.getY() != c2.getY()) return null;
        if(c2.getX() != c3.getX() && c2.getY() != c3.getY()) return null;
        return new MyStreet(id, c1, c2, c3);
    
    }
	

}
