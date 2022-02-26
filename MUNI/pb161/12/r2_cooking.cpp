#include <cassert>
#include <string>

/* In this exercise, we will implement a simple model of cooking,
 * with recipes and a pantry. Try to think about code duplication
 * and whether you can reduce it and what is the cost of reduction
 * in duplication. */

/* The class ‹pantry› will keep a list of available ingredients and
 * their quantity. It should be default-constructible and offer a
 * method ‹add›, which takes a string (the name of the ingredient)
 * and an integer (the quantity). A ‹const› method ‹count› should
 * take a string (name of the ingredient) and return the quantity
 * available (possibly 0). */

class pantry;

/* We will use another class to represent recipes (in our simplified
 * world, a list of ingredients and quantities required to cook a
 * meal). Like ‹pantry›, it should be default-constructible and
 * offer a method ‹add›, which accepts 2 or 3 arguments: name, the
 * «required» quantity and, if supplied, an «optional» quantity of
 * the ingredient that will be used if available («in addition to»
 * the required amount) but is not required to cook the meal. */

class recipe;

/* Finally, implement function ‹cook› with 3 arguments: a «mutable»
 * reference to the ‹pantry› which will be used to obtain the
 * ingredients, a ‹const› reference to the ‹recipe› to cook and an
 * ‹int›, the number of portions to prepare. The function then
 * returns ‹true› if everything went okay (and of course deducts the
 * ingredients used up from the ‹pantry›) or ‹false› if some
 * ingredient was missing or there wasn't enough of it, in which
 * case the ‹pantry› content remains unchanged. */

bool cook( pantry &, const recipe &, int qty );

int main()
{
    pantry p;
    const pantry &cp = p;

    p.add( "spinach", 3 );
    p.add( "pea can", 2 );
    p.add( "potato", 23 );
    p.add( "tofu", 7 );
    p.add( "onion", 5 );

    recipe spinach;
    spinach.add( "spinach", 1 );
    spinach.add( "potato", 3 );
    spinach.add( "onion", 1 );
    spinach.add( "tofu", 0, 1 );

    recipe tofu;
    tofu.add( "potato", 3 );
    tofu.add( "tofu", 1, 1 );
    tofu.add( "onion", 1 );
    tofu.add( "pea can", 0, 1 );

    assert( cp.count( "potato" ) == 23 );
    assert( cook( p, tofu, 1 ) );
    assert( cp.count( "potato" ) == 20 );
    assert( cp.count( "tofu" ) == 5 );
    assert( cp.count( "pea can" ) == 1 );
    assert( cp.count( "onion" ) == 4 );

    assert( !cook( p, tofu, 5 ) );
    assert( cp.count( "potato" ) == 20 );
    assert( cp.count( "tofu" ) == 5 );
    assert( cp.count( "pea can" ) == 1 );
    assert( cp.count( "onion" ) == 4 );

    assert( cook( p, tofu, 2 ) );
    assert( cp.count( "potato" ) == 14 );
    assert( cp.count( "tofu" ) == 1 );
    assert( cp.count( "pea can" ) == 1 );
    assert( cp.count( "onion" ) == 2 );

    p.add( "potato", 3 );
    assert( cp.count( "potato" ) == 17 );
    assert( cook( p, spinach, 2 ) );
    assert( cp.count( "potato" ) == 11 );
    assert( cp.count( "spinach" ) == 1 );
    assert( cp.count( "onion" ) == 0 );

    return 0;
}
