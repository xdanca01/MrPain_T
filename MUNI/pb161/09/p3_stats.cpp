#include <utility>
#include <vector>
#include <forward_list>
#include <set>
#include <cassert>

/* In this exercise, we will do some basic statistics: median,
 * quartiles and mode. */

/* Implement the functions ‹mode›, ‹median› and ‹quartiles›, in such
 * a way that it accepts any sequential ‹std› container, with
 * element type that allows less-than and equality comparison.
 * Additional notes:
 *
 *  • ‹mode› returns an ‹std::set› of numbers, since there might be
 *    arbitrarily many: include any input number for which the number
 *    of occurrences is maximal
 *  • ‹median› return a single number; pick the smaller of the two
 *    elements if the median lies in between two different numbers
 *  • ‹quartiles› returns numbers at indices (size / 4) and ( ( 3 *
 *    size ) / 4 ) of the sorted input sequence, in an ‹std::pair›
 *    [this is slightly incorrect but simpler]. */

// mode:      ref. 15 lines
// median:    ref.  7 lines
// quartiles: ref.  8 lines

int main()
{
    std::forward_list< int > fl;
    fl.push_front( 1 );
    fl.push_front( 3 );
    fl.push_front( 5 );
    fl.push_front( 3 );

    assert( median( fl ) == 3 );
    assert( mode( fl ) == std::set{ 3 } );
    assert( quartiles( fl ) == std::pair( 3, 5 ) );

    std::set< std::pair< int, int > > s;
    s.emplace( 1, 0 );
    s.emplace( 2, 0 );
    s.emplace( 2, 1 );
    s.emplace( 3, 1 );
    s.emplace( 4, 0 );
    assert( mode( s ) == s );
    assert( median( s ) == std::pair( 2, 1 ) );
    assert( quartiles( s ) == std::pair( std::pair( 2, 0 ),
                                         std::pair( 3, 1 ) ) );

    std::vector< double > v;
    v.push_back( 3.14 );
    v.push_back( 3.14 );
    v.push_back( 2.0 );
    v.push_back( 2.0 );
    v.push_back( 2.71 );

    assert( mode( v ) == ( std::set{ 2.0, 3.14 } ) );
    assert( median( v ) == 2.71 );
    assert( quartiles( v ) == std::pair( 2.0, 3.14 ) );

    return 0;
}
