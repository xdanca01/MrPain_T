#include <array>
#include <memory>
#include <cassert>

/* Imagine there is a large array of data (e.g. numbers), but we
 * sometimes need to change a few of those. However, we also need to
 * keep the original data intact, and we don't want to copy all the
 * data around. In this exercise, we will design a simple solution
 * to this problem. */

/* Implement class template ‹sparse›, with a type argument ‹T› and a
 * ‹size_t› argument ‹N›, with the following interface:
 *
 *  • construct from an ‹std::array› of ‹T› with size ‹N›,
 *  • construct a copy (the ‹array› given to the constructor is
 *    «shared» by all copies),
 *  • ‹set( i, v )› replaces the value stored at index ‹i› with ‹v›
 *    (without affecting the backing array),
 *  • ‹get( i )› returns the value at index ‹i› (that is the ‹v›
 *    passed to latest call to ‹set( i, v )› with the same ‹i›, or
 *    the value from the backing array if ‹set› was never called
 *    for ‹i›,
 *  • ‹merge()› that propagates the changes made in this instance
 *    into all other instances sharing the same backing array.
 *
 * Note: the memory complexity should be O(N) of shared data, and
 * O(M) per instance of ‹sparse› where M is the number of altered
 * indices. A ‹merge› on one copy should «not» affect altered
 * indices in other copies. */

template< typename T, size_t N >
class sparse;

int main()
{
    std::array arr{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    sparse s( arr );
    sparse t = s;

    assert( s.get( 0 ) == 0 );
    assert( s.get( 1 ) == 1 );
    assert( t.get( 0 ) == 0 );
    assert( t.get( 1 ) == 1 );

    s.set( 0, 33 );
    s.set( 1, 7 );
    t.set( 1, 13 );
    assert( s.get( 0 ) == 33 );
    assert( t.get( 0 ) == 0 );

    s.merge();
    assert( s.get( 0 ) == 33 );
    assert( t.get( 0 ) == 33 );

    assert( s.get( 1 ) == 7 );
    assert( t.get( 1 ) == 13 );

    sparse u = t;
    assert( t.get( 1 ) == 13 );
    assert( u.get( 1 ) == 13 );

    t.set( 1, 9 );
    assert( t.get( 1 ) == 9 );
    assert( u.get( 1 ) == 13 );

    return 0;
}
