package ija.ija2019.homework2.maps;

import ija.ija2019.homework2.myMaps.*;


public interface Stop{

    public Coordinate getCoordinate();
    public String getId();
    public Street getStreet();
    public void setStreet(Street s);
    public static Stop defaultStop(String id, Coordinate c){
        return new MyStop(id, c);
    }



}
