#include <cassert>
#include <vector>
#include <set>
#include <string_view>

/* Write a function which takes two sequences, ‹a› and ‹b›, and
 * produces a single vector with values from the two sequences
 * (first all values from ‹a›, then all values from ‹b›, preserving
 * their order). Assume each sequence has a nested typedef
 * ‹value_type›. The sequences do not need to be of the same type,
 * but their values must be compatible. */

// … concat( … a, … b )

int main()
{
    std::string_view he( "he" ), llo( "llo" );
    std::vector hello{ 'h', 'e', 'l', 'l', 'o' };
    std::vector i12{ 1, 2 }, i345{ 3, 4, 5 },
                i12345{ 1, 2, 3, 4, 5 };
    std::set s12{ 1, 2 };

    assert( concat( he, llo ) == hello );
    assert( concat( i12, i345 ) == i12345 );
    assert( concat( s12, i345 ) == i12345 );

    return 0;
}
