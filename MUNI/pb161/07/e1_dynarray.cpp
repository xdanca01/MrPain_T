#include <cassert>

/* Implement a dynamic array of integers with 2 operations: element
 * access (using ‹operator[]›) and ‹resize›. The constructor takes
 * the initial size as its only parameter. */

class dynarray;

int main()
{
    dynarray d( 3 );
    d[ 0 ] = 1;
    d[ 1 ] = 2;
    d[ 2 ] = 3;
    assert( d[ 0 ] == 1 );
    assert( d[ 2 ] == 3 );
    d.resize( 2 );
    assert( d[ 0 ] == 1 );
    assert( d[ 1 ] == 2 );
    d.resize( 4 );
    assert( d[ 0 ] == 1 );
    assert( d[ 1 ] == 2 );
    d[ 2 ] = 3;
    d[ 3 ] = 4;
    assert( d[ 0 ] == 1 );
    assert( d[ 1 ] == 2 );
    assert( d[ 2 ] == 3 );
    assert( d[ 3 ] == 4 );

    return 0;
}
