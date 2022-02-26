#include <string_view>
#include <cassert>

/* Implement a function ‹split›, which given a string view ‹s› and a
 * delimiter ‹delim›, produces a pair of string_views ‹a› and ‹b›
 * such that:
 *
 *  • ‹delim› is not in ‹a›,
 *  • and either
 *    ◦ ‹s == a + delim + b› if ‹delim› was present in ‹s›,
 *    ◦ ‹s == a› and ‹b› is empty otherwise */

using split_view = std::pair< std::string_view, std::string_view >;
split_view split( std::string_view s, char delim );

int main()
{
    auto [ a, b ] = split( "hello world", ' ' );
    assert( a == "hello" );
    assert( b == "world" );

    auto [ c, d ] = split( "hello world", '!' );
    assert( c == "hello world" );
    assert( d == "" );

    auto [ e, f ] = split( "hello world ", ' ' );
    assert( e == "hello" );
    assert( f == "world " );

    auto [ g, h ] = split( "hello_world ", ' ' );
    assert( g == "hello_world" );
    assert( h == "" );

    return 0;
}
