#include <string>
#include <cassert>

/* Write a function ‹stoi_or› which takes a string and an ‹int›. If
 * the string can be parsed using ‹std::stoi›, return the result of
 * ‹stoi›, otherwise return the ‘default’ value from the second
 * argument. */

int main()
{
    assert( stoi_or( "32", 0 ) == 32 );
    assert( stoi_or( "x",  1 ) == 1 );
    assert( stoi_or( "",   3 ) == 3 );
    assert( stoi_or( "123456789123456789", 1 ) == 1 );

    return 0;
}
