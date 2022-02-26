#include <cassert>

/* Implement 2 classes which represent 2D shapes: (regular)
 * ‹polygon› and ‹circle›. Each of the shapes has 2 constructors:
 *
 *  • ‹circle› takes either 2 points (center and a point on the
 *    circle) or a point and a number (radius),
 *  • ‹polygon› takes an integer (the number of sides ≥ 3) and
 *    either two points (center and a vertex) or a single point and a
 *    number (the major radius).
 *
 * Add a toplevel function ‹area› which can compute the area of
 * either. */

struct point;
struct polygon;
struct circle;

int main()
{
    polygon square( 4, point( 0, 0 ),  std::sqrt( 2 ) );
    assert( std::fabs( area( square ) - 4 ) < .001 );
    polygon hexagon( 6, point( 0, 0 ), point( 0, 1 ) );
    assert( std::fabs( area( hexagon ) - 2.598 ) < .001 );
    circle c1( point( 0, 0 ), point( 1, 0 ) );
    circle c2( point( 0, 0 ), point( 2, 0 ) );
    circle c3( point( 0, 0 ), point( 1, 1 ) );
    assert( std::fabs( area( c1 ) -     3.1415 ) < .001 );
    assert( std::fabs( area( c2 ) - 4 * 3.1415 ) < .001 );
    assert( std::fabs( area( c3 ) - 2 * 3.1415 ) < .001 );

    return 0;
}
