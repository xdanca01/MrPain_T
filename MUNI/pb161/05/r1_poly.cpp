#include <cassert>

/* Goal: implement polynomials with addition (easy) and
 * multiplication (less easy).  A polynomial is a term of the form
 * ⟦7x⁴ + 0x³ + 0x² + 3x + x⁰⟧ -- i.e. a sum of non-negative
 * integral powers of ⟦x⟧, with each power carrying a fixed
 * (constant) coefficient. Adding two polynomials will simply give
 * us a polynomial where coefficients are sums of the coefficients
 * of the two addends. The case of multiplication is more
 * complicated, because:
 *
 *  • each term of the first polynomial has to be multiplied by each
 *    term of the second polynomial
 *  • some of those products give equal powers of ⟦x⟧ and hence
 *    their coefficients need to be summed
 *
 * For each polynomial, there is some ⟦n⟧, such that all powers
 * higher than ⟦n⟧ have a zero coefficient. This is important when
 * you want to store the polynomials in a computer. */

/* The default constructor of the class ‹poly› should generate a
 * polynomial which has all coefficients set to 0. Besides addition
 * and multiplication (which are implemented as operators), also
 * implement equality and a method ‹set›, which takes an exponent
 * (power of ⟦x⟧) and a coefficient, both integers. */

class poly; /* reference implementation is 45 lines */

int main()
{
    auto mkpoly = []( auto... coeff )
    {
        poly p;
        int i = sizeof...( coeff );
        for ( int c : { coeff... } )
            p.set( --i, c );
        return p;
    };

    const poly a       = mkpoly( 1, 0, 1 );       // x² + 1
    const poly b       = mkpoly( 2, 0, 1 );       // 2x² + 1
    const poly c       = mkpoly( 1, 1 );          // x + 1
    const poly d       = mkpoly( 1, 0 );          // x
    const poly sum_ab  = mkpoly( 3, 0, 2 );       // 3x² + 2
    const poly sum_ac  = mkpoly( 1, 1, 2 );       // x² + x + 2
    const poly sum_ad  = mkpoly( 1, 1, 1 );       // x² + x + 1
    const poly prod_ab = mkpoly( 2, 0, 3, 0, 1 ); // 2x⁴ + 3x² + 1
    const poly prod_ac = mkpoly( 1, 1, 1, 1 );    // x³ + x² + x + 1
    const poly prod_cd = mkpoly( 1, 1, 0 );       // x² + x
    const poly prod_bd = mkpoly( 2, 0, 1, 0 );    // 2x³ + x

    assert( a + b == sum_ab );
    assert( b + a == sum_ab );
    assert( a + c == sum_ac );
    assert( c + a == sum_ac );
    assert( a + d == sum_ad );
    assert( d + a == sum_ad );

    assert( a * b == prod_ab );
    assert( b * a == prod_ab );
    assert( a * c == prod_ac );
    assert( c * a == prod_ac );
    assert( c * d == prod_cd );
    assert( d * c == prod_cd );
    assert( b * d == prod_bd );
    assert( d * b == prod_bd );

    return 0;
}
