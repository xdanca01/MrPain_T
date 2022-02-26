#include <cassert>
#include <vector>

/* Goal: build a tree by preserving the structure of an existing
 * tree, but obtain new values by applying a given function to the
 * originals. The type of the value may change. Hint: assuming
 *
 *  • ‹function_t fun› is a unary function,
 *  • ‹value_t val› is a value such that ‹fun( val )› makes sense,
 *
 * you can use:
 *
 *  • ‹std::invoke_result_t< function_t, value_t >› to obtain the
 *    type of ‹fun( val )› without having either ‹fun› or ‹val› (only
 *    their types). */

template< typename value_t >
struct tree
{
    value_t value;
    std::vector< tree > children;

    tree( value_t v, std::vector< tree > ch = {} )
        : value( std::move( v ) ), children( std::move( ch ) )
    {}
};

/* Build a tree of a suitable type given a function ‹f› which maps
 * values to values and some tree ‹t›, compatible with ‹f›. */

// … tmap( … f, … t )

int main()
{
    auto half = []( int x ) { return double( x ) / 2; };
    auto dbl  = []( double x ) { return int( x * 2 ); };

    tree t_in( 7, { tree( 1 ), tree( 2 ) } );
    auto t_out = tmap( half, t_in );

    assert( t_out.value == 3.5 );
    assert( t_out.children.size() == 2 );
    assert( t_out.children[ 0 ].value == 0.5 );
    assert( t_out.children[ 1 ].value == 1 );

    auto t_back = tmap( dbl, t_out );

    assert( t_back.value == 7 );
    assert( t_back.children.size() == 2 );
    assert( t_back.children[ 0 ].value == 1 );
    assert( t_back.children[ 1 ].value == 2 );

    return 0;
}
