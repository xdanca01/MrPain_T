#include <cassert>

/* An exercise similar to the ‹bittrie› earlier (same data structure
 * but with bigger keys). To make it more interesting, the node
 * management will happen within the class itself and will not be
 * part of the interface. The encoding you should use is this:
 *
 *  • the left child of a node adds a single character to the key,
 *    like in the bit trie from before (the character is part of the
 *    left edge)
 *  • the right child is actually a «sibling» of the current node
 *    and the edge is not labelled
 *  • the chain to the right is sorted in ascending order
 *
 * In other words, you can imagine the trie to be a 256-ary tree,
 * which is obviously impractical to implement directly (this would
 * need 256 pointers per node). Hence, we encode each ‘virtual’ node
 * in this 256-ary trie using a singly-linked list made of the right
 * children of each real, binary node. */

struct trie_node; /* ref: 9 lines */

/* The interface of `trie` is very simple: it has an ‹add› method,
 * which inserts a key into the data structure, and a ‹has› method
 * which decides whether a given key is present. Both accept a
 * single ‹std::string›. Like with the bit trie before, we do not
 * consider prefixes of included keys to be present. */

class trie; /* ref: 53 lines; has() = 10, add() = 36 */

int main()
{
    trie t;
    const trie &ct = t;
    assert( !ct.has( "x" ) );

    t.add( "x" );
    assert(  t.has( "x" ) );
    assert( !t.has( "xx" ) );

    /* "x" disappears from the trie, since it is a prefix of "xx" */

    t.add( "xx" );
    assert(  t.has( "xx" ) );
    assert( !t.has( "x" ) );

    /* add a right sibling to a non-root node */

    t.add( "xy" );
    assert(  t.has( "xx" ) );
    assert(  t.has( "xy" ) );
    assert( !t.has( "yy" ) );
    assert( !t.has( "yx" ) );

    /* add a right sibling to the root node */

    t.add( "yx" );
    assert(  t.has( "xx" ) );
    assert(  t.has( "yx" ) );
    assert(  t.has( "xy" ) );
    assert( !t.has( "yy" ) );

    /* add a left sibling to a non-root node */

    t.add( "yax" );
    assert(  t.has( "xx" ) );
    assert(  t.has( "yax" ) );
    assert(  t.has( "yx" ) );
    assert(  t.has( "xy" ) );

    /* add a left sibling to the root node */

    t.add( "ax" );
    assert(  t.has( "xx" ) );
    assert(  t.has( "yx" ) );
    assert(  t.has( "xy" ) );
    assert(  t.has( "ax" ) );
    assert(  t.has( "yax" ) );

    return 0;
}
