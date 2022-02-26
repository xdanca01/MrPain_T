#include <vector>
#include <cmath>
#include <cassert>

/* Besides simple output arguments, like in the previous demo, we
 * can pass values out of functions by manipulating existing
 * objects, most straightforwardly containers. In this demo, we will
 * write a function ‹primes› which appends prime numbers from a
 * given range to an existing ‹std::vector›. We will still call
 * ‹out› an «output argument», though the concept is clearly more
 * nuanced here. Like before, we will use a mutable reference to
 * achieve the desired semantics. */

void primes( int from, int to, std::vector< int > &out )
{
    for ( int candidate = from; candidate < to; ++ candidate )
    {
        bool prime = true;
        int bound = std::sqrt( candidate ) + 1;

        /* Decide whether a given number is prime, naively, by trial
         * division. */

        for ( int div = 2; div < bound; ++ div )
            if ( div != candidate && candidate % div == 0 )
            {
                prime = false;
                break;
            }

        /* Now the interesting part: if the number was found to be
         * prime, we append it to the object referenced by ‹out›
         * (i.e. the original object which was declared outside this
         * function and passed into it by reference). Below in
         * ‹main›, you can see that the content of the vector
         * ‹p_out› changes when we call this function on it. */

        if ( prime )
            out.push_back( candidate );
    }
}

int main() /* demo */
{
    std::vector< int > p_out;
    std::vector< int > p7 = { 2, 3, 5 },
                       p15 = { 2, 3, 5, 7, 11, 13 };

    primes( 2, 7, p_out );
    assert( p_out == p7 );
    primes( 7, 15, p_out );
    assert( p_out == p15 );
}
