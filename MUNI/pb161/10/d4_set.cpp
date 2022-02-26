#include <set>
#include <algorithm>
#include <cassert>

/* In which we will combine operator templates and template argument
 * deduction to spice up the standard ‹std::set› container. */

/* We have already seen in ‹apply.cpp› that the compiler can deduce
 * template arguments based on (self-contained) fragments of
 * function argument types. We will use that along with operator
 * templates to provide overloads for all instances of ‹std::set›,
 * without affecting any other standard container, or any other type
 * at all. */

/* We will overload operator ‹&› for intersection, operator ‹|› for
 * union, ‹-› for standard difference and finally ‹^› for symmetric
 * difference of two sets. Please keep in mind that the priorities
 * of bitwise operators in C++ are unintuitive at best: overloaded
 * operators inherit both their priority and associativity from the
 * built-in ones. */

template< typename T >
std::set< T > operator&( const std::set< T > &a,
                         const std::set< T > &b )
{
    std::set< T > out;

    /* Remember standard algorithms? */

    std::set_intersection( a.begin(), a.end(),
                           b.begin(), b.end(),
                           std::inserter( out, out.begin() ) );
    return out;
}

/* Now the union. */

template< typename T >
std::set< T > operator|( const std::set< T > &a,
                         const std::set< T > &b )
{
    std::set< T > out;

    std::set_union( a.begin(), a.end(),
                    b.begin(), b.end(),
                    std::inserter( out, out.begin() ) );
    return out;
}

/* And difference. This is getting a little boring. */

template< typename T >
std::set< T > operator-( const std::set< T > &a,
                         const std::set< T > &b )
{
    std::set< T > out;

    std::set_difference( a.begin(), a.end(),
                         b.begin(), b.end(),
                         std::inserter( out, out.begin() ) );
    return out;
}

/* And finally the symmetric difference. Surprise! */

template< typename T >
std::set< T > operator^( const std::set< T > &a,
                         const std::set< T > &b )
{
    return ( a | b ) - ( a & b );
}

int main() /* demo */
{
    std::set a{ 1, 2, 3 };
    std::set b{ 1, 3, 5 };

    std::set aib{ 1, 3 };
    std::set aub{ 1, 2, 3, 5 };
    std::set amb{ 2 };
    std::set axb{ 2, 5 };

    assert( ( a & b ) == aib );
    assert( ( a | b ) == aub );
    assert( ( a - b ) == amb );
    assert( ( a ^ b ) == axb );

    assert( ( a & b ) == ( b & a ) );
    assert( ( a | b ) == ( b | a ) );
    assert( ( a - b ) != ( b - a ) );
    assert( ( a ^ b ) == ( b ^ a ) );
}
