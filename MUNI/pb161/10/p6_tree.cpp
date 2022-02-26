#include <memory>
#include <cassert>

/* In this exercise, we will implement an immutable binary tree,
 * similar to the list we saw earlier. */

/* Implement class template ‹tree› with a single type argument ‹T›
 * and the following interface (all method are ‹const›):
 *
 *  • the default constructor creates an empty tree,
 *  • a 3-parameter constructor creates a tree with the value given
 *    in the first argument as root and the next two arguments specify
 *    the left and right subtrees,
 *  • a 1-parameter constructor creates a leaf node which stores the
 *    value given in the argument,
 *  • ‹empty› returns true if the tree is empty,
 *  • ‹root› returns a reference to the value in the root,
 *  • ‹left› returns the left subtree,
 *  • ‹right› returns the right subtree.
 *
 * Hint: two of the constructors can be merged using default
 * arguments. */

template< typename T >
class tree;

int main()
{
    using ti = tree< int >;
    using tu = tree< std::unique_ptr< int > >;

    const ti ei;
    const tu eu;

    assert( ei.empty() );
    assert( eu.empty() );

    const ti fi( 3 ), gi( 1, fi, fi );

    assert( fi.root() == 3 );
    assert( fi.left().empty() );
    assert( fi.right().empty() );
    assert( gi.root() == 1 );
    assert( gi.left().root() == 3 );
    assert( gi.right().root() == 3 );
    assert( gi.left().right().empty() );
    assert( gi.left().left().empty() );

    assert( &gi.left().root() == &gi.right().root() );

    const tu fu( std::make_unique< int >( 7 ) ),
             gu( std::make_unique< int >( 4 ), fu, eu ),
             hu( std::make_unique< int >( 1 ), fu, gu );

    assert( *fu.root() == 7 );
    assert( *gu.root() == 4 );
    assert( *hu.root() == 1 );
    assert( *hu.left().root() == 7 );
    assert( *hu.right().root() == 4 );

    assert( hu.left().root() == gu.left().root() );
    assert( hu.right().root() == gu.root() );

    return 0;
}
