#include <set>
#include <vector>
#include <cassert>

/* Decompose an undirected graph into connected components
 * (described by a set of sets of numbers). The graph is given as a
 * symmetric adjacency matrix. Vertices are numbered from 1 to ⟦n⟧
 * where ⟦n⟧ is the dimension of the input matrix. */

using graph = std::vector< std::vector< bool > >;

using component = std::set< int >;
using components = std::set< component >;

components decompose( const graph &g );

int main()
{
    graph g_1{ { 0, 1, 1, 0 },
               { 1, 0, 0, 0 },
               { 1, 0, 0, 0 },
               { 0, 0, 0, 0 } },

          g_2{ { 0, 1, 0, 0 },
               { 1, 0, 1, 0 },
               { 0, 1, 0, 1 },
               { 0, 0, 1, 0 } },

          g_3{ { 0, 1, 0, 0 },
               { 1, 0, 0, 0 },
               { 0, 0, 0, 1 },
               { 0, 0, 1, 0 } },

          g_4{ { 0, 0, 0, 0 },
               { 0, 0, 0, 1 },
               { 0, 0, 0, 0 },
               { 0, 1, 0, 0 } },

          g_5{ { 0, 0, 0, 0 },
               { 0, 0, 0, 0 },
               { 0, 0, 0, 0 },
               { 0, 0, 0, 0 } },

          g_6{ { 0, 1, 1, 0 },
               { 1, 0, 1, 0 },
               { 1, 1, 0, 0 },
               { 0, 0, 0, 0 } },

          g_7{ { 0, 1, 1, 1 },
               { 1, 0, 1, 1 },
               { 1, 1, 0, 1 },
               { 1, 1, 1, 0 } };

    components c_1{ { 1, 2, 3 }, { 4 } },
               c_2{ { 1, 2, 3, 4 } },
               c_3{ { 1, 2 }, { 3, 4 } },
               c_4{ { 1 }, { 2, 4 }, { 3 } },
               c_5{ { 1 }, { 2 }, { 3 }, { 4 } },
               c_6{ { 1, 2, 3 }, { 4 } },
               c_7{ { 1, 2, 3, 4 } };

    assert( decompose( g_1 ) == c_1 );
    assert( decompose( g_2 ) == c_2 );
    assert( decompose( g_3 ) == c_3 );
    assert( decompose( g_4 ) == c_4 );
    assert( decompose( g_5 ) == c_5 );
    assert( decompose( g_6 ) == c_6 );
    assert( decompose( g_7 ) == c_7 );

    return 0;

    // clang-tidy: -modernize-use-bool-literals
}
