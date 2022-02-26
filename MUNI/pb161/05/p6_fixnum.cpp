#include <cassert>

/* In this exercise, we will implement fixed-precision numbers, with
 * 2 fractional digits and up to 6 integral digits (both decimal),
 * i.e. numbers of the form ‘123456.78’. */

/* This is the class which we will use for indicating that parsing
 * of the ‹fixnum› has failed (i.e. this class will be thrown as an
 * exception in that case). */

class bad_format;

/* The ‹fixnum› class should provide following operations: addition,
 * subtraction and multiplication. It should have «explicit»
 * constructors which construct the number from an integer or from
 * a string. The latter constructor should throw an exception if the
 * string is ill-formed (it is okay to only handle positive numbers
 * in string form). Finally, it should be possible to compare
 * ‹fixnum› instances for equality. All operations should round
 * toward zero, to the nearest representable number. */

class fixnum; /* reference implementation: 32 lines */

int main()
{
    assert( fixnum( 10 ) == fixnum( "10" ) );
    assert( fixnum( 1 )  == fixnum( "1" ) );

    fixnum a( 10 ), b( "0.5" ), c( "5.5" );
    const fixnum d( "0.01" ), e( "0.05" );

    assert( a + b + b == fixnum( 11 ) );
    assert( c + b == fixnum( 6 ) );
    assert( c + c - fixnum( 1 ) == a );
    assert( a * a * d == fixnum( 1 ) );
    assert( fixnum( 5 ) * d == e );
    assert( a * b == fixnum( 5 ) );
    assert( d * d == fixnum( 0 ) );
    assert( a * a == fixnum( 100 ) );
    assert( e * fixnum( 100 ) == fixnum( 5 ) );
    assert( e * c == fixnum( "0.27" ) );
    assert( fixnum( -1 ) * e * c == fixnum( -1 ) * fixnum( "0.27" ) );

    using badfmt = const bad_format &;

    try { fixnum( "x" );   assert( false ); } catch ( badfmt ) {}
    try { fixnum( "x.1" ); assert( false ); } catch ( badfmt ) {}
    try { fixnum( "1.." ); assert( false ); } catch ( badfmt ) {}
    try { fixnum( "1.x" ); assert( false ); } catch ( badfmt ) {}
    try { fixnum( "1,1" ); assert( false ); } catch ( badfmt ) {}

    return 0;
}
