#include <vector>
#include <cassert>

/* We will do an infix version of the evaluator from the previous
 * exercise. Additionally, we will want to store common
 * sub-expressions only once. For this reason, we will store the
 * nodes in a pool and only take out references to them. */

struct node
{
    /* The type of the node. Only ‹mul› and ‹add› nodes have
     * children. */

    enum op_t { mul, add, constant } op;

    /* The attributes ‹left› and ‹right› are indices, with -1
     * indicating an invalid (null) reference. The ‹is_root› boolean
     * indicates whether this node is a root – that is, it does not
     * appear as a child of any other node. */

    int left = -1, right = -1;
    bool is_root = true;

    /* The value stored in a ‹constant›-type node. */

    int value = 0;
};

using node_pool = std::vector< node >;

/* An ‘ephemeral’ reference to a node – one that can be used to
 * traverse an expression tree, but which is only valid as long as
 * the ‹eval› instance which created it is alive. Add ‹const›
 * methods ‹left()›, ‹right()› which return another ‹node_ref›
 * instance, a ‹const› method ‹compute()› which evaluates the
 * subtree, and a non-‹const› method ‹update( int )› which only
 * works on nodes of type ‹constant›. */

class node_ref;

/* The ‹eval› class represents an entire expression which can be
 * evaluated, traversed (starting from root nodes – those which have
 * no parent) and, most importantly, extended by creating new
 * nodes. */

class eval
{
    node_pool _pool;
public:
    std::vector< node_ref > roots();

    node_ref add( node_ref, node_ref );
    node_ref mul( node_ref, node_ref );
    node_ref number( int );
};

int main()
{
    auto root = []( eval &e )
    {
        assert( e.roots().size() == 1 );
        return e.roots().front();
    };

    eval e;

    node_ref a = e.number( 7 );
    node_ref b = e.number( 8 );
    node_ref sum = e.add( a, b );
    node_ref mul = e.mul( sum, sum );

    assert( sum.compute() == 15 );
    assert( mul.compute() == 225 );

    eval f = e;

    assert( root( f ).compute() == 225 );
    assert( root( f ).left().compute() == 15 );

    node_ref r = f.add( root( f ), root( f ) );
    assert( r.compute() == 450 );
    assert( root( e ).compute() == 225 );

    e = f;
    assert( root( e ).compute() == 450 );

    return 0;
}
