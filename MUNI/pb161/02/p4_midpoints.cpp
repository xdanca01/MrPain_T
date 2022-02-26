#include <vector>
#include <cmath>
#include <cassert>

/* A familiar class: add a 2-parameter constructor and x(), y()
 * accessors (the coordinates should be double-precision floating
 * point numbers). */

class point;

/* Consider a closed shape made of line segments. Replace each
 * segment A with one that starts at the midpoint of A and ends at
 * the midpoint of B, the segment that comes immediately after A.
 * The input is given as a sequence of points (each point shared by
 * two segments). The last segment goes from the last point to the
 * first point (closing the shape). */

void midpoints( std::vector< point > &pts );

/* helper functions for floating-point almost-equality */

bool near( double a, double b )
{
    return std::fabs( a - b ) < 1e-8;
}

bool near( point a, point b )
{
    return near( a.x(), b.x() ) && near( a.y(), b.y() );
}

int main()
{
    using point_vec = std::vector< point >;

    point_vec s1{ { 0, 0 }, { 4, 0 }, { 4, 3 } };
    midpoints( s1 );

    assert( near( s1[ 0 ], point( 2, 0 ) ) );
    assert( near( s1[ 1 ], point( 4, 1.5 ) ) );
    assert( near( s1[ 2 ], point( 2, 1.5 ) ) );

    point_vec s2{ { 0, 0 }, { 0, 4 }, { 4, 4 }, { 4, 0 } };
    midpoints( s2 );

    assert( near( s2[ 0 ], point( 0, 2 ) ) );
    assert( near( s2[ 1 ], point( 2, 4 ) ) );
    assert( near( s2[ 2 ], point( 4, 2 ) ) );
    assert( near( s2[ 3 ], point( 2, 0 ) ) );

    return 0;
}
