#include <vector>
#include <set>
#include <memory>
#include <cmath>
#include <cassert>

/* Lazy sequences, part two. */

/* Define a class template, ‹filter›, which holds two items:
 *
 *  • a «reference» to an arbitrary container,
 *  • a lambda ‹f› (of type ‹a → bool›).
 *
 * The constructor of ‹filter› should accept both, in this order. It
 * should be possible to use instances of ‹filter› in range ‹for›
 * loops: each element from the underlying container is first passed
 * to ‹f› and if the result is true, is returned to the user (via
 * the dereference operator), otherwise it is discarded. You may
 * want to review ‹map.cpp› in this unit and ‹filter.cpp› from week
 * 7. */

template< typename, typename >
struct filter_iterator; /* ref: 25 lines */

template< typename, typename >
struct filter; /* ref: 11 lines */

int main()
{
    std::vector v{ 1, 2, 3, 4 };
    std::set s{ 0.1, -1.0, 0.5 };
    std::set one{ 1 };
    std::vector two{ 2, 1 };

    auto is_odd = []( auto i ) { return i % 2 == 1; };
    auto is_neg = []( auto i ) { return i < 0; };
    auto is_odd_ptr = [&]( const auto &p ) { return is_odd( *p ); };

    int i = 0;

    for ( int h : filter( v, is_odd ) )
        assert( 1 + 2 * i++ == h );

    assert( i == 2 );
    i = 0;

    for ( int h : filter( s, is_neg ) )
        ++i, assert( h == -1.0 );

    assert( i == 1 );

    std::vector< std::unique_ptr< int > > tricky;
    tricky.emplace_back( new int( 7 ) );
    tricky.emplace_back( new int( 0 ) );

    i = 0;

    for ( const auto &h : filter( tricky, is_odd_ptr ) )
        ++i, assert( *h == 7 );

    assert( i == 1 );

    filter s_neg( s, is_neg );

    /* Check that ‹filter› holds the container by reference. */

    i = 0;

    for ( int h : s_neg )
        ++i, assert( h == -1.0 );

    assert( i == 1 );
    s.insert( -1 );

    for ( int h : s_neg )
        assert( ++i == 1 ? h == -1.0 : h == -1 );
    assert( i == 2 );

    /* check that throwing away everything works */

    for ( auto x : filter( one, is_neg ) )
        assert( x && false );

    for ( auto x : filter( two, is_odd ) )
        assert( x == 1 );

    return 0;
}
