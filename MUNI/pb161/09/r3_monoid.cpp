#include <cassert>
#include <string>

/* Monoids are algebraic structures with a single operation, usually
 * written as multiplication. A «free monoid» ⟦M⟧ on a set ⟦A⟧ is
 * defined as the set of all strings of elements from ⟦A⟧ with
 * «concatenation» as the operation. A monoid «homomorphism» is a
 * map ⟦f⟧ from ⟦M⟧ to ⟦M'⟧ with the property ⟦f(a⋅b) = f(a)⋅f(b)⟧.
 * All monoids arise as «homomorphic images» of a free monoid on
 * some set. */

/* Define a «class template» ‹monoid›, which takes a single type
 * argument, ‹hom_t›, with the following behaviour:
 *
 *  • the constructor will then accept ‹hom›, a value (typically a
 *    lambda) of type ‹hom_t›, as an argument and store it in an
 *    attribute (by value),
 *  • method ‹elem›, which takes an ‹std::string› and returns a
 *    value of a suitable type with a multiplication and an
 *    equality operator.
 *
 * The class should work with a fixed underlying set: the minuscule
 * Latin letters (i.e. 'a' through 'z') and use the mechanics of a
 * free monoid to implement multiplication. The provided ‹hom› will
 * take an ‹std::string› as an argument, and return a value of some
 * arbitrary type. Assume applying ‹hom› to a string yields values
 * which can be compared, but not multiplied (at least not in a way
 * compatible with the provided homomorphism). */

template< typename hom_t >
class monoid_element; /* ref: 11 lines */

template< typename hom_t >
class monoid; /* ref: 8 lines */

int main()
{
    auto len = []( const std::string &s ) { return s.size(); };

    auto highest = []( const std::string &s )
    {
        char e = 'a';
        for ( auto c : s )
            e = std::max( e, c );
        return e;
    };

    auto first_a = []( const std::string &s )
    {
        if ( auto i = s.find( 'a' ); i != s.npos )
            return i;
        else
            return 0ul;
    };

    /* The non-negative integers with addition. */

    monoid nat( len );

    auto n_0 = nat.elem( "" ),   n_1 = nat.elem( "a" ),
         n_2 = nat.elem( "ab" ), n_4 = nat.elem( "xyza" );

    assert( n_0 * n_0 == n_0 );
    assert( n_1 * n_0 == n_1 );
    assert( n_0 * n_1 == n_1 );
    assert( n_1 * n_1 == n_2 );
    assert( n_2 * n_2 == n_4 );

    /* A finite, linearly-ordered semilattice on 26 elements. */

    monoid s( highest );

    /* s_0 is the identity (bottom) */

    auto s_0 = s.elem( "a" ),    s_1 = s.elem( "b" ),
         s_2 = s.elem( "cabc" ), s_4 = s.elem( "da" );

    assert( s.elem( "abc" ) == s.elem( "c" ) );

    assert( s_0 * s_0 == s_0 );
    assert( s_0 * s_1 == s_1 );
    assert( s_1 * s_0 == s_1 );
    assert( s_1 * s_1 == s_1 );
    assert( s_0 * s_2 == s_2 );
    assert( s_4 * s_2 == s_4 );

    /* The find-first monoid on natural numbers (a monoid extension
     * of the left zero semigroup on N). */

    monoid nat_ff( first_a );

    auto f_0 = nat_ff.elem( "" ),   f_1 = nat_ff.elem( "a" ),
         f_2 = nat_ff.elem( "xa" ), f_3 = nat_ff.elem( "xya" );

    /* f_0 is the identity element */

    assert( f_0 * f_1 == f_1 );
    assert( f_1 * f_0 == f_1 );
    assert( f_0 * f_2 == f_2 );
    assert( f_2 * f_0 == f_2 );

    assert( f_1 * f_1 == f_1 );
    assert( f_2 * f_1 == f_2 );
    assert( f_1 * f_2 == f_1 );
    assert( f_2 * f_2 == f_2 );
    assert( f_3 * f_2 == f_3 );
    assert( f_3 * f_1 == f_3 );
    assert( f_2 * f_3 == f_2 );
    assert( f_1 * f_3 == f_1 );

    return 0;
}
