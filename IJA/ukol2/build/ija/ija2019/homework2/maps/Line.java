package ija.ija2019.homework2.maps;

import ija.ija2019.homework2.maps.*;
import ija.ija2019.homework2.myMaps.*;
import java.util.List;
import java.util.AbstractMap.SimpleEntry;

public interface Line{

	public boolean addStop(Stop stop);
	public boolean addStreet(Street street);
	public List<SimpleEntry<Street,Stop>> getRoute();
	public static Line defaultLine(String id){
        return new MyLine(id);
    }


}
