#include <cassert>

/* A quotient field is a generalization of rational numbers: one can
 * be constructed from any «integral domain». When the integral
 * domain is taken to be Z (the integers), the result is Q (the
 * rational numbers). However, the construction is much more general
 * and can be applied to polynomials, Gaussian integers, p-adic
 * numbers and so on. Here, we will construct standard rationals and
 * Gaussian rationals (which are like normal rationals, but with an
 * imaginary part). */

/* Define a «class template» ‹rat›. The type parameter will provide
 * the integral domain: ‹int› for integers, ‹gauss› for Gaussian
 * integers. The constructor should take the numerator and the
 * denominator as arguments. Define addition, multiplication and
 * division on ‹rat›'s, as well as equality. */

/* When done, implement ‹gauss›, which is simply a complex number
 * where both the real and imaginary parts are integers.  Store them
 * in algebraic form for simplicity. Define addition, multiplication
 * and equality. */

int main()
{
    rat q_zero( 0, 1 ), q_one( 1, 1 ), q_half( 1, 2 );

    assert( q_zero + q_one == q_one );
    assert( q_zero + q_zero == q_zero );
    assert( q_half + q_half == q_one );
    assert( q_half * q_one == q_half );
    assert( q_half * q_half == rat( 1, 4 ) );
    assert( q_half / q_half == q_one );

    gauss gi_zero( 0, 0 ), gi_one( 1, 0 ), gi_i( 0, 1 ),
          gi_four( 4, 0 );

    assert( gi_one + gi_zero == gi_one );
    assert( gi_one + gi_i == gauss( 1, 1 ) );
    assert( gi_i * gi_i == gauss( -1, 0 ) );
    assert( gi_i * gi_one == gi_i );

    rat gr_zero( gi_zero, gi_one ),
        gr_one( gi_one, gi_one ),
        gr_half( gi_one, gi_one + gi_one ),
        gr_i( gi_i, gi_one ),
        gr_minusi( gi_one, gi_i );

    assert( gr_zero + gr_zero == gr_zero );
    assert( gr_zero + gr_zero == gr_zero );
    assert( gr_half + gr_half == gr_one );
    assert( gr_half * gr_one == gr_half );
    assert( gr_half * gr_half == rat( gi_one, gi_four ) );
    assert( gr_half / gr_half == gr_one );
    assert( gr_one / gr_i == gr_minusi );
    assert( gr_one / gr_one == gr_one );
    assert( gr_i / gr_i == gr_one );
    assert( gr_i * gr_i * gr_i == gr_minusi );

    return 0;
}
