#include <cassert>

/* In this exercise, we will implement a circular list again, but
 * this time generically, i.e. using templates.  Like before,
 * instead of the usual access operators and iteration, it will have
 * a ‹rotate› method, which rotates the entire list. We require that
 * rotation does not invalidate any references to elements in the
 * list.

 * If you think of the list as a stack, you can think of the
 * ‹rotate› operation as taking an element off the top and putting
 * it at the bottom of the stack. It is undefined on an empty list.
 *
 * To add and remove elements, we will implement ‹push› and ‹pop›
 * which work in a stack-like manner. Only the top element is
 * accessible, via the ‹top› method. This method should allow both
 * read and write access. Finally, we also want to be able to check
 * whether the list is ‹empty›. It is okay to make copies in ‹push›,
 * but make sure you return references in ‹top›. */

template< typename T >
struct circular_node; /* ref: 8 lines */

template< typename >
class circular; /* ref: 34 lines */

int main()
{
    circular< int > list;
    const auto &c = list;

    /* We first push a few items and check that ‹top› behaves as
     * expected. */

    assert( c.empty() );
    list.push( 7 ); // [7]
    assert( !c.empty() );
    assert( c.top() == 7 );

    list.push( 3 ); // 7 [3]
    assert( c.top() == 3 );
    list.top() = 8; // 7 [8]
    list.push( 9 ); // 7 8 [9]

    /* Now let's rotate the list a couple times. */

    assert( c.top() == 9 );
    int &ref_1 = list.top();
    list.rotate(); // 9 7 [8]
    assert( c.top() == 8 );
    assert( ref_1 == 9 );
    int &ref_2 = list.top();
    list.rotate(); // 8 9 [7]

    assert( ref_1 == 9 );
    assert( ref_2 == 8 );

    /* Now pop off each of the items, until the list is empty. We
     * keep checking that the references were not damaged. */

    assert( c.top() == 7 );
    list.pop();

    assert( ref_1 == 9 );
    assert( ref_2 == 8 );
    assert( c.top() == 9 );
    list.pop();

    assert( ref_2 == 8 );
    assert( c.top() == 8 );
    list.pop();

    assert( c.empty() );

    /* Try again with a different type. */

    circular< double > dlist;
    dlist.push( 0.5 ); // [0.5]
    dlist.push( 2.0 ); // 0.5 [2.0]
    assert( dlist.top() == 2.0 );
    dlist.rotate();
    assert( dlist.top() == 0.5 );
    dlist.pop();
    assert( dlist.top() == 2.0 );
    dlist.pop();
    assert( dlist.empty() );

    return 0;
}
