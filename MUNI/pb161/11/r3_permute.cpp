#include <cassert>

/* Implement class ‹permutations›, with a constructor which accepts
 * an ‹std::vector› of ‹int› and which represents a sequence of all
 * distinct permutations of the input vector. Iterating an instance
 * of ‹permutations› should yield values which can be both iterated
 * and indexed, yielding, in turn, integers. The ‹permutations›
 * object itself does not need to be indexable. For example:
 *
 *     std::vector vec{ 1, 3, 2 };
 *
 *     for ( auto p : permutations( vec ) )
 *         for ( int v : p )
 *             std::cerr << v << " ";
 *
 * The first permutation should be sorted in ascending order. The
 * «sequence of permutations» as a whole should be sorted in
 * lexicographic order (as a consequence of this, the last
 * permutation should be sorted in descending order). The output of
 * the above program, therefore, should be: 1 2 3 1 3 2 2 1 3 2 3 1
 * 3 1 2 3 2 1. */

int main()
{
    /* helper functions */

    auto lt = []( const auto &a, const auto &b )
    {
        auto i = a.begin();
        auto j = b.begin();
        while ( i != a.end() && j != b.end() )
        {
            if ( *i > *j )
                return false;
            if ( *i < *j )
                return true;
            ++ i, ++ j;
        }
        assert( i == a.end() && j == b.end() );
        return false;
    };

    auto eq = [&]( const auto &a, const auto &b )
    {
        return !lt( a, b ) && !lt( b, a );
    };

    auto v = []( auto... ns ) { return std::vector{ ns... }; };
    auto next = []( auto i, int n ) { while ( n --> 0 ) ++i ; return i; };
    auto dist = []( auto b, auto e )
    {
        int count = 0;
        while ( b != e ) ++ b, ++ count;
        return count;
    };

    /* test data */
    std::vector vec{ 1, 3, 2 };
    permutations p( vec );
    vec.push_back( 7 );
    const permutations q( vec );

    /* the number of iterations is correct */
    assert( dist( p.begin(), p.end() ) == 6 );
    assert( dist( q.begin(), q.end() ) == 24 );

    /* the first and last permutations are sorted */
    assert( eq( *p.begin(), v( 1, 2, 3 ) ) );
    assert( eq( *q.begin(), v( 1, 2, 3, 7 ) ) );
    assert( eq( *next( p.begin(), 5 ) , v( 3, 2, 1 ) ) );
    assert( eq( *next( q.begin(), 23 ), v( 7, 3, 2, 1 ) ) );

    /* multiple independent iterators */
    auto i = p.begin(), j = i;
    ++ j;
    assert( lt( *i, *j ) );

    /* ordering */
    assert( lt( *next( p.begin(), 0 ), *next( p.begin(), 1 ) ) );
    assert( lt( *next( p.begin(), 3 ), *next( p.begin(), 4 ) ) );

    /* simple cases */
    vec.clear();
    const permutations empty( vec );
    assert( empty.begin() == empty.end() );
    vec.push_back( 1 );
    const permutations one( vec );
    assert( dist( one.begin(), one.end() ) == 1 );
    vec.push_back( 1 );
    const permutations repeated( vec );
    assert( dist( repeated.begin(), repeated.end() ) == 1 );

    return 0;
}
