#include <vector>
#include <map>
#include <cassert>

/* The goal of this exercise is simple: take an oriented graph as
 * the input and produce a list (vector) of vertices in the
 * ‘leftmost’ DFS post-order. That is, visit the successors of a
 * vertex in order, starting from leftmost (different exploration
 * order will result in different post-orders). The graph is encoded
 * as a neighbourhood list. */

template< typename value_t >
using graph = std::map< value_t, std::vector< value_t > >;

/* Construct the post-order of ‹g› starting from vertex ‹i›. */
// … dfs_post( … g , … i );

int main()
{
    graph< int > gi{ { 1, { 2, 3, 4 } },
                     { 2, { 1, 2 } },
                     { 3, { 3, 4 } },
                     { 4, {} },
                     { 5, { 3 } } };

    auto o = []( auto... v ) { return std::vector< int >{ v... }; };
    assert( dfs_post( gi, 5 ) == o( 4, 3, 5 ) );
    assert( dfs_post( gi, 2 ) == o( 4, 3, 1, 2 ) );
    assert( dfs_post( gi, 1 ) == o( 2, 4, 3, 1 ) );

    using color = std::tuple< short, short, short >;
    color red( 255, 0, 0 ), green( 0, 255, 0 ), blue( 0, 0, 255 ),
          yellow( 255, 255, 0 ), purple( 255, 0, 255 );

    graph< color > gc{ { red, { blue, green, yellow } },
                       { blue, { red, blue } },
                       { green, { green, yellow } },
                       { yellow, {} },
                       { purple, { green } } };

    std::vector ygp{ yellow, green, purple };
    std::vector ygrb{ yellow, green, red, blue };
    std::vector bygr{ blue, yellow, green, red };

    assert( dfs_post( gc, purple ) == ygp );
    assert( dfs_post( gc, blue ) == ygrb );
    assert( dfs_post( gc, red ) == bygr );

    return 0;
}
