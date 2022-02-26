#include <cassert>

/* We will do another take at expressions, this time with templates,
 * which will allow us to use values instead of references. While
 * the first two examples were directly comparable to earlier
 * versions, this one will deviate quite far from ‹07/expr.cpp›,
 * though you may still find it useful to quickly go over that one
 * first. The semantics will be the same: we will have sums,
 * products and constants. However, there will be no distinction
 * between static and dynamic types and no ‹virtual› methods. */

/* We will start with constants, since those are the simplest. Since
 * we are not using ‹virtual› methods, we also don't need a common
 * base class or inheritance. We do, however, need to provide a
 * common interface (i.e. method names and signatures) between the
 * different classes. */

class constant
{
    int _value;
public:
    constant( int v ) : _value( v ) {}
    int value() const { return _value; }
};

/* So far, we have only seen templates with a single type parameter.
 * However, we can have as many as we want (in fact, we can even
 * have a variable number, though that is outside of the scope of
 * this subject). For ‹sum›, we will need 2: the type of the left
 * and of the right sub-expression. */

template< typename left_t, typename right_t >
class sum
{

    /* Unlike before, the «static» type of the left and the right
     * sub-expressions may be different, and instead of references
     * or pointers, we will simply store them by value in
     * attributes. */

    left_t _left;
    right_t _right;

public:

    /* We need to define a constructor. Like in our earlier take on
     * expressions, the constructor will take the two
     * sub-expressions as arguments. This time, their types are
     * given by the template parameters though. Other than that, the
     * constructor is pretty normal. */

    sum( const left_t &l, const right_t &r )
        : _left( l ), _right( r )
    {}

    /* And the interface to get values out of the expression: */
    int value() const { return _left.value() + _right.value(); }
};

/* The ‹product› class looks pretty much the same: */

template< typename left_t, typename right_t >
class product
{
    left_t _left;
    right_t _right;

public:

    product( const left_t &l, const right_t &r )
        : _left( l ), _right( r )
    {}

    int value() const { return _left.value() * _right.value(); }
};

/* The duplication is somewhat unsatisfactory. Maybe we could do a
 * little better by using inheritance, so let's try defining another
 * class. First the base class: */

template< typename left_t, typename right_t >
class operation
{
protected:
    left_t _left;
    right_t _right;

public:
    operation( const left_t &l, const right_t &r )
        : _left( l ), _right( r )
    {}
};

/* Now a derived class -- let's do subtraction.  Remember that
 * inheritance works with classes, but ‹operation› is a class
 * ‹template›: we need to instantiate it to obtain a class before we
 * can inherit from it! */

template< typename left_t, typename right_t >
class difference : public operation< left_t, right_t >
{
public:
    difference( const left_t &l, const right_t &r )
        : operation< left_t, right_t >( l, r ) {}

    /* Plot twist: if the type of the base class depends on template
     * parameters, we cannot directly access inherited attributes.
     * Instead, we have to explicitly tell the compiler that those
     * are attributes of this class using ‹this›.*/

    int value() const
    {
        return this->_left.value() - this->_right.value();
    }
};

/* That wasn't much better. Templates are, unfortunately, somewhat
 * verbose. On the upside, notice that we have implemented the first
 * two operations (‹+›, ‹*›) somewhat differently from the last
 * (‹-›), but they can still interoperate smoothly. Templates use
 * «duck typing»: if it looks and quacks like a duck (i.e. it has
 * the right attributes and methods) it probably is a duck, and the
 * compiler will let us use the type with the template. */

int main() /* demo */
{
    /* We first define some constants, those are simple. */

    constant c_0( 0 ), c_1( 1 ), c_2( 2 );

    /* When we create instances of class templates using
     * constructors which take arguments of types that match the
     * type parameters of the template, we do not need to explicitly
     * type them out. This is the same principle that lets us write
     * ‹std::pair( 0, 1 )›. The feature is called «template argument
     * deduction» and we will see more of it with function templates
     * in the next unit. Of course, we can specify the template
     * arguments ourselves if we want to, but it gets tedious rather
     * quickly. We will show both styles, first the explicit one: */

    sum< constant, constant > s_1( c_0, c_1 );
    sum< sum< constant, constant >, constant > s_2( s_1, c_1 );

    /* This is clearly impractical. Let's try the implicit style. */

    sum s_3( s_2, c_1 );
    product p_0( c_0, c_1 );
    product p_1( c_1, s_1 );

    /* That is much better. Let's make some differences and then we
     * will check all the values. */

    difference d_2( s_3, s_1 );
    difference d_0( d_2, c_2 );

    assert( c_0.value() == 0 );
    assert( s_1.value() == 1 );
    assert( s_2.value() == 2 );
    assert( s_3.value() == 3 );
    assert( p_0.value() == 0 );
    assert( p_1.value() == 1 );
    assert( d_2.value() == 2 );
    assert( d_0.value() == 0 );
}
