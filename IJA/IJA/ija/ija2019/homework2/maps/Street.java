package ija.ija2019.homework2.maps;

import java.util.List;

public interface Street{

	public String getId();
	
	public List<Coordinate> getCoordinates();

	public Coordinate begin();

	public Coordinate end();

	public List<Stop> getStops();

	public boolean addStop(Stop stop);

	public boolean follows​(Street s);
	

}