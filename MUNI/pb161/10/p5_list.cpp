#include <cassert>
#include <memory>

/* In this exercise, we will define a ‹list› class that behaves like
 * the lists in functional programming: the values and structure
 * will be immutable, but it'll be possible to fairly cheaply create
 * new lists by prepending values to existing lists. */

/* Define a class template ‹list› with a singe type parameter ‹T›,
 * with the following interface (all methods are ‹const›):
 *
 *  • ‹head› returns the value of the current list
 *  • ‹tail› returns the remainder of the list as another ‹list›
 *    instance, without copying any values
 *  • ‹empty› which returns ‹true› if the list is empty
 *  • a default constructor which creates an empty list (i.e. the
 *    ‹[]› constructor you know from Haskell)
 *  • a 2-parameter constructor which takes the value and the tail
 *    of the list (i.e. the ‹(:)› constructor).
 *
 * Hint: It is preferable to store the values «inline» in the nodes.
 * You should also use 2 data types, one for the list itself and
 * another for nodes: this will make it easier to implement empty
 * lists and in general make the implementation nicer. */

template< typename T >
class list; /* ref: 24 lines */

int main()
{
    using ilist = list< int >;
    using ulist = list< std::unique_ptr< int > >;
    auto mku = []( int v ) { return std::make_unique< int >( v ); };

    ilist i( 1, ilist( 2, ilist( 3, ilist() ) ) );
    ulist u( mku( 1 ), ulist( mku( 2 ), ulist() ) );

    assert( i.head() == 1 );
    assert( i.tail().head() == 2 );
    assert( i.tail().tail().head() == 3 );
    assert( i.tail().tail().tail().empty() );

    auto j = i.tail();
    assert( i.tail().head() == j.head() );

    assert( *u.head() == 1 );
    assert( *u.tail().head() == 2 );
    assert( u.tail().tail().empty() );

    auto v = u.tail();
    assert( *v.head() == 2 );

    return 0;
}
