#include <tuple>
#include <set>
#include <string>
#include <cassert>

/* This exercise demonstrates use of ‹std::tuple› and structural
 * bindings.  Since we cannot write generic code yet (and even if we
 * did, writing the below operators in full generality would be
 * rather tricky), we will only work with a fixed set of types
 * (relations). */

/* First a bunch of type aliases: ‹item› and its variants each
 * represent a single row, while ‹rel› and its variants represent an
 * entire relation. */

using item     = std::tuple< std::string, int, double >;
using item_dbl = std::tuple< std::string, double >;
using item_int = std::tuple< std::string, int >;

using rel     = std::set< item >;
using rel_dbl = std::set< item_dbl >;
using rel_int = std::set< item_int >;

/* Projections: keep a subset of columns, in this case the string
 * and either of the numeric columns. */

rel_int project_int( const rel & );
rel_dbl project_dbl( const rel & );

/* Selection: keep a subset of rows -- those that match on the given
 * column. Throw away all the rest. */

rel select_str( const rel &, const std::string &n );
rel select_int( const rel &, int n );

int main()
{
    rel r{ { "hydrogen",  1, 0.78 },
           { "hydrogen",  2, 1.50 },
           { "hydrogen",  3, 3.09 },
           { "helium",    3, 3.09 },
           { "iron",     56, 9.15 },
           { "iron",     58, 9.14 },
           { "nickel",   60, 9.15 },
           { "nickel",   62, 9.15 } };

    rel nickel = select_str( r, "nickel" ),
        iron   = select_str( r, "iron" ),
        helium = select_str( r, "helium" );

    rel helium_expect{ { "helium", 3, 3.09 } },
        iron_expect  { { "iron", 56, 9.15 },
                       { "iron", 58, 9.14 } },
        nickel_expect{ { "nickel", 60, 9.15 },
                       { "nickel", 62, 9.15 } };

    assert( helium == helium_expect );
    assert( iron   == iron_expect );
    assert( nickel == nickel_expect );

    rel_int helium_n = project_int( helium ),
            nickel_n = project_int( nickel );

    rel_int helium_n_expect{ { "helium", 3 } },
            nickel_n_expect{ { "nickel", 60 },
                             { "nickel", 62 } };

    assert( helium_n == helium_n_expect );
    assert( nickel_n == nickel_n_expect );

    rel_dbl nickel_e = project_dbl( nickel ),
            iron_e   = project_dbl( iron );

    rel_dbl nickel_e_expect{ { "nickel", 9.15 } },
            iron_e_expect  { { "iron",   9.15 },
                             { "iron",   9.14 } };

    assert( nickel_e == nickel_e_expect );
    assert( iron_e   == iron_e_expect );

    return 0;
}
