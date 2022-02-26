#include <cassert>
#include <memory>

/* We will now implement a data structure ‘from scratch’ (i.e.
 * without using ‹std› containers) using templates. Again, you may
 * find it useful to go back to ‹06/queue.cpp› and compare the two
 * implementations. */

/* Like before, since we are going for a custom, node-based
 * structure, we will need to first define the class to represent
 * the nodes. Unlike the previous implementation, however, the node
 * itself needs to be parametrized by the type of the value it
 * should hold. */

template< typename T >
struct queue_node
{

    /* You may have noticed this with the ‹zipper› earlier: we do
     * not need to mention the type parameter when we want to refer
     * to the instance of ‹queue_node› where ‹T› is ‹T› (though we
     * can if we want to). In other words, within ‹queue_node›,
     * saying ‹queue_node› when referring to a type means the same
     * thing as ‹queue_node< T >›. */

    std::unique_ptr< queue_node > next;

    /* The data attribute will be of type ‹T›. */

    T value;
};

/* Like the node, the iterator also needs to be parametric. */

template< typename T >
struct queue_iterator
{
    /* Here, we have no choice but to explicitly spell out the type
     * parameter of ‹queue_node›, since we are no longer within that
     * class. */

    queue_node< T > *node;

    /* Constructor names are unaffected by templates. */

    queue_iterator( queue_node< T > *n ) : node( n ) {}

    /* The pre-increment operator simply shifts the pointer to the
     * ‹next› pointer of the currently active node. This method is
     * unchanged from the non-generic version. */

    queue_iterator &operator++()
    {
        node = node->next.get();
        return *this;
    }

    /* The implicit ‘current instance of the template’ shortcut
     * works in arguments too, including in arguments of ‹friend›
     * functions, so let's demonstrate that: */

    friend bool operator!=( const queue_iterator &a,
                            const queue_iterator &b )
    {
        return a.node != b.node;
    }

    /* Finally the dereference operator. Unlike before, we don't
     * know much about ‹T›, hence we prefer to always return a
     * reference, even in the ‹const› overload. */

          T &operator*()       { return node->value; }
    const T &operator*() const { return node->value; }
};

/* The ‹queue› itself will be a template too, of course. */

template< typename T >
class queue
{
    /* Like in the iterator, we need to instantiate any template
     * classes that we use that were defined earlier. That is the
     * only difference compared to our earlier ‹queue›
     * implementation.  */

    std::unique_ptr< queue_node< T > > first;
    queue_node< T > *last = nullptr;
public:

    /* In the integer-only version, we passed the argument by value,
     * but like in the dereference operator above, we will now
     * instead use a ‹const› reference: ‹T› might be a big class
     * with an expensive copy operation. We do not want to do that
     * twice. */

    void push( const T &v )
    {
        if ( last ) /* non-empty list */
        {
            /* Notice the ‹T› in the ‹make_unique› call. */
            last->next = std::make_unique< queue_node< T > >();
            last = last->next.get();
        }
        else /* empty list */
        {
            first = std::make_unique< queue_node< T > >();
            last = first.get();
        }

        last->value = v;
    }

    /* Now we run into a bit of a problem. Since making copies of
     * ‹T› is possibly expensive, we would like to return a
     * reference: but we cannot, since ‹pop› will destroy the node
     * which stores the value. Incidentally, this is the reason why
     * ‹std::queue::pop› is a void function and you need to use a
     * separate ‹front› call to get the value. We will simply return
     * by value instead, which can be less efficient, but not
     * terribly so. We can reduce the cost by using ‹std::move› on
     * the value, since the node is going to be destroyed anyway. */

    T pop()
    {
        T v = std::move( first->value );
        first = std::move( first->next );

        /* Do not forget to update the ‹last› pointer in case we
         * popped the last item. */

        if ( !first ) last = nullptr;
        return v;
    }

    /* The emptiness check is simple enough. */

    bool empty() const { return !last; }

    /* Same as before, but we need to instantiate the
     * ‹queue_iterator› template. */

    queue_iterator< T > begin() { return { first.get() }; }
    queue_iterator< T > end()   { return { nullptr }; }

    /* Same. */

    void erase_after( queue_iterator< T > i )
    {
        assert( i.node->next );
        i.node->next = std::move( i.node->next->next );
    }
};

int main() /* demo */
{
    /* We start by constructing an (empty) queue and doing some
     * basic operations on it. We start by inserting and removing a
     * single element. */

    queue< std::pair< long, long > > q;

    assert( q.empty() );
    q.push( { 7, 0 } );
    assert( !q.empty() );
    assert( q.pop() == std::pair( 7l, 0l ) );
    assert( q.empty() );

    /* Now that we have emptied the queue again, we add a few more
     * items and try erasing one and iterating over the rest.*/

    q.push( { 1, 0 } );
    q.push( { 2, 0 } );
    q.push( { 7, 0 } );
    q.push( { 3, 0 } );

    /* We check that erase works as expected. */

    queue_iterator i = q.begin();
    ++ i;
    assert( *i == std::pair( 2l, 0l ) );
    q.erase_after( i );

    /* We can still use instances of ‹queue› in range ‹for› loops,
     * because they have ‹begin› and ‹end›, and the types those
     * methods return (i.e. iterators) have dereference, inequality
     * and pre-increment. Since our current instance of ‹queue›
     * contains pairs, we can also use structured bindings in the
     * ‹for› loop. */

    int x = 1;

    for ( auto [ v, w ] : q )
    {
        assert( v == x++ );
        assert( w == 0 );
    }

    /* That went rather well, let's just check that the order of
     * removal is the same as the order of insertion (first in,
     * first out). This is how queues should behave. */

    assert( q.pop() == std::pair( 1l, 0l ) );
    assert( q.pop() == std::pair( 2l, 0l ) );
    assert( q.pop() == std::pair( 3l, 0l ) );
    assert( q.empty() );
}
