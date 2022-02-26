#include <cassert>
#include <vector>

/* Everyone's favourite: deterministic finite state machines. We
 * will write a class template that will let us decide whether a
 * generalized word (a sequence of values of a type equipped with
 * equality) belongs to a regular language described by a given
 * finite automaton (finite state machine) or not. The type which
 * represents individual letters is given to ‹fsm› as a type
 * parameter. */

/* The constructor of ‹fsm› should accept a single boolean (mark the
 * constructor ‹explicit›), which determines if the state
 * represented by the instance is accepting or not. A
 * default-constructed ‹fsm› should be non-accepting. Make the
 * following methods available:
 *
 *  • ‹next› which accepts a letter (of type ‹letter_t›) and a ‹const›
 *    reference to another ‹fsm› instance; the method adds a
 *    transition to the automaton,
 *  • ‹accept› which takes a sequence of ‹letter_t› values (the type
 *    of the sequence is not fixed, but it can be iterated using a
 *    range ‹for›) and returns ‹true› if the automaton accepts the
 *    word stored in the sequence; this method should be marked
 *    ‹const›. */

// … class fsm;

int main()
{
    using namespace std::literals;
    // note: "…"sv is an instance of ‹std::string_view›

    fsm< char > a( true ), b, c, d;
    std::vector zx{ 'z', 'x' };

    assert(  a.accept( ""sv ) );
    assert( !a.accept( "x"sv ) );
    assert( !b.accept( ""sv ) );

    b.next( 'x', a );
    a.next( 'x', a );

    assert( a.accept( ""sv ) );
    assert( a.accept( "x"sv ) );
    assert( a.accept( "xx"sv ) );
    assert( a.accept( "xxx"sv ) );

    c.next( 'z', a );
    assert( c.accept( "z"sv ) );
    assert( c.accept( "zx"sv ) );
    assert( c.accept( zx ) );

    d.next( 'y', d );
    d.next( 'z', c );

    assert( !d.accept( ""sv ) );
    assert( !d.accept( "y"sv ) );
    assert( !d.accept( "yy"sv ) );
    assert( !d.accept( "yyz"sv ) );
    assert(  d.accept( "yyzz"sv ) );
    assert(  d.accept( "yyzzx"sv ) );
    assert( !d.accept( "yyzzzx"sv ) );
    assert(  d.accept( "yyzzxx"sv ) );

    fsm< int > i_a( true );
    i_a.next( 3, i_a );
    i_a.next( 5, i_a );

    std::vector< int > v;
    assert( i_a.accept( v ) );
    v.push_back( 3 );
    assert( i_a.accept( v ) );
    v.push_back( 5 );
    assert( i_a.accept( v ) );
    v.push_back( 3 );
    assert( i_a.accept( v ) );
    v.push_back( 7 );
    assert( !i_a.accept( v ) );

    return 0;
}
