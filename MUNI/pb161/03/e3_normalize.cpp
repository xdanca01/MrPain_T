#include <vector>
#include <cassert>

/* Given a vector of non-negative floating-point numbers, produce a
 * new vector where all entries fall into the 0-1 range, and they
 * are all related to the original entries by the same factor. */

using signal_t = std::vector< double >;

signal_t normalize( const signal_t & );

int main()
{
    signal_t s_1{ 1, 2, 1, 4 },
             s_2{ 0.25, 0.5, 0.25, 1 },
             s_3{ 0, 1, 0, 2, 0, 1 },
             s_4{ 0, 0.5, 0, 1, 0, 0.5 },
             s_5{ 0, 0, 0 };

    assert( normalize( s_1 ) == s_2 );
    assert( normalize( s_2 ) == s_2 );
    assert( normalize( s_3 ) == s_4 );
    assert( normalize( s_4 ) == s_4 );
    assert( normalize( s_5 ) == s_5 );

    return 0;
}
