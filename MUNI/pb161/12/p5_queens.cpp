#include <vector>
#include <utility>
#include <cassert>

/* Write a function that checks whether a given configuration of
 * 8 queens on a chessboard is such that no two queens endanger each
 * other. */

/* The first number is the column numbered from left, a = 1, b = 2,
 * ...; second is the row (likewise indexed from 1, starting at
 * bottom): { 1, 1 } is the bottom left corner. */

using position = std::pair< int, int >;
using queens = std::vector< position >;

/* Return true if all queens are safe. */

bool check( const queens &q ); /* ref: 43 lines */

int main()
{
    queens q{ { 6, 8 }, { 4, 7 }, { 7, 6 }, { 1, 5 },
              { 8, 4 }, { 2, 3 }, { 5, 2 }, { 3, 1 } };
    queens k{ { 2, 1 }, { 5, 2 }, { 7, 3 }, { 4, 4 },
              { 1, 5 }, { 8, 6 }, { 6, 7 }, { 3, 8 } };
    queens l{ { 1, 2 }, { 2, 4 }, { 3, 6 }, { 4, 8 },
              { 5, 3 }, { 6, 1 }, { 7, 7 }, { 8, 5 } };

    assert( check( q ) );
    assert( check( k ) );
    assert( check( l ) );

    q[ 0 ].first ++;
    k[ 2 ].second ++;
    std::swap( l[ 0 ].first, l[ 2 ].first );

    assert( !check( q ) );
    assert( !check( k ) );
    assert( !check( l ) );

    return 0;
}
