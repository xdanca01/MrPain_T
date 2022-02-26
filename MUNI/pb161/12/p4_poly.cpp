#include <cmath>
#include <vector>
#include <cassert>

/* In this exercise, we will find at least one (real) root of an
 * odd-degree polynomial (this is guaranteed to exist, and is
 * comparatively easy to find using the intermediate value theorem
 * and binary search). */

/* Write function ‹find_root› which takes 2 arguments:
 *
 *  • a vector of coefficients (each represented as a ‹double›),
 *    sorted from the highest-degree to the lowest, e.g.
 *    ⟦x³ - 3x + 2⟧ is represented as the vector
 *    ‹{ 1, 0, -3, 2 }›,
 *  • an ‹std::pair› with a lower and an upper bound on the value
 *    of the root.
 *
 * The function should return a root ⟦x⟧, that is, a number for
 * which the polynomial evaluates to zero, e.g. ⟦x = -2⟧ for the
 * above example.
 *
 * The pre-condition is that the bounds evaluate to numbers with
 * opposite signs (and hence the interval must contain at least one
 * root). There might be multiple roots in the interval, though: it
 * does not matter which one the function finds. The returned
 * ‹double› should be within `1e-5` of the actual value of the
 * root. */

using bounds = std::pair< double, double >;
using poly   = std::vector< double >;

double find_root( const poly &, bounds ); /* ref: 28 lines */

int main()
{
    using il = std::initializer_list< double >;

    auto check = []( il roots, bounds b, auto coeff )
    {
        double found = find_root( std::vector( coeff ), b );
        for ( double r : roots )
            if ( std::fabs( found - r ) < 1e-5 )
                return true;
        return false;
    };

    auto mul = []( poly x, poly y )
    {
        std::vector< double > out( x.size() + y.size() - 1 );
        for ( size_t i = 0; i < x.size(); ++i )
            for ( size_t j = 0; j < y.size(); ++j )
                out[ i + j ] += x[ i ] * y[ j ];
        return out;
    };

    auto prod = [&]( il factors )
    {
        poly p{ 1 };
        for ( auto f : factors )
            p = mul( p, std::vector{ 1, -f } );
        return p;
    };

    auto chk_prod = [&]( il roots, bounds b )
    {
        return check( roots, b, prod( roots ) );
    };

    assert( check( { -1 }, bounds( -2, 2 ), il{ 1, 1 } ) );
    assert( check( { -1 }, bounds( -2, 2 ), il{ 1, 0, 0, 1 } ) );
    assert( check( { -1 }, bounds( -2, 2 ), il{ 1, 0, 0, 0, 0, 1 } ) );

    assert( chk_prod( { -1, 1, 2 },   bounds( -4, 4 ) ) );
    assert( chk_prod( { 3, 7, 1 },    bounds( 0, 20 ) ) );
    assert( chk_prod( { 1, 1, 1 },    bounds( -5, 5 ) ) );
    assert( chk_prod( { -10, 0, 10 }, bounds( -100, 100 ) ) );

    return 0;
}
