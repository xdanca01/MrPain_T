#include <cassert>
#include <initializer_list>

/* The class represents a set of integers; operations:
 *   • ‹add› -- adds a number, returns true if it was new
 *   • ‹del› -- removes a number, returns true if it was present
 *   • ‹del_range› -- removes numbers within given bounds (inclusive)
 *   • ‹merge› -- adds all numbers from another instance
 *   • ‹has› -- returns true if the given number is in the set
 *  Complexity requirements:
 *   • ‹del_range› and ‹merge› must run in O(n) time
 *   • everything else in O(logn) time */

class numbers;

int main()
{
    numbers a, b;
    const numbers &x = a, &y = b;

    for ( int n : { 1, 3, 5, 7, 11, 17 } )
        assert( a.add( n ) );
    for ( int n : { 2, 4, 8, 16 } )
        assert( b.add( n ) );

    assert(  x.has( 1 ) );
    assert(  x.has( 7 ) );
    assert( !x.has( 2 ) );
    assert(  y.has( 2 ) );
    assert( !y.has( 7 ) );

    assert(  a.del( 7 ) );
    assert( !x.has( 7 ) );
    assert( !a.del( 7 ) );
    assert( !a.has( 7 ) );
    assert( !a.add( 1 ) );
    assert(  a.add( 7 ) );

    a.del_range( 3, 8 );
    assert( !x.has( 3 ) );
    assert( !x.has( 5 ) );
    assert( !x.has( 7 ) );

    a.merge( y );
    assert( a.has(  1 ) );
    assert( a.has(  2 ) );
    assert( a.has(  4 ) );
    assert( a.has(  8 ) );
    assert( a.has( 11 ) );
    assert( a.has( 17 ) );

    return 0;
}
