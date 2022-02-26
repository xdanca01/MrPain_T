#include <cassert>

/* A rope is a string-like data structure, represented as a binary
 * tree with traditional strings in leaves and weights in internal
 * nodes. Subtree sharing is allowed and expected.
 *
 * A weight of a given node is the total length of the string
 * represented by its left subtree. Provides an O(1) concatenation
 * and O(d) indexing, where d is the depth of the tree.
 *
 * In addition to the indexing operator, provide 2 constructors: one
 * which constructs a singleton rope from a string, and another that
 * joins 2 existing ropes.
 *
 * You do not need to implement any rebalancing. */

class rope;

int main()
{
    rope h( "hello" ), w( "world" );
    assert( h[ 0 ] == 'h' );
    assert( h[ 4 ] == 'o' );
    assert( w[ 0 ] == 'w' );
    assert( w[ 4 ] == 'd' );

    rope hw( h, w );
    assert( hw[ 0 ] == 'h' );
    assert( hw[ 4 ] == 'o' );
    assert( hw[ 5 ] == 'w' );
    assert( hw[ 9 ] == 'd' );

    rope hww( hw, w );
    assert( hww[  0 ] == 'h' );
    assert( hww[  4 ] == 'o' );
    assert( hww[  5 ] == 'w' );
    assert( hww[  9 ] == 'd' );
    assert( hww[ 10 ] == 'w' );
    assert( hww[ 14 ] == 'd' );

    const rope hhw( h, hw );
    assert( hhw[ 0 ] == 'h' );
    assert( hhw[ 4 ] == 'o' );
    assert( hhw[ 5 ] == 'h' );
    assert( hhw[ 9 ] == 'o' );
    assert( hhw[ 10 ] == 'w' );
    assert( hhw[ 14 ] == 'd' );

    h[ 0 ] = 'x';
    assert( h[ 0 ] == 'x' );
    assert( h[ 4 ] == 'o' );
    assert( w[ 0 ] == 'w' );
    assert( w[ 4 ] == 'd' );

    /* subtrees are shared */
    assert( hhw[ 0 ] == 'x' );

    return 0;
}
