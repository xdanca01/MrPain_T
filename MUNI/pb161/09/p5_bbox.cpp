#include <cassert>
#include <vector>
#include <deque>
#include <utility>

/* We will dust off geometry a little bit: we will look at
 * constructing a bounding box around a sequence of points (this
 * time in 3D). */

/* Points can be constructed from three floating-point numbers (of
 * type ‹double›. */

struct point;

/* There is a ‹dist› function which gives the Euclidean distance of
 * two points. */

double dist( point a, point b );

/* A helper function to check approximate point equality. */

bool close( point a, point b ) { return dist( a, b ) < 1e-10; }

/* Now for the bounding box: we want an axis-aligned box (i.e. not
 * the smallest one), and will represent it using 2 points -- those
 * in the opposite corners. Some of the resulting dimensions might
 * be 0 (in case the points all lie on a line or in a plane). Return
 * the points in such a way that the coordinates of the first one
 * are smaller along all axes. It should be possible to pass the
 * points using a ‹const› reference to any container which can be
 * iterated. */

using box_t = std::pair< point, point >;
// ... box_t box( ... )

int main()
{
    std::vector< point > pvec;
    std::deque< point > pdeq;
    const auto & c_pvec = pvec;
    const auto & c_pdeq= pdeq;

    pvec.emplace_back(  0, 0, 0 );
    pvec.emplace_back( -1, 0, 0 );
    pvec.emplace_back( 1, 0, -1 );
    pvec.emplace_back( 1, -2, 0 );

    auto [ box_1a, box_1b ] = box( c_pvec );

    assert( close( box_1a, point( -1, -2, -1 ) ) );
    assert( close( box_1b, point(  1,  0,  0 ) ) );

    pvec.emplace_back( 10, 10,  0 );
    pvec.emplace_back( 10, 10, 12 );

    auto [ box_2a, box_2b ] = box( c_pvec );

    assert( close( box_2a, point( -1, -2, -1 ) ) );
    assert( close( box_2b, point( 10, 10, 12 ) ) );

    pdeq.emplace_back( 1, 1, 1 );

    auto [ box_3a, box_3b ] = box( c_pdeq );

    assert( close( box_3a, point( 1, 1, 1 ) ) );
    assert( close( box_3b, point( 1, 1, 1 ) ) );

    pdeq.emplace_back( 3, 2, 1 );

    auto [ box_4a, box_4b ] = box( c_pdeq );
    assert( close( box_4a, point( 1, 1, 1 ) ) );
    assert( close( box_4b, point( 3, 2, 1 ) ) );

    pdeq.emplace_back( 2, 2, 1 );

    auto [ box_5a, box_5b ] = box( c_pdeq );
    assert( close( box_5a, point( 1, 1, 1 ) ) );
    assert( close( box_5b, point( 3, 2, 1 ) ) );

    pdeq.emplace_back( -1, 7, 1 );

    auto [ box_6a, box_6b ] = box( c_pdeq );
    assert( close( box_6a, point( -1, 1, 1 ) ) );
    assert( close( box_6b, point(  3, 7, 1 ) ) );

    for ( double i = 1.1; i < 2; i += 0.05 )
        pdeq.emplace_back( i, i, 1 );

    auto [ box_7a, box_7b ] = box( c_pdeq );
    assert( close( box_7a, point( -1, 1, 1 ) ) );
    assert( close( box_7b, point(  3, 7, 1 ) ) );

    return 0;

    // clang-tidy: -cert-flp30-c,-clang-analyzer-security.FloatLoopCounter
}
