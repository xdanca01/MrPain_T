package ija.ija2019.homework2.maps;

import ija.ija2019.homework2.maps.*;
import java.util.List;
import java.util.AbstractMap.SimpleImmutableEntry;

public interface Line{

	public boolean addStop(Stop stop);
	public boolean addStreet(Street street);
	public List<AbstractMap.SimpleImmutableEntry<Street,Stop>> getRoute();
	public static Line defaultLine(String id){
	    Line A = new Line();
	    A.id = id;
        return A;
    }


}
