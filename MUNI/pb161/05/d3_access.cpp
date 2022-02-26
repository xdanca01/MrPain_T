#include <vector>
#include <cassert>

/* This set of operators will be slightly more difficult. Surely,
 * you remember the unary ‹*› operator from C, where it is used to
 * dereference pointers. We haven't seen much of that in C++, except
 * perhaps with iterators. We will now see how to implement a class
 * which can be dereferenced like a pointer. We will also add
 * indexing to the mix (like with plain C arrays, or ‹std::vector›
 * or even ‹std::map›). */

/* Let us revisit the ‹zipper› class from last week. We will add
 * indexing (relative to the focus), use a dereference operator to
 * access the focus and we will not store integers, but points in a
 * plane. Cue our favourite class, a ‹point›: */

struct point
{
    double x, y;
    point( double x, double y ) : x( x ), y( y ) {}

    /* We know equality comparison from previous examples. We will
     * need it later on for writing test cases for ‹zipper›. */

    bool operator==( point o ) const { return x == o.x && y == o.y; }
};

/* Now for the zipper. We will need to use ‹std::vector› to be able
 * to index elements, but we will still use ‹left› and ‹right› like
 * stacks. */

class zipper
{
    using stack = std::vector< point >;
    stack left, right;
    point focus;
public:

    zipper( double x, double y ) : focus( x, y ) {}

    /* Inserting points into the zipper. */

    zipper &emplace_left( double x, double y )
    {
        left.emplace_back( x, y );
        return *this;
    }

    zipper &emplace_right( double x, double y )
    {
        right.emplace_back( x, y );
        return *this;
    }

    /* A helper method, so we don't repeat ourselves in the
     * increment/decrement operators below. The trick is to pass the
     * ‹left›/‹right› stacks by reference, since moving left and
     * right is symmetric with regards to those (i.e. the code to
     * move left is the same as to move right, with all occurrences
     * of ‹left› and ‹right› swapped). */

    void shift( stack &a, stack &b )
    {
        b.push_back( focus );
        focus = a.back();
        a.pop_back();
    }

    /* First the pre-increment operators, i.e. ‹++x› and ‹--x›.
     * Here, we use those operators in the manner of C pointer
     * arithmetic (you may want to review that topic). */

    zipper &operator++() { shift( right, left ); return *this; }
    zipper &operator--() { shift( left, right ); return *this; }

    /* Now the post-increment: ‹x++› and ‹x--›. In this particular
     * data structure, they are «expensive» and should «not» be
     * used. They are here just to demonstrate the syntax and a
     * common implementation technique. The difference is that
     * post-increment needs to make a copy, since the «value» of the
     * expression is the object «before» the increment/decrement was
     * applied to it. */

    zipper operator++( int ) { auto r = *this; ++*this; return r; }
    zipper operator--( int ) { auto r = *this; --*this; return r; }

    /* The dereference (unary ‹*›) and indirect member access
     * operators (mutable, i.e.  non-‹const› overloads first, then
     * the ‹const› overloads). Those operators allow us to treat
     * ‹zipper› as if it was a pointer to a ‹point› instance (the
     * one that is in focus). See ‹main› below to see how this works
     * when used. */

    point &operator*()  { return  focus; }
    point *operator->() { return &focus; }

    const point &operator*()  const { return focus; }
    const point *operator->() const { return &focus; }

    /* And finally an indexing operator. We will not bother with the
     * ‹const› version at this time: it would be certainly possible,
     * but ugly and/or repetitive. */

    point &operator[]( int i )
    {
        if ( i == 0 ) return focus;
        if ( i < 0 ) return left[ left.size() + i ];
        if ( i > 0 ) return right[ right.size() - i ];
        assert( false );
    }
};

int main() /* demo */
{

    zipper z( 0, 0 ); // [0,0]

    /* Notice the correspondence between ‹*x› and ‹x[ 0 ]› that we
     * carried over from C pointers. */

    assert( z[ 0 ] == point( 0, 0 ) );
    assert( *z == point( 0, 0 ) );

    /* We will add a few items to the zipper, so that we can
     * demonstrate the other operators. */

    z.emplace_left( 1, 1 );  // (1,1) [0,0]
    z.emplace_right( 2, 1 ); // (1,1) [0,0] (2,1)

    /* Check that the indexing operators behave as expected:
     * negative indices give us items on the left and positive
     * indices give us items on the right. */

    assert( z[ -1 ] == point( 1, 1 ) );
    assert( z[ 1 ]  == point( 2, 1 ) );

    /* Let us check that indexing also works further out. */

    z.emplace_left( 2, 2 ); // (1,1) (2,2) [0,0] (2,1)
    assert( z[ -2 ] == point( 1, 1 ) );
    assert( z[ -1 ] == point( 2, 2 ) );

    /* The pre-decrement operator moves the focus of the zipper tho
     * the left. Let's check that (and demonstrate the
     * correspondence between ‹z[ 0 ]› and ‹*z› again, for a good
     * measure). */

    -- z; // (1,1) [2,2] (0,0) (2,1)
    assert( z[ -1 ] == point( 1, 1 ) );
    assert( z[ 0 ] == point( 2, 2 ) );
    assert( *z == point( 2, 2 ) );

    /* Finally the indirect access operators let us look at ‹x› and
     * ‹y› of the focused point in a nice, succinct way. The syntax
     * is the same that you used to access ‹struct› members via a
     * pointer to the ‹struct› in C. */

    assert( z->x == 2 );
    assert( z->y == 2 );

    /* Move the zipper twice to the right and do a final check. */

    ++ z; ++ z; // (1,1) (2,2) (0,0) [2,1]
    assert( z->x == 2 );
    assert( z->y == 1 );
}

/* Next: quick introduction to exceptions, in ‹exceptions.cpp›. */
