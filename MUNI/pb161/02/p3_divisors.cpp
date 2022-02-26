#include <vector>
#include <algorithm>
#include <cassert>

/* Take a number, find all its «prime» divisors and add them into
 * ‹divs›, unless they are already there. Be sure to do this in time
 * proportional (linear) to the input number.
 *
 * «Bonus»: If you assume that ‹divs› is sorted in ascending order
 * when you get it, you can make ‹add_divisors› a fair bit more
 * efficient. Can you figure out how? */

bool isprime(int i, std::vector< int > &divs)
{
    for(int c : divs) if(i % c == 0) return false;
    return true;
}

void add_divisors( int num, std::vector< int > &divs ){
    for(int i = 2; i <= num; ++i)
    {
        if(num % i == 0 && isprime(i, divs)) divs.push_back(i);
    }
}

int main()
{
    std::vector< int > divs;

    add_divisors( 7, divs );
    assert( divs.size() == 1 );
    assert( divs[ 0 ] == 7 );

    add_divisors( 14, divs );
    std::sort( divs.begin(), divs.end() );
    assert( divs.size() == 2 );
    assert( divs[ 0 ] == 2 );
    assert( divs[ 1 ] == 7 );

    add_divisors( 32, divs );
    std::sort( divs.begin(), divs.end() );
    assert( divs.size() == 2 );
    assert( divs[ 0 ] == 2 );
    assert( divs[ 1 ] == 7 );

    add_divisors( 18, divs );
    std::sort( divs.begin(), divs.end() );
    assert( divs.size() == 3 );
    assert( divs[ 0 ] == 2 );
    assert( divs[ 1 ] == 3 );
    assert( divs[ 2 ] == 7 );

    return 0;
}
