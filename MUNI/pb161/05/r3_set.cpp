#include <cassert>

/* In this exercise, we will implement a set of «arbitrary»
 * integers, with the following operations: union using ‹|›,
 * intersection using ‹&›, difference using ‹-› and inclusion using
 * ‹<=›. Use efficient algorithms for the operations (check out
 * what's available in the standard header ‹algorithm›). Provide
 * methods ‹add› and ‹has› to add elements and test their presence.
 */

class set; /* reference implementation: 36 lines */

int main()
{
    set a, b;
    a.add( 1 ), a.add( 2 ), a.add( 5 ), a.add( -3 );
    b.add( 1 ), b.add( -3 ), b.add( -1000 ), b.add( 1 << 20 );

    const set x = a, y = b;

    const set u = x | y;
    const set i = x & y;
    const set d = x - y;

    assert(  u.has( 1 ) );
    assert(  i.has( 1 ) );
    assert( !d.has( 1 ) );

    assert(  u.has( 2 ) );
    assert( !i.has( 2 ) );
    assert(  d.has( 2 ) );
    assert( !i.has( -1000 ) );
    assert(  u.has( -1000 ) );
    assert( !d.has( -1000 ) );
    assert(  u.has( 1 << 20 ) );
    assert( !i.has( 1 << 20 ) );

    assert( !( a <= b ) );
    assert( !( b <= a ) );
    assert( a <= u );
    assert( b <= u );
    assert( i <= u );
    assert( i <= a );
    assert( i <= b );
    assert( u <= u );
    assert( d <= u );
    assert( d <= a );
    assert( !( d <= b ) );

    return 0;
}
