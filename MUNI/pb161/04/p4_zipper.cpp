#include <cassert>

/* In this exercise, we will implement a simple data structure
 * called a «zipper» -- a sequence of items with a single «focused»
 * item. Since we can't write class templates yet, we will just
 * make a zipper of integers. Our zipper will have these operations:
 *
 *  • (constructor) constructs a singleton zipper from an integer
 *  • ‹shift_left› and ‹shift_right› move the point of focus, in
 *    O(1), to the nearest left (right) element; they return true if
 *    this was possible, otherwise they return false and do nothing
 *  • ‹insert_left› and ‹insert_right› add a new element just left
 *    (just right) of the current focus, again in O(1)
 *  • ‹focus› access the current item (read and write)
 *  • bonus: add ‹erase_left› and ‹erase_right› to remove elements
 *    around the focus (return ‹true› if this was possible), in O(1)
 */

class zipper;

int main()
{
    zipper a( 7 ); // [7]
    const zipper &c = a;

    assert( c.focus() == 7 ); /* constant read works */
    assert( !a.shift_left() );
    assert( c.focus() == 7 );
    assert( !a.shift_right() );
    assert( c.focus() == 7 );

    a.insert_left( 3 );        // 3 [7]
    assert( c.focus() == 7 );
    assert( a.shift_left() );  // [3] 7
    assert( c.focus() == 3 );
    assert( !a.shift_left() );
    assert( a.shift_right() ); // 3 [7]
    assert( c.focus() == 7 );
    assert( !a.shift_right() );

    zipper b = a;

    a.insert_left( 2 );   // 3 2 [7]
    a.insert_right( 4 );  // 3 2 [7] 4
    a.insert_right( 11 ); // 3 2 [7] 11 4
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

    assert( b.focus() == 7 );
    assert( !b.shift_right() );

    return 0;
}
