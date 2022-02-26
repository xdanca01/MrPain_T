#include <tuple>
#include <string>
#include <set>
#include <cassert>

/* We will take a second look at function templates, but this time
 * we will also add non-type template arguments to the mix. We haven't
 * used it much, but this is how ‹std::get› works. It might be
 * useful to review ‹03/p4_rel.cpp› before diving into this example. */

/* General projections are still too hard for us, so we will only do
 * a single-column projection. However, selection is easier so let's
 * look at those first. We will need 3 template arguments: one to
 * specify which column to use as the selection criterion, another
 * to specify the type of a single row and the last one to specify
 * the type of the value which we will compare with the entries. */

template< int index, typename rel_t, typename key_t >
rel_t select( const rel_t &rel, const key_t &key )
{
    /* Since the type of the relation does not change under
     * selection, it is simple enough to create an empty relation
     * and add matching rows from ‹rel› to it. */

    rel_t out;

    /* We assume that it is possible to iterate a ‹rel_t›, and that
     * it is possible to ‹insert› things into a ‹rel_t›. Since
     * templates are «duck-typed», this will be checked when the
     * template is instantiated. */

    for ( const auto &row : rel )

        /* We now need to decide whether the row matches the
         * criterion: the ‹index›-th column should be equal to ‹key›
         * for that, so let's check that. */

        if ( std::get< index >( row ) == key )

            /* Just insert the row. */
            out.insert( row );

    /* And return the result. */

    return out;
}

/* Now for a single-column projection. Again, we will pass in ‹index›
 * as a template parameter. However, we will run into some problems
 * with the return type. Fortunately, in the signature, we can just
 * use ‹auto› as the return type and worry about it later. */

template< int index, typename rel_t >
auto project( const rel_t &rel )
{
    /* Actually, we can't put that off for very long. We need to
     * declare the variable to hold the resulting relation. First of
     * all, we need to find out the type of a single row. For that,
     * we can use standardized nested types that all ‹std›
     * containers provide, which we have learned about in the
     * previous unit. The row is the ‹value_type› of the relation,
     * like this. Remember that the ‹typename› specifier is
     * compulsory whenever we want to refer to a type nested within
     * a template parameter, or within something that depends on a
     * template parameter. */

    using row_t = typename rel_t::value_type;

    /* Now that we have the row type, we need to extract the type of
     * ‹index›-th column. For that, the standard library provides
     * the ‹tuple_element_t› helper template, like this: */

    using col_t = std::tuple_element_t< index, row_t >;

    /* Now we have the type that we need to construct the output
     * relation, which we will construct as a set of ‹col_t›. */

    std::set< col_t > out;

    /* At this point, the code for ‹project› is just a variation on
     * what we already saw in ‹select›. */

    for ( const auto &row : rel )
        out.insert( std::get< index >( row ) );

    return out;
}

int main() /* demo */
{
    using element = std::tuple< std::string, int, double >;
    using elem_rel = std::set< element >;

    /* We first define a testing data set. */

    elem_rel r{ { "hydrogen",  1, 0.78 },
                { "hydrogen",  2, 1.50 },
                { "hydrogen",  3, 3.09 },
                { "helium",    3, 3.09 },
                { "iron",     56, 9.15 },
                { "iron",     58, 9.14 },
                { "nickel",   60, 9.15 },
                { "nickel",   62, 9.15 } };

    /* Using ‹select› is straightforward: we specify, as a template
     * parameter (i.e. using angle brackets) the column index, and
     * pass in the relation and the key as standard arguments. The
     * types of the relation and the key are then deduced
     * automatically. You may find it helpful to compare the calls
     * with the definition of ‹select› above. */

    elem_rel nickel = select< 0 >( r, "nickel" ),
             iron   = select< 0 >( r, "iron" ),
             helium = select< 0 >( r, "helium" );

    elem_rel helium_expect{ { "helium", 3, 3.09 } },
             iron_expect  { { "iron", 56, 9.15 },
                            { "iron", 58, 9.14 } },
             nickel_expect{ { "nickel", 60, 9.15 },
                            { "nickel", 62, 9.15 } };

    assert( helium == helium_expect );
    assert( iron   == iron_expect );
    assert( nickel == nickel_expect );

    /* Now for projection: again, we explicitly specify the index of
     * the column to extract, as a template parameter. The type of
     * the relation is deduced and we therefore do not need to
     * mention it. */

    auto names = project< 0 >( r );
    std::set< std::string > names_expect{ "hydrogen", "helium",
                                          "iron", "nickel" };

    assert( names == names_expect );

    std::set< std::pair< int, int > > p{ { 1, 1 }, { 1, 2 },
                                         { 2, 2 }, { 2, 4 } };

    std::set< int > left{ 1, 2 }, right{ 1, 2, 4 };

    assert( project< 0 >( p ) == left );
    assert( project< 1 >( p ) == right );
    assert( project< 1 >( select< 0 >( p, 1 ) ) == left );
}
