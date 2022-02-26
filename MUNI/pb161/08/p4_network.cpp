#include <cassert>

/* In this exercise, we will define a network of counters, where
 * each node has its own counter which starts at zero, and events
 * which affect the counters propagate in the network. Different
 * node types react differently to the events. */

/* There are three basic events which can propagate through the
 * network: ‹reset› will set the counter to 0, ‹increment› and
 * ‹decrement› add and subtract 1, respectively. */

enum class event { reset, increment, decrement };

/* The «abstract base class», ‹node›, will define the polymorphic
 * interface. Methods:
 *
 *  • ‹react› with a single argument of type ‹event›,
 *  • ‹connect› which will take a reference to another ‹node›
 *    instance: the connection thus created starts in ‹this› and
 *    extends to the ‹node› given in the argument,
 *  • ‹read›, a ‹const› method that returns the current value of the
 *    counter.
 *
 * Think carefully about which methods need to be ‹virtual› and
 * which don't. The counter is signed and starts at 0. Each node can
 * have an arbitrary number of both outgoing and incoming
 * connections. */

class node;

/* Now for the node types. Each node type first applies the event to
 * its own counter, then propagates (or not) some event along all
 * outgoing connections. Implement the following node types:
 *
 *  • ‹forward› sends the same event it has received
 *  • ‹invert› sends the opposite event
 *  • ‹gate› resends the event if the new counter value is positive
 */

class forward;
class invert;
class gate;

int main()
{
    forward source, sink_0, sink_1;
    gate g_0, g_1;
    invert inv;

    const invert &c_inv = inv;

    assert( source.read() == 0 );
    assert( c_inv.read() == 0 );

    source.connect( g_0 );
    source.connect( inv );
    inv.connect( g_1 );

    g_0.connect( sink_0 );
    inv.connect( sink_0 );

    source.connect( sink_1 );
    g_0.connect( sink_1 );

    source.react( event::increment );

    assert( source.read() == 1 );
    assert( g_0.read()    == 1 );
    assert( inv.read()    == 1 );
    assert( g_1.read()    == -1 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 2 );

    source.react( event::increment );

    assert( source.read() == 2 );
    assert( g_0.read()    == 2 );
    assert( inv.read()    == 2 );
    assert( g_1.read()    == -2 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 4 );

    source.react( event::decrement );

    assert( source.read() == 1 );
    assert( g_0.read()    == 1 );
    assert( inv.read()    == 1 );
    assert( g_1.read()    == -1 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 2 );

    source.react( event::reset );

    assert( source.read() == 0 );
    assert( g_0.read()    == 0 );
    assert( inv.read()    == 0 );
    assert( g_1.read()    == 0 );
    assert( sink_0.read() == 0 );
    assert( sink_1.read() == 0 );

    source.react( event::decrement );

    assert( source.read() == -1 );
    assert( g_0.read()    == -1 );
    assert( inv.read()    == -1 );
    assert( g_1.read()    == 1 );
    assert( sink_0.read() == 1 );
    assert( sink_1.read() == -1 );

    return 0;
}
