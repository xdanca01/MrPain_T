#include <memory>
#include <vector>
#include <set>
#include <cassert>

/* We have mostly ignored the question of ownership when we worked
 * with on-the-fly `map` and `filter` implementations in week 9: it
 * was up to the user to ensure that the underlying container
 * outlives the range object. However, we may sometimes want to be
 * able to return such mapped ranges from functions which construct
 * the underlying data, and this does not work in our previous
 * model. Let's try a different approach then. */

/* Define a class template ‹range› which takes a «container» as a
 * template argument. The class should offer the following interface
 * (all methods are ‹const›):
 *
 *  • construction from a container, with argument template
 *    deduction (we will make a copy of the container: in real world,
 *    we would be able to avoid doing that),
 *  • iteration interface: ‹begin› and ‹end› which return suitable
 *    iterators (usable in range ‹for› at minimum),
 *  • ‹take› and ‹drop› which construct a new ‹range› object that
 *    shares the backing data with the current one, but offer a
 *    reduced view of it (‹take› reduces the view to first N
 *    elements, while ‹drop› removes the first N elements from the
 *    view),
 *  • an «element-wise» equality comparison operator (we want this
 *    to work with ranges backed by different containers, so you
 *    will need to implement this operator as a template). */

template< typename container_t >
class range;

int main()
{
    std::vector< std::unique_ptr< int > > uniq;

    auto vec_range = []( auto... elems )
    {
        return range( std::vector{ elems...} );
    };

    uniq.emplace_back( new int( 3 ) );
    uniq.emplace_back( new int( 1 ) );
    uniq.emplace_back( new int( 7 ) );

    std::set s_set{ 1, 2, 3 };

    range r = vec_range( 1, 2, 3 );
    range s( s_set );
    range t( std::move( uniq ) );

    assert( s == r );
    assert( r.take( 1 ) == vec_range( 1 ) );
    assert( r.drop( 1 ) == vec_range( 2, 3 ) );
    assert( s.take( 2 ) == vec_range( 1, 2 ) );

    int count = 0;

    for ( const auto &i : t.take( 1 ) )
        ++ count, assert( *i == 3 );

    assert( count == 1 );

    return 0;
}
