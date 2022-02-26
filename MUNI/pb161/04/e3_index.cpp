#include <cassert>
#include <vector>
#include <utility>

/* In this exercise, you will provide index-based access to pairs
 * and vectors of integers, using function overloading. The
 * ‹element› function should take an ‹std::vector› or an ‹std::pair›
 * as its first argument and an index as its second argument. A
 * companion ‹size› function should return the number of valid
 * indices for either of the two types of objects. */

// ??? element( ???, int idx );
// ??? size( ??? );

int main()
{
    std::pair p{ 1, 2 };
    const auto &p_const = p;
    std::vector< int > v{ 1, 2, 3 };
    const auto &v_const = v;

    assert( size( p ) == 2 );
    assert( size( v ) == 3 );
    assert( size( p_const ) == 2 );
    assert( size( v_const ) == 3 );

    assert( element( p, 0 ) == 1 );
    assert( element( p, 1 ) == 2 );
    assert( element( p_const, 0 ) == 1 );
    assert( element( p_const, 1 ) == 2 );
    element( p, 1 ) = 3;
    assert( element( p_const, 1 ) == 3 );

    assert( element( v, 0 ) == 1 );
    assert( element( v, 2 ) == 3 );
    assert( element( v_const, 0 ) == 1 );
    element( v, 1 ) = 3;
    assert( element( v_const, 0 ) == 1 );
    assert( element( v_const, 1 ) == 3 );
    assert( element( v_const, 2 ) == 3 );

    return 0;
}
