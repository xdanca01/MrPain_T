from __future__ import annotations
from typing import Tuple
from math import sqrt, acos
from math import isclose, pi

class Point:
    def __init__( self, x: float, y: float ) -> None:
        self.x = x
        self.y = y

    def __sub__( self, other: 'Point' ) -> Vector:
        return Vector( self.x - other.x, self.y - other.y )

    def translated( self, vec: Vector ) -> 'Point':
        return Point( self.x + vec.x, self.y + vec.y )

class Vector:

    def __init__( self, x: float, y: float ) -> None:
        self.x = x
        self.y = y

    def __mul__( self, s: float ) -> Vector:
        return Vector( self.x * s, self.y * s )

    def length( self ) -> float:
        return sqrt( self.x * self.x + self.y * self.y )

    def dot( self, other: Vector ) -> float:
        return self.x * other.x + self.y * other.y

    def angle( self, other: Vector ) -> float:
        cos = self.dot( other ) / ( self.length() * other.length() )
        if isclose( cos, 1 ): cos = 1
        if isclose( cos, -1 ): cos = -1
        return acos( cos )

class Line:
    def __init__( self, p1: Point, p2: Point ):
        self.p1 = p1
        self.p2 = p2

    def translated( self, vec: Vector ) -> Line:
        return Line( self.p1.translated( vec ),
                     self.p2.translated( vec ) )

    def get_point( self ) -> Point:
        return self.p1

    def get_direction( self ) -> Vector:
        v_dir = self.p2 - self.p1
        return v_dir * ( 1 / v_dir.length() )


class Segment( Line ):

    def __init__( self, p1: Point, p2: Point ) -> None:
        super( Segment, self ).__init__( p1, p2 )

    def length( self ) -> float:
        return ( self.p2 - self.p1 ).length()

    def translated( self, vec: Vector ) -> Segment:
        return Segment( self.p1.translated( vec ),
                        self.p2.translated( vec ) )

    def get_endpoints( self ) -> Tuple[ Point, Point ]:
        return ( self.p1, self.p2 )

class Circle:
    def __init__( self, c: Point, r: float ) -> None:
        self.c = c
        self.r = r

    def translated( self, vec: Vector ) -> Circle:
        return Circle( self.c.translated( vec ), self.r )

    def center( self ) -> Point:
        return self.c

    def radius( self ) -> float:
        return self.r
