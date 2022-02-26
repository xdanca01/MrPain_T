#include <cassert>
#include <vector>
#include <set>

/* Write a class template ‹view› which will allow us to bundle a
 * pair of iterators of an existing sequence and use them as a
 * virtual sequence in its own right. It should be possible to
 * change the underlying sequence through the view. The constructor
 * should accept two iterators, ‹start› and ‹end›, and iterating the
 * resulting sequence should include everything in this range (‹end›
 * is excluded, as is customary). */

// … class view;

int main()
{
    std::vector vec{ 1, 2, 3, 4 };
    std::set    set{ 1, 2, 3, 4 };

    auto check = []( auto seq, int f, int t )
    {
        int i = f;
        auto b = std::next( seq.begin(), f - 1 );
        auto e = std::prev( seq.end(), 4 - t );

        for ( int j : view( b, e ) )
            if ( i++ != j )
                return false;
        return i == t + 1;
    };

    assert( check( vec, 1, 4 ) );
    assert( check( vec, 1, 3 ) );
    assert( check( vec, 1, 2 ) );
    assert( check( vec, 2, 4 ) );
    assert( check( vec, 2, 3 ) );
    assert( check( set, 1, 2 ) );
    assert( check( set, 2, 4 ) );

    view v( vec.begin() + 1, vec.end() );
    *v.begin() = 3;
    assert( vec[ 1 ] == 3 );

    return 0;
}
