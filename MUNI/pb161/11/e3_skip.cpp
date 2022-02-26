#include <cassert>
#include <vector>
#include <set>

/* Write a class template ‹skip› which will be like a view, but
 * allow us to iterate every n-th item (a stride) of a given
 * iterator range, instead of every item. Make sure that if the
 * stride does not evenly divide the sequence length, iteration
 * still works correctly.
 *
 * Hint: please note that this class is significantly more
 * complicated than ‹view› was. You might find ‹decltype( auto )›
 * useful, particularly as the return type of a function. */

int main()
{
    std::vector vec{ 1, 2, 3, 4, 5, 6, 7, 8 };
    std::set    set{ 1, 2, 3, 4, 5, 6, 7, 8 };

    std::vector< std::pair< int, int > > vp
    {
        { 1, 1 }, { 1, 2 }, { 1, 3 },
        { 2, 1 }, { 2, 2 }, { 2, 3 },
        { 3, 1 }, { 3, 2 }, { 3, 3 }
    };

    auto check = []( auto seq, int f, int t, int step )
    {
        int i = f;
        auto b = std::next( seq.begin(), f - 1 );
        auto e = std::prev( seq.end(), 8 - t );

        for ( int j : skip( b, e, step ) )
        {
            if ( i != j )
                return false;
            i += step;
        }

        return i == t + step;
    };

    assert( check( vec, 1, 7, 2 ) );
    assert( check( vec, 2, 6, 2 ) );
    assert( check( vec, 1, 3, 2 ) );
    assert( check( set, 3, 6, 3 ) );
    assert( check( set, 2, 8, 3 ) );

    int i = 1;

    for ( auto [ x, y ] : skip( vp.begin(), vp.end(), 3 ) )
    {
        assert( x == i++ );
        assert( y == 1 );
    }

    assert( i == 4 );

    i = 1;
    int j = 1;

    for ( auto [ x, y ] : skip( vp.begin(), vp.end(), 4 ) )
    {
        assert( x == i++ );
        assert( y == j++ );
    }

    assert( i == 4 );

    skip v( vec.begin() + 1, vec.end(), 3 );
    auto iter = v.begin();
    *iter++ = 3;
    *iter++ = 7;
    assert( vec[ 1 ] == 3 );
    assert( vec[ 4 ] == 7 );

    return 0;
}
