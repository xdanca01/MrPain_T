#include <cassert>

/* In this exercise, we will implement a very simple ‘string
 * builder‘: a class that will help us create strings from smaller
 * pieces. It will have a single overloaded method called ‹add›, in
 * 3 variants: it will accept either a string, an integer or a
 * floating-point number (use ‹std::to_string› for conversions).
 *
 * To make it easier to use, ‹add› should return a reference to the
 * instance it was called on. See below for examples. The method
 * ‹get› should return the constructed string. */

class string_builder;

int main()
{
    string_builder b;
    b.add( "hello " ).add( 42 );
    assert( b.get() == "hello 42" );
    b.add( ", pi = " ).add( 3.1415 );
    assert( b.get() == "hello 42, pi = 3.141500" );

    string_builder c;
    assert( c.add( "hello" ).get() == "hello" );

    const string_builder &d = c;
    c.add( " world" );
    assert( d.get() == "hello world" );

    string_builder e;
    e.add( 1 ).add( "x" ).add( 3.14 ).add( "a" ).get();
    assert( e.get() == "1x3.140000a" );

    return 0;
}
