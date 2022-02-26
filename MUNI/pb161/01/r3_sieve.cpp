#include <cassert>
#include <vector>

/* Implement the Sieve of Eratosthenes for quickly finding the largest prime
 * smaller than or equal to a given bound. */

int sieve( int bound );

int main()
{
    assert( sieve(  10 ) == 7   );
    assert( sieve(  13 ) == 13  );
    assert( sieve(  20 ) == 19  );
    assert( sieve( 200 ) == 199 );

    return 0;
}
