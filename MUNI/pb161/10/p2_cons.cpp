#include <cassert>
#include <utility>
#include <functional>

/* We will elaborate a little on the topic from ‹icons.cpp›, by
 * making the type of ‹car› into a template argument. That way, we
 * will be able to make a list that has items of different types in
 * it. */

/* Generalize ‹cons› from the previous exercise and write a
 * ‹reduce› function that takes an arbitrary ‹cons› instance, a
 * function object (e.g. a lambda) and an initial accumulator value.
 * The function object must be able to accept the accumulator as its
 * first argument, and an arbitrary value that appears in the ‹cons›
 * list as its second argument. */

/* null, cons, reduce */

/* callable object with overloads, for testing */
struct collect
{
    using pair = std::pair< int, double >;

    pair operator()( pair p, int i ) const
    {
        p.first += i;
        return p;
    }

    pair operator()( pair p, double d ) const
    {
        p.second += d;
        return p;
    }
};

int main()
{
    cons c( 1, cons( 2, null() ) );
    assert( reduce( c, std::plus<>(), 0 ) == 3 );

    cons d( 1.5, cons( 1, null() ) );
    assert( reduce( d, std::plus<>(), 0.0 ) == 2.5 );
    assert( reduce( d, collect(), std::pair( 0, 0.0 ) ) == std::pair( 1, 1.5 ) );

    return 0;
}
