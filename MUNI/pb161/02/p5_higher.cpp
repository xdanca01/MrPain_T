#include <vector>
#include <cassert>

/* Write a map function, which takes a function ‹f› and a vector ‹v› and
 * returns a new vector ‹w› such that w[ i ] = f( v[ i ] ) for any valid index
 * ‹i›. We will need to use the ‘lambda’ syntax for this, since we don't yet
 * know any other way to write functions which accept functions as arguments. */

// static auto map = []( ... ) { ... };

/* Similar, but ‹f› is a binary function, and there are two input vectors of
 * equal length. You do not need to check this. */

// static auto zip = []( ... ) { ... };

/* You can assume that the output vector is of the same type as the input
 * vector (i.e. ‹f› is of type a → a in map, and of type a → b → a for ‹zip›. */

int main()
{
    using intv = std::vector< int >;

    intv a{ 1, 2, 3, 4, 5 };

    intv b = map( []( int x ) { return 2 * x; }, a );
    intv c = map( []( int x ) { return x * x; }, a );
    intv d = zip( []( int x, int y ) { return x * y; }, a, a );

    assert( b == ( intv{ 2, 4, 6, 8, 10 } ) );
    assert( c == ( intv{ 1, 4, 9, 16, 25 } ) );
    assert( d == ( intv{ 1, 4, 9, 16, 25 } ) );

    return 0;
}
