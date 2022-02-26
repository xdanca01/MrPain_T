#include <vector>
#include <cassert>

std::vector< int > digits( int n, int base );

int main()
{
    std::vector< int > d10_1337 = { 1, 3, 3, 7 },
                       d10_123  = { 1, 2, 3 },
                       d7_1234  = { 3, 4, 1, 2 },
                       d2_9     = { 1, 0, 0, 1 };
    assert( digits( 1337, 10 ) == d10_1337 );
    assert( digits( 123,  10 ) == d10_123 );
    assert( digits( 1234,  7 ) == d7_1234 );
    assert( digits( 9, 2 ) == d2_9 );

    return 0;
}
