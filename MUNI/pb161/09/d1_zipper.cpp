#include <cassert>
#include <vector>

/* The canonic use for C++ templates is designing container classes
 * (collections) which can hold different types of values. In C, the
 * solution was to either use ‹void› pointers (e.g. linked lists in
 * PB071), implement data structures using macros (e.g. linked lists
 * in the Linux kernel), or remember store sizes explicitly and copy
 * in data using ‹void› pointers and the element size (e.g.  ‹glib›
 * data structures). Let's now look at the C++ way, using the
 * familiar ‹zipper› as an example. You may find it helpful to
 * compare this example with ‹05/access.cpp›, which implemented a
 * non-generic version of the same class. */

/* Definition of a «class template» looks the same as a definition
 * of a normal class, all we need to do is specify that we want a
 * template instead, with a single type parameter, which is, by
 * convention, called ‹T›: */

template< typename T >

/* We can now proceed to define a ‹class› and it will become a
 * «class template» (i.e. it will be parametrized by ‹T› above).
 * Anywhere in the body of the class where we can specify a type, we
 * can now also use ‹T›. When we instantiate the template later, the
 * compiler will find all occurrences of ‹T› in the class definition
 * and replace them with the supplied type parameter. This process
 * is known as «substitution». */

class zipper
{
    /* If you remember from a few weeks ago, a zipper can be
     * represented using 2 stacks. Like before, we will use
     * a pair of ‹std::vector› instances. However, the zipper now
     * stores values of type ‹T›, so we will supply that as the type
     * parameter of ‹vector›. */

    using stack = std::vector< T >;
    stack left, right;
    T focus;

public:

    /* Forwarding arguments of arbitrary types is a little too
     * advanced for us, so we will settle for making a copy of the
     * initial value. This means that we require ‹T› to be a type
     * with a copy constructor. In other words, we won't be able to
     * create zippers that hold values of type ‹unique_ptr›. */

    zipper( const T &f ) : focus( f ) {}

    /* Like above, we will settle for a copy. */

    zipper &push_left( const T &x )
    {
        left.push_back( x );
        return *this;
    }

    zipper &push_right( const T &x )
    {
        right.push_back( x );
        return *this;
    }

    /* The ‹shift› helper remains unchanged from our previous
     * implementation. */

    void shift( stack &a, stack &b )
    {
        b.push_back( focus );
        focus = a.back();
        a.pop_back();
    }

    /* This time, we will only have pre-increment and pre-decrement,
     * since those are the only practical variants for this class. */

    zipper &operator++() { shift( right, left ); return *this; }
    zipper &operator--() { shift( left, right ); return *this; }

    /* The dereference and indirect access operators are more
     * interesting, since they need to mention the element type,
     * which is now ‹T›. */

    T &operator*()  { return  focus; }
    T *operator->() { return &focus; }

    /* And the ‹const› overloads of the same: */

    const T &operator*()  const { return  focus; }
    const T *operator->() const { return &focus; }

    /* Indexing operator (non-‹const› overload only). */

    T &operator[]( int i )
    {
        if ( i == 0 ) return focus;
        if ( i < 0 ) return left[ left.size() + i ];
        if ( i > 0 ) return right[ right.size() - i ];
        assert( false );
    }
};

int main() /* demo */
{
    /* Let's first create a ‹zipper› which holds integers. */

    zipper< int > zi( 0 ); // [0]
    zi.push_left( 2 );     // 2 [0]
    zi.push_left( 1 );     // 2 1 [0]
    zi.push_right( 1 );    // 2 1 [0] 1

    /* check */
    assert( zi[ -2 ] == 2 );
    assert( zi[ -1 ] == 1 );
    assert( zi[ 0 ] == 0 );
    assert( zi[ 1 ] == 1 );

    assert( *zi == 0 );

    /* And now a different instance, with pairs. */
    using p = std::pair< int, int >;
    zipper< p > zp( p( 0, 1 ) );

    assert( *zp == p( 0, 1 ) );
    assert( zp->first == 0 );
    assert( zp->second == 1 );

    zp.push_left( p( 7, 7 ) );
    assert( zp[ -1 ] == p( 7, 7 ) );

    -- zp;
    assert( zp->first == 7 );
    assert( zp->second == 7 );
}
