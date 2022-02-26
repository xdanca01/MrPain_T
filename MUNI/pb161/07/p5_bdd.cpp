#include <cassert>
#include <memory>

/* Binary decision diagrams are a compact way to write boolean
 * functions in multiple arguments. You could think of the data
 * structure as a DAG with additional semantics: each vertex is
 * either a «variable» and has two successors which tell us where
 * to go next depending on the value of that variable, or is a 0 or
 * 1, represented by two sink nodes in the DAG (there are no
 * outgoing edges). */

/* The interface should be as follows;
 *
 *  • the constructor takes a ‹char›: the variable to use for the
 *    root node
 *  • ‹one› returns the «true» node
 *  • ‹zero› returns the «false» node
 *  • ‹root› returns the initial node
 *  • ‹add_var› takes a char and «creates» a new variable node:
 *    there may be multiple nodes for the same variable
 *  • ‹add_edge› takes the parent node, a boolean, and the child
 *  • ‹eval› takes a map from ‹char› to ‹bool› and returns ‹true› or
 *    ‹false› by traversing the BDD from the root and at each
 *    variable node, taking the path dictated by the input map
 *    (variable assignment)
 *
 * Note: It is UB if a variable node does not have both successors
 * set. */

class bdd_node; /* ref:  6 lines */
class bdd;      /* ref: 19 lines */

int main()
{
    bdd b( 'z' );
    const bdd &cb = b;

    auto tt = b.one();
    auto ff = b.zero();

    auto x1 = b.add_var( 'x' );
    auto x2 = b.add_var( 'x' );
    auto y1 = b.add_var( 'y' );
    auto y2 = b.add_var( 'y' );
    auto z =  b.root();

    auto eval = [&]( bool x, bool y, bool z )
    {
        using m = std::map< char, bool >;
        return cb.eval( m{ { 'x', x }, { 'y', y }, { 'z', z } } );
    };

    b.add_edge( z,  true,  y1 );
    b.add_edge( z,  false, y2 );
    b.add_edge( y1, false, ff );
    b.add_edge( y1, true,  tt );
    b.add_edge( y2, false, x1 );
    b.add_edge( y2, true,  x2 );
    b.add_edge( x1, true,  ff );
    b.add_edge( x1, false, tt );
    b.add_edge( x2, false, ff );
    b.add_edge( x2, true,  tt );

    assert(  eval( 0, 0, 0 ) );
    assert( !eval( 0, 0, 1 ) );
    assert( !eval( 0, 1, 0 ) );
    assert(  eval( 0, 1, 1 ) );
    assert( !eval( 1, 0, 0 ) );
    assert( !eval( 1, 0, 1 ) );
    assert(  eval( 1, 1, 0 ) );
    assert(  eval( 1, 1, 1 ) );

    return 0;
}
