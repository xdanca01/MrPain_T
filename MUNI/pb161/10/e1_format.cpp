#include <set>
#include <vector>
#include <sstream>
#include <cassert>

/* In this exercise, we will practice writing functions with more
 * complex argument deduction. The functions in question will use
 * ‹std::ostringstream› to produce string representation of sets and
 * vectors. */

/* Use a comma-separated format for ‹std::vector› instances, with
 * arbitrary element type, then do the same for ‹std::set›. Vectors
 * should use square brackets ‹[]› and sets should use curly braces
 * ‹{}› as delimiters. Assume the ‹value_type› stored in the vector
 * has appropriate ‹std::ostream› operators. The functions should be
 * called ‹format›. */

int main()
{
    std::vector v{ 1, 2, 3 };
    std::vector w{ "hello", "world" };
    std::set s{ 1.0, 2.0 };

    assert( format( v ) == "[ 1, 2, 3 ]" );
    assert( format( w ) == "[ hello, world ]" );
    assert( format( s ) == "{ 1, 2 }" );
    assert( format( std::set< int >() ) == "{}" );
    assert( format( std::vector< int >() ) == "[]" );

    return 0;
}
