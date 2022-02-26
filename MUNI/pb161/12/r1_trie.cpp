#include <cassert>
#include <memory>

/* We will implement a binary trie (see ‹06/bintrie.cpp› for more
 * details about the data structure) with a twist. */

/* The user will manage the nodes explicitly, for two reasons: doing
 * it automatically is a fair amount of work, and we want to be able
 * to share subtrees. In particular, our present trie implementation
 * will be able to encode 0 and 1 bits in a key, but also a ‹?›: a
 * bit which will not affect the outcome. The easiest way to achieve
 * this is by pointing both the left and the right pointer of a tree
 * to the same child node. To make things even more interesting,
 * each leaf node should be able to reconstruct its own key, with
 * question marks always taken to be ones. The interface:
 *
 *  • ‹root› returns a suitable pointer to the root node,
 *  • ‹add› takes a suitable node pointer (the parent node),
 *    and the bit to append (a ‹bool›),
 *  • ‹add_amb› takes a node and appends a question mark to it,
 *  • ‹find› takes an ‹std::vector› of ‹bool› and returns a shared
 *    pointer to the corresponding node (or a ‹nullptr› if not
 *    found).
 *
 * The default-constructed ‹trie› should be empty. Both ‹add› and
 * ‹add_amb› should return a (shared) pointer to the new node. The
 * nodes should provide the method ‹key› which returns an
 * ‹std::vector› of ‹bool›. The nodes must not store the entire key.
 * */

class trie_node; /* ref: 26 lines */
class trie;      /* ref: 30 lines */

int main()
{
    using bv = std::vector< bool >;

    trie t;
    const trie &ct = t;

    auto n_1    = t.add( t.root(), true );
    auto n_10   = t.add( n_1, false );
    auto n_11   = t.add( n_1, true );
    auto n_10x  = t.add_amb( n_10 );
    auto n_10x1 = t.add( n_10x, true );

    assert( ct.find( bv{ 1 } ) == n_1 );
    assert( ct.find( bv{ 1, 0, 0, 1 } ) == n_10x1 );
    assert( ct.find( bv{ 1, 0, 1, 1 } ) == n_10x1 );
    assert( ( n_10x1->key() == bv{ 1, 0, 1, 1 } ) );
    assert( ( n_10->key() == bv{ 1, 0 } ) );

    return 0;
};
