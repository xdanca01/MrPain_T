#include <cassert>
#include <string>
#include <vector>

/* A regular grammar has rules of the form ⟦A → xB⟧ or ⟦A → x⟧ where
 * ⟦A⟧ and ⟦B⟧ are non-terminals and ⟦x⟧ is a terminal. */

/* Implement a class ‹grammar›, which is default-constructible and
 * has 2 methods:
 *
 *  • ‹add_rule›, which takes 2 or 3 arguments: a single ‹char› for
 *    the left-hand non-terminal (a capital alphabetic letter), a
 *    terminal and optionally another non-terminal,
 *  • ‹generate›, a ‹const› method which takes 2 arguments: the
 *    starting non-terminal and an integer which gives the maximum
 *    length of a word, and returns an ‹std::vector› of ‹std::string›
 *    with all the «words» the grammar can generate (within the given
 *    size bound), sorted lexicographically. */

class grammar;

int main()
{
    auto w = []( auto... words ) /* helper function */
    {
        return std::vector< std::string >{ words... };
    };

    grammar g;
    const grammar &cg = g;

    assert( cg.generate( 'A', 10 ) == w() );
    g.add_rule( 'A', 'a' );
    assert( cg.generate( 'A', 10 ) == w( "a" ) );

    g.add_rule( 'A', 'b', 'A' );
    assert( cg.generate( 'A', 3 ) == w( "a", "ba", "bba" ) );

    g.add_rule( 'A', 'x', 'X' );
    g.add_rule( 'X', 'x' );
    assert( cg.generate( 'A', 3 ) == w( "a", "ba", "bba", "bxx", "xx" ) );
    g.add_rule( 'X', 'x', 'X' );
    assert( cg.generate( 'X', 3 ) == w( "x", "xx", "xxx" ) );

    return 0;
}
