#include <cassert>

/* Write a simple stack-based evaluator for numeric expressions in
 * an RPN form.  The operations:
 *
 *  • ‹push› takes a number and pushes it onto the working stack,
 *  • ‹apply› accepts an instance of one of the three operator
 *    classes defined below; like with the string builder earlier,
 *    both those methods should return a reference to the evaluator,
 *  • again like with the zipper, a ‹top› method should give access
 *    to the current top of the stack, including the possibility of
 *    changing the value,
 *  • ‹pop› which also returns the popped value, and
 *  • ‹empty› which returns a ‹bool›.
 *
 * All three operators are binary (take 2 arguments). */

struct add {};  /* addition */
struct mul {};  /* multiplication */
struct dist {}; /* absolute value of difference */

class eval;

int main()
{
    eval e;
    const eval &f = e;

    e.push( 7 ).push( 3 ).apply( add() );
    assert( f.top() == 10 );
    assert( e.push( 1 ).apply( mul() ).top() == 10 );
    assert( e.pop() == 10 );
    assert( f.empty() );
    e.push( 10 );
    e.top() = 11;
    assert( f.top() == 11 );
    e.push( 10 ).apply( dist() );
    assert( f.top() == 1 );
    assert( !f.empty() );
    assert( e.pop() == 1 );
    assert( f.empty() );

    return 0;
}
