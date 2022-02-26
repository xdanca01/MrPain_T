#include <cassert>
#include <set>
#include <vector>

/* In this unit, we will look at «function templates», which are
 * similar to class templates we have seen in previous units.
 * Function templates rely even more heavily on template parameter
 * deduction than class templates: most of the time, calling
 * function templates looks just like calling standard function: the
 * compiler will «deduce» the type parameters from the actual
 * argument types. We will see that later down.
 *
 * One further thing to note is that we have actually met function
 * templates quite early on, we just did not mention they were
 * templates: the call operator of a «lambda» with an ‹auto›
 * parameter is, in fact, a function template, the only difference
 * is that the syntax is (usually) less verbose. */

/* We will start by defining some commonly useful folds on
 * containers. Let's start with ‹sum›. The container type will be a
 * «type parameter»: we want our ‹sum› to work on different
 * container types (for instance sets and vectors) and also with
 * different «element types»: the types of items stored in those
 * containers. The syntax for function templates is pretty much the
 * same as it was for class templates: */

template< typename container_t >

/* followed by a standard function signature. In this case, we have
 * a small problem, since we don't have a name for the type of the
 * return value. For now, we can use ‹auto›. */

auto sum( const container_t &xs )
{
    /* There are two ways to go about writing the summing loop, with
     * different trade-offs in terms of types. Probably the most
     * reasonable thing to do is to declare an accumulator of the
     * correct type and initialize it to 0. For that, however, we
     * need to know the type of the values stored in ‹xs›.
     * Fortunately, standard library provides us with a way to do
     * just that: each standard container has a «nested type name»,
     * which we can access using ‹::›. If the outer type is a
     * template argument, or depends on a template argument, we
     * additionally need to tell the compiler that we intend to
     * refer to a type (since templates are «duck typed», the nested
     * name could also turn out to be an attribute or a method). The
     * type of a single element stored in a container is known as
     * its ‹value_type›. */

    using value_t = typename container_t::value_type;

    /* Now that we have named the type of values in the container,
     * we can declare an accumulator with the correct type. Again,
     * by the virtue of duck typing, we do not know for certain
     * whether values of this type can be constructed from integers,
     * but we assume that they can. When we attempt to use the
     * template, the compiler will check and emit errors if this is,
     * in fact, not possible. */

    value_t accum = 0;

    /* The loop itself is then quite trivial. */

    for ( const auto &x : xs )
        accum = accum + x;

    return accum;
}

/* Let's also try to do a product, in a slightly different style,
 * just to see some more options. In this case, since we do not make
 * any use of ‹container_t›, it would be easier to simply use a
 * lambda. We will do that in a bit. */

template< typename container_t >
auto product( const container_t &xs )
{
    auto accum = xs.empty() ? 1 : *xs.begin();
    bool first = true;

    for ( const auto &x : xs )
        if ( first )
            first = false;
        else
            accum = accum * x;

    return accum;
}

/* Let us do ‹mean› in a «lambda» style, so we have a comparison at
 * hand. We can re-use ‹sum› from above. We will take the average of
 * an empty sequence to be 0. */

static auto mean = []( const auto &xs )
{
    return xs.empty() ? 0 : sum( xs ) / xs.size();
};

/* Finally, we will generalize the two folds (sum and product), and
 * add a ‹zip_with› for a good measure, in the ‹template› style. We
 * can accept «functions as arguments» the same way we accept any
 * other values. This will work with anything that can be called
 * (remember «duck typing»?), most importantly lambdas.
 *
 * The initial value of the accumulator passed in by the user gives
 * us the type of the accumulator ‘for free’. In practice, this is a
 * little dangerous in the sense that it could give us some
 * unexpected results if enough implicit conversions are allowed
 * (like accumulating rational numbers into an integer). I will show
 * you another version of ‹fold› as a bonus after we do ‹zip_with›.
 */

template< typename xs_t, typename fun_t, typename init_t >
auto fold( const xs_t &xs, const fun_t &f, const init_t &init )
{
    /* The fold itself is pretty trivial, once we have figured out
     * the types. */

    init_t accum = init;
    for ( const auto &x : xs )
        accum = f( accum, x );
    return accum;
}

/* Now for the ‹zip_with›.  It will accept two sequences and a
 * function. The result will be a ‹vector›, since we do not have a
 * good way to tell the function what type of a container we would
 * like. */

template< typename xs_t, typename ys_t, typename fun_t >
auto zip_with( const xs_t &xs, const ys_t &ys, const fun_t &f )
{
    /* We need a new trick, because there is nothing at hand that
     * would give us the element type of the result (even if we
     * settled for a ‹vector› as the container). The way to find out
     * is ‹decltype›, an operator that takes an «expression» and
     * produces its «type». Whenever we can write out a name of a
     * type, we can instead use ‹decltype› with an expression. The
     * expression must only refer to names that are in scope at the
     * point of the ‹decltype› though. */

    using value_t = decltype( f( *xs.begin(), *ys.begin() ) );

    /* Note: there is a bit of a danger in the above: this function
     * will not work with an ‹f› that returns a reference. Repairing
     * this deficiency is beyond the scope of this course. Ask if
     * you are interested though. */

    std::vector< value_t > out;

    /* We want our ‹zip_with› to terminate when it hits the end of
     * the shorter sequence. This means we cannot use
     * ‹std::transform›, unfortunately, so we will type out the loop
     * by hand. */

    auto x = xs.begin();
    auto y = ys.begin();

    while ( x != xs.end() && y != ys.end() )
        out.push_back( f( *x++, *y++ ) );

    return out;
}

/* And finally, the promised ‘bonus’ ‹fold›, which prefers the
 * return type of ‹f› as its accumulator type. We have the basic
 * recipe for that in ‹zip_with›. */

template< typename xs_t, typename fun_t, typename init_t >
auto fold_( const xs_t &xs, const fun_t &f, const init_t &init )
{
    using accum_t = decltype( f( init, *xs.begin() ) );

    /* Note that ‹accum_t› and ‹init_t› may be different types. */

    accum_t accum = init;

    for ( const auto &x : xs )
        accum = f( accum, x );

    return accum;
}

/* For a good measure, we will define a custom class of numbers. You
 * might remember ‹rat› from an earlier exercise. The minimum viable
 * definition follows. */

struct rat
{
    int p, q;
    rat( int p, int q = 1 ) : p( p ), q( q ) {}

    friend rat operator+( rat r, rat s )
    {
        return { r.p * s.q + s.p * r.q, r.q * s.q };
    }

    rat operator*( rat r ) const { return { p * r.p, q * r.q }; }
    rat operator/( rat r ) const { return { p * r.q, q * r.p }; }

    bool operator<( rat r )  const { return p * r.q <  r.p * q; }
    bool operator==( rat r ) const { return p * r.q == r.p * q; }
};

int main() /* demo */
{
    std::set< int > xs{ 1, 2, 3 };
    std::vector< double > ys{ 1.5, 2 };
    std::set< rat > zs{ 1, { 1, 2 }, { 1, 4 } };

    /* The only interesting thing in the below test cases is that
     * the functions are used like standard functions: no angle
     * brackets to be seen anywhere. This is because the compiler
     * «deduces» the type parameters from the types of the actual
     * arguments which we pass into the function. Since all template
     * arguments can be deduced this way, we can omit angle brackets
     * entirely. */

    assert( sum( xs ) == 6 );
    assert( sum( ys ) == 3.5 );
    assert( sum( zs ) == rat( 7, 4 ) );

    assert( product( xs ) == 6 );
    assert( product( ys ) == 3 );
    assert( product( zs ) == rat( 1, 8 ) );

    assert( mean( xs ) == 2 );
    assert( mean( ys ) == 1.75 );
    assert( mean( zs ) == rat( 7, 12 ) );

    /* When calling our original ‹fold›, we have to be careful with
     * the type of the initial value, otherwise we will run into
     * problems. This is somewhat inconvenient. */

    assert( fold( zs, std::plus<>(), rat( 0 ) ) == rat( 7, 4 ) );

    /* On the other hand, our improved version (here named ‹fold_›)
     * works just fine if we write it in a ‘natural’ style. */

    assert( fold_( zs, std::plus<>(), 0 ) == rat( 7, 4 ) );

    /* Finally, let's look at ‹zip_with›. */

    std::vector xs_ys{ 2.5, 4.0 };

    /* The sets are «sorted» in ascending order, so the pairings
     * will be 1/4 + 1, 1/2 + 2 and 1 + 3. */

    std::vector xs_zs{ rat( 5, 4 ), rat( 5, 2 ), rat( 4 ) };

    assert( zip_with( xs, ys, std::plus<>() ) == xs_ys );
    assert( zip_with( xs, zs, std::plus<>() ) == xs_zs );
}
