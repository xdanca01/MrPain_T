package ija.ija2019.homework2.maps;

public class Coordinate{

	private int x;
	private int y;

	public int getX(){
		return this.x;
	}
	public int getY(){
		return thix.y;
	}
	public static Coordinate create(int x, int y){
		if( x < 0 || y < 0) return null;
		Coordinate A = new Coordinate();
		A.x = x;
		A.y = y;
		return A;
	}
	public int diffX(Coordinate c){
		return this.x - c.getX();
	}
	public int diffY(Coordinate c){
		return this.y - c.getY();
	}
	public boolean equals(Object obj){
		if (obj == this) return true;

		else if (obj == null || obj.getClass() != 	this.getClass()) return false;

		Coordinate test = (Coordinate) obj;
		return text.getX() == this.getX() && test.getY() == 	this.getY();
	}
	//public int hashCode()



}