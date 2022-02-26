#include <cmath>
#include <cassert>

/* This demonstration is as far as we'll venture with regards to
 * numeric approximation – the exercises that deal with
 * approximations are all much simpler than this demo. Here, we will
 * implement the general Newton-Raphson method. This can be used for
 * finding all kinds of roots (zeroes of functions) numerically and
 * for solving ‘hard’ (transcendental) equations. */

/* The input to Newton's method is a function ‹f› and its
 * derivative, ‹df›. A single improvement step then takes the
 * current estimate ⟦x₀⟧ and subtracts ⟦f(x)/df(x)⟧ from it. It
 * is actually quite simple. */

auto newton = []( auto f, auto df, double ini, double prec )
{
    double x = ini, y = ini - f( x ) / df( x );

    while ( std::fabs( y - x ) >= prec )
    {
        x = y;
        y = y - f( x ) / df( x );
    }

    return y;
};

/* We can straightforwardly apply the above generic function to
 * suitable arguments to immediately implement some familiar
 * functions, like square roots or cube roots (we just need to find
 * a function which becomes zero if ⟦x⟧ is the square root of the
 * argument of the function; that function would be
 * ⟦f(z) = z² - x⟧ and its derivative is ⟦f'(z) = 2z⟧). */

double sqrt( double x, double prec ) /* square root */
{
    return newton( [=]( double z ) { return z * z - x; },
                   [=]( double z ) { return 2 * z; }, 1, prec );
}

double cbrt( double n, double prec ) /* cube root */
{
    return newton( [=]( double z ) { return z * z * z - n; },
                   [=]( double z ) { return 3 * z * z; }, 1, prec );
}

/* Compute nth root of x, generalizing ‹sqrt› and ‹cbrt› above. */

double root( int n, double x, double prec )
{
    auto  f = [=]( double z ) { return std::pow( z, n ) - x; };
    auto df = [=]( double z ) { return n * std::pow( z, n - 1 ); };
    return newton( f, df, 1, prec );
}

/* Scroll to the end to see the test cases. The following code
 * computes π using only basic arithmetic and the Newton method…
 * It's all a bit fast and loose, but it works. Enjoy. */

/* Approximately evaluate a function using its truncated Taylor
 * expansion. */

auto taylor = []( auto coeff, double x, double prec )
{
    double r = 0, pow = 1, fact = 1;
    int i = 0;

    while ( pow / fact > prec / 10 )
    {
        r += coeff( i ) * pow / fact;
        fact *= ++i;
        pow *= x;
    }

    return r;
};

/* Shorthand for 4-periodic Taylor coefficients (like those that
 * appear in trigonometric functions). */

auto trig_coeff( int a, int b, int c, int d )
{
    return [=]( int i ) { return i % 4 == 0 ? a : i % 4 == 1 ? b :
                                 i % 4 == 2 ? c : d; };
}

/* Sine and cosine, to feed into Newton. */

double sine( double x, double prec )
{
    return taylor( trig_coeff( 0, 1, 0, -1 ), x, prec );
}

double cosine( double x, double prec )
{
    return taylor( trig_coeff( 1, 0, -1, 0 ), x, prec );
}

/* Compute π/2 as the root of cosine. */

double pi( double prec )
{
    auto  f = [=]( double x ) { return cosine( x, prec ); };
    auto df = [=]( double x ) { return -sine( x, prec ); };
    return 2 * newton( f, df, 1, prec );
}

int main() /* demo */
{
    for ( int decimals = 1; decimals < 10; ++ decimals )
    {
        double p = std::pow( 10, -decimals );

        assert( std::fabs( sqrt( 2, p ) - std::sqrt( 2 ) ) < p );
        assert( std::fabs( cbrt( 2, p ) - std::cbrt( 2 ) ) < p );

        assert( std::fabs( root( 2, 2, p ) - std::sqrt( 2 ) ) < p );
        assert( std::fabs( root( 3, 2, p ) - std::cbrt( 2 ) ) < p );
        assert( std::fabs( root( 4, 16, p ) - 2 ) < p );

        assert( std::fabs( pi( p ) - 4 * std::atan( 1 ) ) < p );
    }
}
