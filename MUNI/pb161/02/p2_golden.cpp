#include <cmath>
#include <cassert>
#include <iostream>

/* The function ‹next_fib› should behave like this:
 * • given:   ‹a == fib( i )› and ‹b == fib( i + 1 )›
 * • execute: ‹next_fib( a, b )›
 * • to get:  ‹a == fib( i + 1 )› and ‹b == fib( i + 2 )›. */

void next_fib( int &a, int &b )
{
    int tmp = b + a;
    a = b;
    b = tmp;
}

/* «Optional»: Compute the n-th Fibonacci number using next_fib.
 * Make it so that: ‹fib( 1 ) == 1›, ‹fib( 2 ) == 1›, ‹fib( 3 ) ==
 * 2›. This is just to practice working with ‹next_fib› in case you
 * aren't sure. */

int fib( int n );

/* Approximate the golden ratio as the ratio of two consecutive
 * Fibonacci numbers. The ‹precision› argument gives an upper bound
 * on the approximation error. The number ‹rounds› is an output
 * parameter and gives us the number of iterations (calls to
 * ‹next_fib›) that were required to satisfy the precision
 * requirement.
 *
 * Notice that:
 *
 *   • the golden mean ⟦φ = 1.618⟧...
 *   • ‹fib( 2 ) / fib( 1 )› = 1 / 1 = 1     is a  lower bound
 *   • ‹fib( 3 ) / fib( 2 )› = 2 / 1 = 2     is an upper bound
 *   • ‹fib( 4 ) / fib( 3 )› = 3 / 2 = 1.5   is a  lower bound
 *   • ‹fib( 5 ) / fib( 4 )› = 5 / 3 = 1.667 is an upper bound
 *
 * and so on. Surely the error – distance from ⟦φ⟧ itself – in any
 * given round is smaller than its distance from the previous round.
 */

double golden( double precision, int &rounds )
{
    rounds = 0;
    double prev = 0.0, now = 0.0;
    int a = 1, b = 1;
    do
    {
        next_fib(a, b);
        prev = now;
        now = double(b) / double(a);
        ++rounds;
    } while (std::abs(now - prev) >= precision);
    return now;
}

int main()
{
    int a = 1, b = 1;

    next_fib( a, b ); assert( a == 1 && b == 2 );
    next_fib( a, b ); assert( a == 2 && b == 3 );
    next_fib( a, b ); assert( a == 3 && b == 5 );
    next_fib( a, b ); assert( a == 5 && b == 8 );

    double actual = ( 1 + std::sqrt( 5.L ) ) / 2;
    int rounds_prev = 0, rounds = 0;

    for ( double d = .1; d > 1e-10; d /= 10 )
    {
        assert( std::fabs( golden( d, rounds ) - actual ) <= d );
        assert( rounds <= 30 ); /* the sequence converges sufficiently quickly */
        assert( rounds >= rounds_prev );
        rounds_prev = rounds;
    }

    return 0;

    // clang-tidy: -cert-flp30-c,-clang-analyzer-security.FloatLoopCounter
}
