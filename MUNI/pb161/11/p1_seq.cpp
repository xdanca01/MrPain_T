#include <cassert>

/* In this exercise, the goal will be to implement a class template
 * which will allow us to iterate over a sequence of number-like
 * objects. */

/* The ‹seq› class should be constructible from 2 number-like
 * objects: the initial value (included) and the final value
 * (excluded). Use pre-increment (operator ‹++›) and equality
 * (operator ‹==›) to generate the values. The dereference operator
 * should return the generated objects by value. */

template< typename T >
class seq_iterator; /* ref: 10 lines */

template< typename T >
class seq; /* ref: 9 lines */

/* A ‹nat› implementation for testing purposes. */

struct nat
{
    int v;
    nat( int v ) : v( v ) {}
    bool operator==( nat o ) const { return v == o.v; }
    nat &operator++() { ++v; return *this; }
};

int main()
{
    int i = 1;
    for ( int j : seq( 1, 10 ) )
        assert( j == i++ );
    assert( i == 10 );

    i = 0;
    for ( double x : seq( 0.2, 2.2 ) )
        switch ( i++ )
        {
            case 0: assert( x == 0.2 ); break;
            case 1: assert( x == 1.2 ); break;
            case 2: assert( x == 2.2 ); break;
            default: assert( false );
        }
    assert( i == 2 );

    i = 1;
    for ( nat j : seq( nat( 1 ), nat( 10 ) ) )
        assert( j.v == i++ );
    assert( i == 10 );

    return 0;
}
