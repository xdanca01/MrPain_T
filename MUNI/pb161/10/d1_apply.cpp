#include <tuple>
#include <cassert>

/* In this example, we will show how to use recursion together with
 * templates to walk through composite, templated data types. In
 * particular, we will look at finding and summing up numeric data
 * types in a binary tree made of ‹std::tuple› instances. */

/* We will need a data type to stop the recursive data definitions:
 * an empty tree, if you will. We will call it ‹null› (not to be
 * confused with the C macro ‹NULL› nor with C++ ‹nullptr›. We want
 * this to be a unique data type, but it does not need to carry any
 * actual data, hence we can use an empty ‹struct›. */

struct null {};

/* The summation will be defined recursively, so let's first define
 * the overload for the base type: ‹null›. The neutral element of
 * addition is 0, so let's use 0 as the value of an empty subtree.
 */

int sum( null ) { return 0; }

/* Now for the non-empty subtrees: we will use 3-tuples: the value
 * in the node (integer) and the left and right subtrees. We will
 * use template argument deduction to obtain the type of the
 * composite tuple. Recall that we used to write function templates
 * somewhat like this:
 *
 *     template< typename T >
 *     int sum( const T &tup )
 *     {
 *         int v = std::get< 0 >( tup );
 *         // ...
 *     }

 * This is not optimal, because there is a conflict with the ‹null›
 * overload above: the template can be instantiated with ‹T = null›.
 * The compiler will prefer the non-template version (or rather the
 * most specific version), but the rules are complex and
 * error-prone. It is better to not rely on those rules if they can
 * be easily avoided. */

/* In this case, we can use a «more specific» (non-overlapping)
 * overload, which will only accept 3-tuples. There is no chance
 * that a ‹null› is confused for a 3-tuple. Nonetheless, we still
 * need to figure out how to do template argument deduction in this
 * case. Easier shown than described. We will use 2 template type
 * parameters, for the left and right subtree. */

template< typename L, typename R >

/* However, we cannot directly use L and R as function arguments: we
 * want to accept 3-tuples. Fortunately, the compiler can also
 * deduce «parts» of an argument type: */

int sum( const std::tuple< int, L, R > &tup )
{
    /* We can also use structured bindings to decompose the tuple,
     * making the code easier to read: */

    const auto &[ v, left, right ] = tup;

    /* The rest of the function now looks like the most
     * straightforward recursive definition from IB015. */

    return v + sum( left ) + sum( right );
}

int main() /* demo */
{
    std::tuple a{ 3, null(), null() };
    std::tuple b{ 7, null(), null() };
    std::tuple c{ 1, null(), null() };
    std::tuple d{ 10, a, null() };
    std::tuple e{ 2, b, c };
    std::tuple f{ 0, d, e };

    assert( sum( null() ) == 0 );
    assert( sum( a ) == 3 );
    assert( sum( b ) == 7 );
    assert( sum( c ) == 1 );
    assert( sum( d ) == 13 );
    assert( sum( e ) == 10 );
    assert( sum( f ) == 23 );
}
