#include <vector>
#include <set>
#include <memory>
#include <cmath>
#include <cassert>

/* Lazy sequences, part one. */

/* Define a class template, ‹map›, which holds two items:
 *
 *  • a reference to an arbitrary container,
 *  • a lambda ‹f› (of type ‹a → b›).
 *
 * The constructor of ‹map› should accept both (via ‹const›
 * references), in this order. It should be possible to use
 * instances of ‹map› in range ‹for› loops: each element from the
 * underlying container is first passed to ‹f› and the result of
 * that is returned to the user (via the dereference operator).
 *
 * Hint: the type of the iterator that ‹const› versions of ‹begin›
 * and ‹end› return is available in standard containers as a «nested
 * type», like this: ‹std::vector< int >::const_iterator›. */

int main()
{
    std::vector v{ 1, 2, 3, 4 };
    std::set s{ 0.1, -0.9, 0.5 };

    int i = 0;

    /* Mapping integers to integers. */

    for ( int h : map( v, []( auto i ) { return i / 2; } ) )
        assert( ++i / 2 == h );

    std::vector expect{ 'a', 'b', 'c', 'd' };
    i = 0;

    /* Mapping ‹int› to ‹char›. */

    for ( int h : map( v, []( auto i ) { return 'a' + i - 1; } ) )
        assert( expect[ i++ ] == h );

    std::vector< std::unique_ptr< int > > tricky;
    tricky.emplace_back( new int( 7 ) );
    tricky.emplace_back( new int( 0 ) );

    i = 0;

    /* Let's make sure that ‹map› is not making unnecessary copies
     * of elements (or even worse, of the entire container). */

    auto deref = []( const auto &i ) { return *i; };

    for ( int h : map( tricky, deref ) )
        assert( h == ( i++ == 0 ? 7 : 0 ) );

    /* And also that the results of the lambda passed to ‹map› are
     * not stored anywhere, but immediately returned. */

    auto mkuniq = []( int i )
    {
        return std::make_unique< int >( i );
    };

    i = 1;

    for ( auto h : map( v, mkuniq ) )
        assert( *h == i++ );

    /* Finally check that ‹map› also works on ‹std::set› as the
     * underlying container. */

    auto square = []( double i ) { return std::pair( i, i * i ); };

    for ( auto [ i, s ] : map( s, square ) )
    {
        assert( std::fabs( i ) > s );
        assert( s > 0 );
    }

    return 0;
}
