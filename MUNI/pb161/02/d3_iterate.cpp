#include <cassert>
#include <vector>

/* In this short demo, we will introduce new syntax for writing
 * functions. The type of function we will use is called a «lambda»,
 * from the symbol that is used in «lambda calculus» to introduce
 * anonymous functions. In C++, lambdas are like regular functions
 * with a few extras. */

/* Notice that ‹iterate› is declared as a «variable» – the function
 * is on the right-hand side, and does not have an intrinsic name
 * (i.e. it is anonymous). The «type» of ‹iterate› is not
 * specified – instead, we have used ‹auto›, to instruct the
 * compiler to fill in the type.
 *
 * Besides the missing name and the empty square brackets, the
 * signature of the lambda is similar to a standard function.
 * However, on closer inspection, another thing is missing: the
 * return type. This might be specified using ‹-> type› after the
 * argument list, but if it is not, the compiler will, again, deduce
 * the type for us. The return type is commonly omitted. */

auto iterate = []( auto f, auto x, int count )

/* An advantage of a «lambda» is that we do not need to know the
 * types of all the arguments in advance: in particular, we don't
 * know the type of ‹f› – this will most likely be a lambda itself
 * (i.e. ‹iterate› is a higher-order function). When this is the
 * case, instead of the type, we specify ‹auto›, instructing the
 * compiler to deduce a type when the function is used. This is the
 * same principle which we have applied to the «variable» ‹iterate›
 * itself: we do not know the type, so we ask the compiler to fill
 * it in for us (by using ‹auto›). Let us continue by writing the
 * body of ‹iterate›: */
{
    /* We want to build a vector of values, starting with ‹x›, then
     * ‹f(x)›, ‹f(f(x))›, and so on. Immediately, we face a problem:
     * what should be the type of the vector? We need to specify the
     * type parameter to declare the variable, and this time we
     * won't be able to weasel out by just saying ‹auto›, since the
     * compiler can't tell the type without an unambiguously typed
     * initializer. We have two options here:

     *  1. in some circumstances, it is possible to omit the type
     *     parameter of ‹std::vector› and let the compiler deduce
     *     only that. This would be written ‹std::vector out{ x }›
     *     – by putting ‹x› into the vector right from the start, the
     *     compiler can deduce that the element type should be the
     *     same as the type of ‹x›, whatever that is; we will deal
     *     with this mechanism much later in the course (in the last
     *     block); in the meantime,
     *  2. we can use ‹decltype› to obtain the type of ‹x› and use
     *     that to specify the required type parameter for ‹out›,
     *     i.e.: */

    std::vector< decltype( x ) > out;
    out.push_back( x );

    /* We build the return vector by repeatedly calling ‹f› on the
     * previous value, until we hit ‹count› items. */

    for ( int i = 1; i < count; ++ i )
        out.push_back( f( out.back() ) );

    /* And we return the value, like in a regular function. Please
     * also note the semicolon after the closing brace: definition
     * of a lambda is an «expression», and the variable declaration
     * as a whole needs to be delimited by a semicolon, just like in
     * ‹int x = 7;›. */

    return out;
};

int main() /* demo */
{
    auto f = []( int x ) { return x * x; };
    auto g = []( int x ) { return x + 1; };

    /* Of course, we can use ‹auto› in declaration of regular
     * variables too, as long as they are initialized. */

    auto v = iterate( f, 2, 4 );

    std::vector< int > expect{ 2, 4, 16, 256 };
    assert( v == expect );

    std::vector< int >
        iota = iterate( g, 1, 4 ),
        iota_expect{ 1, 2, 3, 4 };

    assert( iota == iota_expect );
}
