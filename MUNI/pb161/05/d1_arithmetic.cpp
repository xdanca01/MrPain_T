#include <cassert>

/* Operator overloading allows instances of classes to behave more
 * like built-in types: it makes it possible for values of custom
 * types to appear in expressions, as operands. Before we look at
 * examples of how this looks, we need to define a class with some
 * overloaded operators. For binary operators, it is customary to
 * define them using a ‘friends trick’, which allows us to define a
 * top-level function inside a class. */

/* As a very simple example, we will implement a class which
 * represents integral values modulo 7 (this happens to be a finite
 * field, with addition and multiplication). */

class gf7
{
    int value;
public:

    /* The constructor is trivial, it simply constructs a ‹gf7›
     * instance from an integer. We mark it ‹explicit› to avoid
     * surprising automatic conversions of integers into ‹gf7›
     * instances. */

    explicit gf7( int v ) : value( v % 7 ) {}

    /* This is the ‘friend trick’ syntax for writing operators, and
     * for binary operators, it is often the preferred one (because
     * of its symmetry). The function is not really a part of the
     * class in this case -- the trick is that we can write it here
     * anyway. */

    friend gf7 operator+( gf7 a, gf7 b )
    {
        return gf7( a.value + b.value ); // [a]₇ + [b]₇ = [a + b]₇
    }

    /* For multiplication, we will use the more ‘orthodox‘ syntax,
     * where the operator is a ‹const› method: the left operand is
     * passed into the operator as ‹this›, the right operand is the
     * argument. In general, operators-as-methods have one explicit
     * argument less (unary operators take 0 arguments, binary take
     * 1 argument). */

    gf7 operator*( gf7 b ) const
    {
        return gf7( value * b.value ); // [a]₇ * [b]₇ = [a * b]₇
    }

    /* Values of type ‹gf7› cannot be directly compared (we did not
     * define the required operators) -- instead, we provide this
     * method to convert instances of ‹gf7› back into ‹int›'s. */

    int to_int() const { return value; }
};

/* Operators can be also overloaded using ‘normal’ top-level
 * functions, like this unary minus (which finds the additive
 * inverse of the given element). Notice that we cannot access
 * private fields (attributes) of the class here: */

gf7 operator-( gf7 x ) { return gf7( 7 - x.to_int() ); }

/* Now that we have defined the class and the operators, we can look
 * at how is the result used. */

int main() /* demo */
{
    gf7 a( 3 ), b( 4 ), c( 0 ), d( 5 );

    /* Values ‹a›, ‹b› and so forth can be now directly used in
     * arithmetic expressions, just as we wanted. */

    gf7 x = a + b;
    gf7 y = a * b;

    /* Let us check that the operations work as expected: */

    assert( x.to_int() == c.to_int() ); /* [3]₇ + [4]₇ = [0]₇ */
    assert( y.to_int() == d.to_int() ); /* [3]₇ * [4]₇ = [5]₇ */
    assert( (-a + a).to_int() == c.to_int() ); /* unary minus */
}

/* That was arithmetic operator overloading. Let's now look at
 * relational (ordering) operators, in ‹relational.cpp›. */
