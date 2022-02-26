#include <cassert>

/* Implement our favourite data structure – a zipper of integers –
 * this time using a unique_ptr-linked list extending both ways from
 * the focus. Methods:
 *
 *  • (constructor) constructs a singleton zipper from an integer,
 *  • ‹shift_left› and ‹shift_right› move the point of focus, in
 *    O(1), to the nearest left (right) element; they return true if
 *    this was possible, otherwise they return false and do nothing,
 *  • ‹push_left› and ‹push_right› add a new element just left
 *    (just right) of the current focus, again in O(1),
 *  • ‹focus› access the current item (read and write). */

int main()
{
    zipper a( 7 ); // [7]
    const zipper &c = a;

    assert( c.focus() == 7 ); /* constant read works */
    assert( !a.shift_left() );
    assert( c.focus() == 7 );
    assert( !a.shift_right() );
    assert( c.focus() == 7 );

    a.push_left( 3 );        // 3 [7]
    assert( c.focus() == 7 );
    assert( a.shift_left() );  // [3] 7
    assert( c.focus() == 3 );
    assert( !a.shift_left() );
    assert( a.shift_right() ); // 3 [7]
    assert( c.focus() == 7 );
    assert( !a.shift_right() );

    a.push_left( 2 );   // 3 2 [7]
    a.push_right( 4 );  // 3 2 [7] 4
    a.push_right( 11 ); // 3 2 [7] 11 4
    a.focus() = 8; /* assignment to focus() works */

    assert( c.focus() == 8 );
    assert( a.shift_right() ); // 3 2 8 [11] 4
    assert( c.focus() == 11 );
    assert( a.shift_right() ); // 3 2 8 11 [4]
    assert( !a.shift_right() );

    assert( a.shift_left() ); // 3 2 8 [11] 4
    assert( a.focus() == 11 );
    assert( a.shift_left() ); // 3 2 [8] 11 4
    assert( a.focus() == 8 );
    assert( a.shift_left() ); // 3 [2] 8 11 4
    assert( a.focus() == 2 );
    assert( a.shift_left() ); // [3] 2 8 11 4
    assert( a.focus() == 3 );
    assert( !a.shift_left() );

    return 0;
}
