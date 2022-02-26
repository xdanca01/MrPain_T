#include <string>
#include <cassert>

/* Write a predicate which decides whether a given string is a
 * palindrome, i.e. reads the same in both directions. */

bool is_palindrome( const std::string &s );

int main()
{
    assert( is_palindrome( "eye" ) );
    assert( is_palindrome( "anna" ) );
    assert( is_palindrome( "rats live on no evil star" ) );
    assert( is_palindrome( "step on no pets" ) );
    assert( !is_palindrome( "pie" ) );

    return 0;
}
