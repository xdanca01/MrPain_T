#include <cassert>

/* In this exercise, we will create a fixed-size array of nibbles (half-bytes),
 * with an indexing operator and with a basic iterator. You may want to refer
 * back to ‹05/nibble.cpp› for details about operators. */

/* The class template ‹nibble_array› should take a single ‹size_t›-typed
 * non-type template argument. It should be possible to index the array and to
 * iterate it using a range ‹for› loop. The storage size should be the least
 * required number of bytes. Default-constructed ‹nibble_array› should have
 * zeroes in all its entries. */

template< size_t N >
class nibble_array;

int main()
{
    static_assert( sizeof( nibble_array< 4 > ) == 2 );
    static_assert( sizeof( nibble_array< 5 > ) == 3 );

    nibble_array< 4 > x;
    nibble_array< 5 > y;

    assert( x[ 0 ] == 0 );
    assert( x[ 1 ] == 0 );

    x[ 0 ] = 1;
    assert( x[ 0 ] == 1 );
    assert( x[ 1 ] == 0 );

    x[ 3 ] = 1;
    assert( x[ 0 ] == 1 );
    assert( x[ 1 ] == 0 );
    assert( x[ 2 ] == 0 );
    assert( x[ 3 ] == 1 );

    x[ 1 ] = 7;
    assert( x[ 0 ] == 1 );
    assert( x[ 1 ] == 7 );
    assert( x[ 2 ] == 0 );
    assert( x[ 3 ] == 1 );

    x[ 2 ] = 5;
    assert( x[ 0 ] == 1 );
    assert( x[ 1 ] == 7 );
    assert( x[ 2 ] == 5 );
    assert( x[ 3 ] == 1 );

    int i = 0;
    for ( int j : x )
        switch ( i++ )
        {
            case 0: assert( j == 1 ); break;
            case 1: assert( j == 7 ); break;
            case 2: assert( j == 5 ); break;
            case 3: assert( j == 1 ); break;
            default: assert( false ); break;
        }

    assert( y[ 3 ] == 0 );
    assert( y[ 4 ] == 0 );

    y[ 3 ] = 1;
    y[ 4 ] = 1;

    assert( y[ 3 ] == 1 );
    assert( y[ 4 ] == 1 );

    i = 0;
    for ( int j : y )
        if ( i++ < 3 ) assert( j == 0 );
        else assert( j == 1 );
    assert( i == 5 );

    return 0;
}
