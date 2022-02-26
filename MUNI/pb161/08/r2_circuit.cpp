#include <cassert>

/* In this exercise, we will look at calling ‹virtual› methods from
 * within the class, in an ‘inverted’ approach to inheritance. Most
 * of the implementation will be part of the «base class», in terms
 * of a few (or in this case one) ‹protected virtual› methods. */

/* We will implement a simple class hierarchy to represent a
 * «logical circuit»: a bunch of components connected with wires.
 * Each component will have at most 2 inputs and a single output
 * (all of which are boolean values). Implement the following
 * (non-virtual) methods:
 *
 *  • ‹connect› which takes an integer (0 or 1, the index of the
 *    input to connect) and a reference to another ‹component›
 *    and connects the output of the given component to the input
 *    of this component
 *  • ‹read› with no arguments, which returns the current output of
 *    the component (this will of course depend on the state of the
 *    input components).
 *
 * Both inputs start out unconnected. Unconnected inputs always read
 * out ‹false›. Behaviour is undefined if there is a loop in the
 * circuit (but see also ‹loops.cpp›). */

class component;

/* The derived classes should be as follows:
 *
 *  • ‹nand› for which the output is the NAND logical function of
 *    the two inputs,
 *  • ‹source› which ignores both inputs and reads out ‹true›,
 *  • ‹delay› which behaves as follows: first time ‹read› is called,
 *    it always returns zero; subsequent ‹read› calls return a value
 *    that the input 0 had at the time of the previous call to
 *    ‹read›. */

class nand;
class source;
class delay;

int main()
{
    nand n;
    delay d_0, d_1;
    source s;

    /* Check the behaviour of components when both their inputs are
     * always ‹false›. */

    assert(  s.read() );
    assert(  n.read() );
    assert( !d_0.read() );
    assert( !d_0.read() );

    /* Let's try a basic connection and the function of a ‹delay›
     * component. */

    d_0.connect( 0, s );
    assert( d_0.read() == false );
    assert( d_0.read() == true );

    n.connect( 0, d_0 );
    n.connect( 1, d_1 );

    assert( n.read() );
    assert( n.read() );

    d_1.connect( 0, s );

    assert(  n.read() );
    assert( !n.read() );

    return 0;
}
