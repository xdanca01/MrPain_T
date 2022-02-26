#include <cmath>
#include <cassert>

/* Standard point in a plane, with x and y coordinates, stored as
 * double-precision floating point numbers, with the obvious
 * constructor. */

struct point;

/* Define a structure which describes a circle with a given center
 * and a given radius (a point and a non-negative number). Include a
 * straightforward constructor. */

struct circle_radius;

/* And a structure, which describes a circle using two points: the
 * center and a point on the circle. Again, add a constructor. */

struct circle_point;

/* Finally, define function ‹diameter› which given either of the
 * above representations of a circle, returns its diameter (i.e.
 * twice the radius). */

// double diameter( ??? );

int main()
{
    point p_1( 0, 0 ), p_2( 1, 0 ), p_3( 3, 4 );
    circle_radius c_1( p_1, 1 ), c_2( p_2, 2 );
    circle_point c_3( p_1, p_2 ), c_4( p_1, p_3 );

    assert( std::fabs( diameter( c_1 ) -  2 ) < 1e-8 );
    assert( std::fabs( diameter( c_2 ) -  4 ) < 1e-8 );
    assert( std::fabs( diameter( c_3 ) -  2 ) < 1e-8 );
    assert( std::fabs( diameter( c_4 ) - 10 ) < 1e-8 );

    return 0;
}
