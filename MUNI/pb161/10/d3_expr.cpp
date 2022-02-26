#include <cassert>

/* This example will demonstrate operator overloading in conjunction
 * with class templates. Again, we will use argument deduction with
 * partial types in function signatures, to match the desired types
 * closely enough to to avoid ambiguities. You can probably imagine
 * than an operator ‹+› that accept arbitrary types as arguments
 * would not mesh very well with the rest of the program. */

/* First, we will define an ‹enum› to tag expressions with the
 * operator they represent, and a ‹constant› class to use as leaf
 * nodes. */

enum class expr_op { add, mul };

/* Constants will be a straightforward class with an ‹eval› method,
 * common with the ‹expr› class template below: */

struct constant
{
    int v;
    int eval() const { return v; }
    constant( int v ) : v( v ) {}
};

/* We will start by defining an ‹expr› class template. */

template< typename left_t, typename right_t >
struct expr
{
    expr_op op;
    left_t left;
    right_t right;

    expr( expr_op op, const left_t &l, const right_t &r )
        : op( op ), left( l ), right( r )
    {}

    /* Compute the value of this node. */

    int eval() const
    {
        int l = left.eval(),
            r = right.eval();

        switch ( op )
        {
            case expr_op::add: return l + r;
            case expr_op::mul: return l * r;
        }

        assert( false );
    }

    /* Like with normal operator overloading, there are multiple
     * ways to overload operators for class templates. Let's start
     * by defining a method. However, we immediately run into a
     * problem: the right operand does not have to be of the same
     * «type» as the left one, even though we want it to be an
     * instance of the same class template. For that reason, we need
     * to define the operator as a template method. */

    template< typename l2_t, typename r2_t >

    /* The return type is mildly infuriating, because it needs to
     * spell out the composite instance. Next time, we will just use
     * ‹auto›. */

    expr< expr< left_t, right_t >, expr< l2_t, r2_t > >

    /* Now for the signature of the operator itself: */

    operator+( const expr< l2_t, r2_t > &o ) const
    {
        /* Now that we have spelled out the monster types, the
         * implementation is trivial. */

        return { expr_op::add, *this, o };
    }

    /* Now let's try a ‹friend› definition. The gist is the same,
     * and you may remember that we can still use ‹expr› without
     * arguments to mean the instance with ‹left_t = left_t› and
     * ‹right_t = right_t›. Then: */

    template< typename l2_t, typename r2_t >
    friend auto operator*( const expr &a,
                           const expr< l2_t, r2_t > & b )
    {
        /* But now we have a problem again. We are in the definition
         * of a class template, and hence using the name of the
         * class template without arguments means «this specific
         * instance». But we want to construct a different instance,
         * but using template argument deduction. We need to tell
         * the compiler that is what we mean by using a qualified
         * name for the class template: if qualified, the name no
         * longer refers to this instance. */

        return ::expr( expr_op::mul, a, b );
    }

    /* That covers the expression + expression cases. But we also
     * need to be able to work with ‹constant› instances here. More
     * operators! */

    friend auto operator+( constant c, const expr &a )
    {
        return ::expr( expr_op::add, c, a );
    }

    friend auto operator+( const expr &a, constant c )
    {
        return ::expr( expr_op::add, a, c );
    }

    friend auto operator*( constant c, const expr &a )
    {
        return ::expr{ expr_op::mul, c, a };
    }

    friend auto operator*( const expr &a, constant c )
    {
        return ::expr( expr_op::mul, a, c );
    }
};

/* That's not the end yet. We also need to be able to multiply and
 * add two constants, to get a complete set. Since the result is an
 * ‹expr› instance, it does not make much sense to put those into
 * the ‹constant› class itself. Let's use toplevel definitions for
 * those. Fortunately, in this case, the operators are not templates
 * at least. */

auto operator+( constant a, constant b )
{
    return expr( expr_op::add, a, b );
}

auto operator*( constant a, constant b )
{
    return expr( expr_op::mul, a, b );
}

int main() /* demo */
{
    constant a( 1 );
    constant b( 2 );
    auto c = a + b;
    assert( c.eval() == 3 );
    assert( ( a * c ).eval() == 3 );
    assert( ( a + c ).eval() == 4 );
    assert( ( c + c ).eval() == 6 );
}
