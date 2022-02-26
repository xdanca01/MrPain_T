#include <cassert>

/* In this exercise, we will implement complex numbers with
 * addition, subtraction, unary minus and equality. */

/* The class should be called ‹complex› (do not mind the syntax
 * highlight). The constructor should take 2 real numbers (the real
 * and imaginary parts). */

class complex;

int main()
{
    complex zero( 0, 0 ), one( 1, 0 ), i( 0, 1 );
    const complex minus_one = -one, minus_i = -i;

    assert( i + minus_i == zero );
    assert( one - i == complex( 1, -1 ) );
    assert( i + i == complex( 0, 2 ) );
    assert( minus_i + i == zero );
    assert( -minus_i == i );
    assert( -minus_one == one );

    return 0;
}
