#include <set>
#include <map>
#include <string>
#include <cassert>

/* Write ‹keyset›, a function which takes an instance of ‹std::map›
 * and returns an ‹std::set› with the keys that were present in the
 * input ‹map›. */

/* And ‹intersect›, which takes an ‹std::set› of keys and an
 * ‹std::map› (with the same key type and arbitrary value type) and
 * produces another ‹std::map› which only retains the key/value
 * pairs for which keys were present in the input key set. */

int main()
{
    using chmap = std::map< int, char >;
    using strmap = std::map< std::string, std::string >;
    using strset = std::set< std::string >;

    chmap m{ { 0, 'a' }, { 1, 'b' }, { 2, 'x' } };
    std::set s{ 0, 2 };
    chmap sm{ { 0, 'a' }, { 2, 'x' } };

    strmap n{ { "hello", "world" }, { "bye", "world" } };
    strset nk{ "hello", "bye" };
    strset t{ "hello" };
    strset w{ "hello", "world" };
    strmap nt{ { "hello", "world" } };

    assert( ( keyset( m ) == std::set{ 0, 1, 2 } ) );
    assert( keyset( sm ) == s );
    assert( intersect( s, m ) == sm );
    assert( keyset( n ) == nk );
    assert( intersect( w, n ) == nt );
    assert( intersect( t, n ) == nt );
    assert( keyset( intersect ( w, n ) ) == t );

    return 0;
}
