#include <vector>
#include <set>
#include <memory>
#include <cmath>
#include <cassert>

/* Lazy sequences, part three. */

/* Define a class template, ‹zip›, which holds 2 references to
 * arbitrary containers, possibly of different types.
 *
 * The constructor of ‹zip› should accept both (via ‹const›
 * references). It should be possible to use instances of ‹zip› in
 * range ‹for› loops: in each iteration, the ‹zip› iterator fetches
 * a single elemnt from each of the two containers and returns them
 * as a 2-tuple of ‹const› references. The iteration ends when the
 * shorter of the two sequences runs out of elements.
 *
 * Hint: to create a tuple of references, use ‹std::tie›. */

template< typename, typename >
struct zip_iterator; /* ref: 31 lines */

template< typename, typename >
struct zip; /* ref: 11 lines */

int main()
{
    std::vector v{ 1, 2, 3, 4 };
    std::set s{ 1, 3, 5 };
    std::vector< std::set< int > > empty;

    int i = 0;

    for ( auto [ x, y ] : zip( v, s ) )
        switch ( ++i )
        {
            case 1: assert( x == 1 && y == 1 ); break;
            case 2: assert( x == 2 && y == 3 ); break;
            case 3: assert( x == 3 && y == 5 ); break;
            default: assert( false );
        }

    assert( i == 3 );

    i = 0;

    for ( auto [ x, y ] : zip( s, s ) )
        switch ( ++i )
        {
            case 1: assert( x == 1 && y == 1 ); break;
            case 2: assert( x == 3 && y == 3 ); break;
            case 3: assert( x == 5 && y == 5 ); break;
            default: assert( false );
        }

    assert( i == 3 );

    std::vector< std::unique_ptr< int > > tricky;
    tricky.emplace_back( new int( 7 ) );
    tricky.emplace_back( new int( 0 ) );

    i = 0;

    for ( const auto &[ p, y ] : zip( tricky, v ) )
        switch ( ++i )
        {
            case 1: assert( *p == 7 && y == 1 ); break;
            case 2: assert( *p == 0 && y == 2 ); break;
            default: assert( false );
        }

    assert( i == 2 );

    i = 0;

    for ( const auto &[ p, q ] : zip( tricky, tricky ) )
        switch ( ++i )
        {
            case 1: assert( *p == 7 && *q == 7 ); break;
            case 2: assert( *p == 0 && *q == 0 ); break;
            default: assert( false );
        }

    assert( i == 2 );

    for ( const auto &unused : zip( tricky, empty ) )
    {
        static_cast< void >( unused );
        assert( false );
    }

    return 0;
}
