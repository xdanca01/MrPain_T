#include <vector>
#include <memory>
#include <cassert>

/* We will revisit one of the sequence-related constructs from
 * earlier, that of on-the-fly (on demand) transformation (mapping)
 * of elements using a given function. In particular, we will look
 * at composing maps. */

/* In this exercise, you should implement a ‹map› view like the one
 * we did in ‹09/map.cpp›, with one improvement: it should also work
 * with functions which return references. The easiest way to do
 * that is by creating a type alias using ‹decltype› and use that as
 * the return type of the dereference operator. (Alternatively, look
 * up ‹decltype( auto )› in cppreference, though that wouldn't work
 * if you wanted to use the type as a component of another type.) */

int main()
{
    auto id = []( const auto &x ) -> decltype ( x ) { return x; };
    auto get_0 = []( auto &vec ) -> const int & { return vec[ 0 ]; };

    std::vector< int > x{ 3, 1, 2 };
    std::vector< std::unique_ptr< int > > y;
    y.emplace_back( new int( 4 ) );
    std::vector z{ x, std::vector{ 2 } };

    map m( x, id );
    assert( m.begin() != m.end() );
    assert( *m.begin() == 3 );

    map n( y, id );
    assert( **n.begin() == 4 );

    map o( z, get_0 );
    assert( z[ 0 ][ 0 ] == 3 );
    assert( &*o.begin() == &z[ 0 ][ 0 ] );

    return 0;
}
