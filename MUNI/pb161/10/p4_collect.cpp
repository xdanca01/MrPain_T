#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <cassert>

/* The goal of this exercise is to write a set of overloads that
 * will, together, allow the user to extract values from standard
 * containers, into a vector. For ‹std::map› and
 * ‹std::unordered_map›, this means the value without the key. For
 * other container types, the functions simply copy the contents
 * into an output vector. */

int main()
{
    std::vector iv{ 1, 2, 3 };
    std::vector< std::string > sv{ "hello", "world" };

    std::deque id{ 1, 2, 3 };
    std::deque < std::string > sd{ "hello", "world" };

    std::set is{ 1, 2, 3 };
    std::set< std::string > ss{ "hello", "world" };

    std::unordered_set ius{ 1, 2, 3 };
    std::unordered_set< std::string > sus{ "hello", "world" };

    std::map< int, int > im{ { 0, 1 }, { 7, 2 }, { 13, 3 } };
    std::map< int, std::string > sm{ { 0, "hello" }, { 7, "world" } };

    std::unordered_map< int, int > ium{ { 0, 1 }, { 7, 2 }, { 3, 3 } };
    std::unordered_map< int, std::string > sum{ { 0, "hello" }, { 7, "world" } };

    assert( collect( iv ) == iv );
    assert( collect( sv ) == sv );
    assert( collect( id ) == iv );
    assert( collect( sd ) == sv );
    assert( collect( is ) == iv );
    assert( collect( ss ) == sv );

    auto sorted = []( auto v ) { sort( v.begin(), v.end() ); return v; };

    assert( sorted( collect( ius ) ) == iv );
    assert( sorted( collect( sus ) ) == sv );
    assert( collect( im ) == iv );
    assert( collect( sm ) == sv );
    assert( sorted( collect( ium ) ) == iv );
    assert( sorted( collect( sum ) ) == sv );

    return 0;
}
