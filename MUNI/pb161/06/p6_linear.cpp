#include <cassert>
#include <exception>

/* Write a solver for linear equations in 2 variables. The interface
 * will be a little unconventional: overload operators ‹+›, ‹*› and
 * ‹==› and define global constants ‹x› and ‹y› of suitable types,
 * so that it is possible to write the equations as shown in ‹main›
 * below. */

/* Note that the return type of ‹==› does not have to be bool. It
 * can be any type you like, including of course custom types. For
 * ‹solve›, I would suggest looking up Cramer's rule. */

/* ref: class ‹eqn› 25 lines, ‹solve› 8 lines, ‹x› and ‹y› 2 lines */

/* If the system has no solution, throw an exception of type
 * ‹no_solution›. Derive it from ‹std::exception›. */

int main()
{
    std::pair a( 1.0, 0.0 ), b( -1.0, 0.0 ), c( .6, -.2 );
    assert( solve(     x + y == 1,  x     - y ==  1 ) == a );
    assert( solve(     x + y == -1, x     - y == -1 ) == b );
    assert( solve(     x + y == x,  x     + y ==  1 ) == a );
    assert( solve( 2 * x + y == 1,  x - 2 * y ==  1 ) == c );
    assert( solve(     x + y - 1,   x     - y - 1   ) == a );

    try
    {
        solve( x + y == 1,  x + y == 2 );
        assert( 0 );
    }
    catch ( const std::exception &e ) {}

    try
    {
        solve( x + y == 1,  x + y == 2 );
        assert( 0 );
    }
    catch ( const no_solution &e ) {}

    return 0;
}
