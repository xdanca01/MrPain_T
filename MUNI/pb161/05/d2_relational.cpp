#include <cassert>
#include <cstdint>

/* In this example, we will show relational operators, which are
 * very similar to the arithmetic operators from previous example,
 * except for their return types, which are ‹bool› values. */

/* The example which we will use in this case are sets of small
 * natural numbers (1-64) with inclusion as the order. We will
 * implement the full set of comparison operators, which is still
 * required in C++17 but will no longer be needed in C++20 (with the
 * spaceship operator).
 *
 * NB. Standard ordered containers like ‹std::set› and ‹std::map›
 * require the operator less-than to define a «linear» order. The
 * comparison operators in this example do «not» define a linear
 * order. */

class set
{
    /* Each bit of the below number indicates the presence of the
     * corresponding integer (the index of that bit) in the set. */

    uint64_t bits;
public:

    /* Like before, we add an explicit constructor that takes an
     * initial value. We use a «default argument» to say that the
     * constructor can be used as a default constructor (without
     * arguments), in which case it will create an empty ‹set›. */

    explicit set( uint64_t to_set = 0 ) : bits( to_set ) {}

    /* We also define a few methods to add and remove numbers from
     * the set, to test for presence of a number and an emptiness
     * check. */

    void add( int i ) { bits |=    1ul << i; }
    void del( int i ) { bits &= ~( 1ul << i ); }
    bool has( int i ) const { return bits & ( 1ul << i ); }
    bool empty() const { return !bits; }

    /* We will use the method syntax here, because it is slightly
     * shorter. We start with (in)equality, which is very simple
     * (the sets are equal when they have the same members): */

    bool operator==( set b ) const { return bits == b.bits; }
    bool operator!=( set b ) const { return bits != b.bits; }

    /* It will be quite useful to have set difference to implement
     * the comparisons below, so let us also define that: */

    set operator-( set b ) const { return set( bits & ~b.bits ); }

    /* Since the non-strict comparison (ordering) operators are
     * easier to implement, we will do that first. Set ‹b› is a
     * superset of set ‹a› if all elements of ‹a› are also present
     * in ‹b›, which is the same as the difference ‹a - b› being
     * empty. */

    bool operator<=( set b ) const { return ( *this - b ).empty(); }
    bool operator>=( set b ) const { return ( b - *this ).empty(); }
};

/* And finally the strict comparison operators, which are more
 * conveniently written using top-level function syntax: */

bool operator<( set a, set b ) { return a <= b && a != b; }
bool operator>( set a, set b ) { return a >= b && a != b; }

int main() /* demo */
{
    set a; a.add( 1 ); a.add( 7 ); a.add( 13 );
    set b; b.add( 1 ); b.add( 6 ); b.add( 13 );

    /* In each pair of assertions below, the two expressions are not
     * quite equivalent. Do you understand why? */

    assert( a != b ); assert( !( a == b ) );
    assert( a == a ); assert( !( a != a ) );

    /* The two sets are incomparable, i.e. neither is less than the
     * other, but as shown above they are not equal either. */
    assert( !( a < b ) ); assert( !( b < a ) );

    a.add( 6 ); // let's make ‹a› a superset of ‹b›

    /* And check that the ordering operators work on ordered sets. */

    assert( a > b ); assert( a >= b ); assert( a != b );
    assert( b < a ); assert( b <= a ); assert( b != a );

    b.add( 7 ); /* let's make the sets equal */
    assert( a == b ); assert( a <= b ); assert( a >= b );
}

/* That's all regarding relational operators, you will have a chance
 * to implement your own in one of the exercises later. In the
 * meantime, let us move on to ‘access’ operators: dereference,
 * indirect access and indexing, in ‹access.cpp›. */
