#include <cassert>

/* In this exercise, your goal will be to define a list-like
 * structure using templates, and then recursively sum numbers
 * stored in it. You will need to use both templates and function
 * overloading. */

/* In LISP-like languages, lists are built out of so-called «cons
 * cells» (cons being short for constructor). Each cell contains a
 * value and a pointer to the next cell. The value is traditionally
 * called ‹car› and the pointer to the next cell is called ‹cdr›.
 * The ‹cdr› may also be ‹null›, i.e. an empty list. In our case,
 * the ‹car› will always be of type ‹int›. The ‹cdr› will be given
 * by a template parameter, in the expectation that it is another
 * ‹cons› instance or ‹null›. */

struct null {}; /* empty */

template< typename cdr_t >
struct cons;

/* Overloads and/or templates of ‹sum› go here. */

int main()
{
    /* a cons list with 0, 1 and 2 as elements */
    cons c( 0, cons( 1, cons( 2, null() ) ) );
    cons d( 4, cons( 2, null() ) );
    const cons e( 10, null() );

    assert( sum( c ) ==  3 );
    assert( sum( d ) ==  6 );
    assert( sum( e ) == 10 );
    assert( sum( null() ) == 0 );

    return 0;
};
