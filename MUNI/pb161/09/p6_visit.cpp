#include <vector>
#include <map>
#include <set>
#include <cassert>

/* The input graph is given using adjacency lists: the ‹graph› type
 * gives the successors for each vertex present in the graph. */

template< typename vertex_t >
using graph = std::map< vertex_t, std::vector< vertex_t > >;

/* Visit each vertex of graph ‹g› reachable from ‹initial› once and
 * call ‹f› on its value. The order of calls is not important. */

// void visit( … g, … f, … initial );

int main()
{
    graph< int > gi{ { 1, { 2, 3, 4 } },
                     { 2, { 1, 2 } },
                     { 3, { 3, 4 } },
                     { 4, {} },
                     { 5, { 3 } } };

    auto collect = [&]( auto g, auto i )
    {
        std::set< decltype( i ) > visited;
        auto stash = [&]( auto v )
        {
            assert( visited.count( v ) == 0 );
            visited.insert( v );
        };
        visit( g, stash, i );

        return visited;
    };

    std::set s345{ 3, 4, 5 },
             s1234{ 1, 2, 3, 4 };

    assert( collect( gi, 5 ) == s345 );
    assert( collect( gi, 2 ) == s1234 );
    assert( collect( gi, 1 ) == s1234 );

    using color = std::tuple< short, short, short >;

    color red( 255, 0, 0 ),
          green( 0, 255, 0 ),
          blue( 0, 0, 255 ),
          yellow( 255, 255, 0 ),
          purple( 255, 0, 255 );

    graph< color > gc{ { red, { blue, green, yellow } },
                       { blue, { red, blue } },
                       { green, { green, yellow } },
                       { yellow, {} },
                       { purple, { green } } };

    std::set gyp{ green, yellow, purple };
    std::set rgby{ red, green, blue, yellow };

    assert( collect( gc, purple ) == gyp );
    assert( collect( gc, blue ) == rgby );
    assert( collect( gc, red ) == rgby );

    return 0;
}
