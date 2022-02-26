#include <cassert>
#include <vector>
#include <set>
#include <string_view>
#include <variant>

/* Write a function which returns a vector of variants, such that
 * the i-th position is taken from input ‹a› iff ‹which[ i ]› is
 * true and from input ‹b› otherwise. Both ‹a› and ‹b› must have at
 * least ‹which.size()› elements. Elements beyond this size are
 * ignored. Both ‹a› and ‹b› are sequences with a ‹value_type›
 * nested typedef. */

// … select( … a, … b, const std::vector< bool > &which );

int main()
{
    std::string_view he( "he" ), llo( "llo" );
    std::vector hello{ 'h', 'e', 'l', 'l', 'o' };
    std::vector i12{ 1, 2 }, i345{ 3, 4, 5 };
    std::set s12{ 1, 2 };

    using vec_ci = std::vector< std::variant< char, int > >;
    using vec_ic = std::vector< std::variant< int, char > >;

    vec_ci ci_h2{ 'h', 2 },
           ci_l4o{ 'l', 4, 'o' },
           ci_he{ 'h', 'e' };

    vec_ic ic_h2{ 'h', 2 },
           ic_l4o{ 'l', 4, 'o' };

    using bitmap = std::vector< bool >;
    bitmap b01{ 0, 1 },
           b101{ 1, 0, 1 },
           b10{ 1, 0 },
           b010{ 0, 1, 0 },
           b11{ 1, 1 },
           b00{ 0, 0 };

    assert( select( he, i12, b10 ) == ci_h2 );
    assert( select( he, s12, b10 ) == ci_h2 );
    assert( select( s12, he, b01 ) == ic_h2 );
    assert( select( he, i12, b11 ) == ci_he );
    assert( select( llo, i345, b101 ) == ci_l4o );
    assert( select( i345, llo, b010 ) == ic_l4o );

    return 0;

    // clang-tidy: -modernize-use-bool-literals
}
