#include <utility>
#include <string>
#include <vector>
#include <cassert>

/* In this demo, we will look at overloading of standard toplevel
 * functions. We will use 3 record types to represent artistic
 * works: books of fiction, musical compositions and paintings. They
 * have some common attributes, but they are also quite different.
 * We will use function overloading to provide uniform access to the
 * common attributes. */

/* We will use a very simplified view of periodization of art, one
 * that can be more-or-less applied to all 3 types of work which we
 * are interested in. It's perhaps important to note, that the
 * historical periods associated with those styles do not exactly
 * coincide in the 3 disciplines. */

enum class style_t
{
    antique, medieval, renaissance, baroque, classical, romantic, modern
};

/* The three record types: a book has an author, a name and a
 * publisher, along with a style. A composition additionally has a
 * key (e.g. ‘c minor’) and a list of parts. On the other hand, a
 * painting does not have a publisher, but we can associate a
 * technique with it (say, ‘oil on canvas’). For simplicity, we
 * store everything as free-form strings .*/

struct book
{
    std::string author, name, publisher;
    style_t style;
};

struct composition
{
    std::string author, name, key, publisher;
    std::vector< std::string > parts;
    style_t style;
};

struct painting
{
    std::string author, name, technique;
    style_t style;
};

/* Now the functions: the first will be the simplest, essentially
 * just forwarding to attribute access. In practice, a function like
 * this is not especially useful, but it is simple. */

std::string author( book b ) { return b.author; }
std::string author( composition c ) { return c.author; }
std::string author( painting p ) { return p.author; }

/* A slightly more interesting function will be ‹description›, which
 * takes some of the attributes and combines them into a single
 * human-readable string describing the work. */

std::string description( book b )
{
    return b.name + " by " + b.author;
}

std::string description( composition c )
{
    return c.name + " in " + c.key + " by " + c.author;
}

std::string description( painting p )
{
    return p.name + " by " + p.author + " (" + p.technique + ")";
}

/* Another attribute that is shared by books and composition is the
 * name of the publisher. But there is no equivalent concept for
 * paintings. What now? There are a few options: we could leave the
 * overload undefined, which is clearly correct, but not super
 * helpful. Or we can implement an overload which returns some
 * placeholder value. Let's do that here. */

std::string publisher( book b ) { return b.publisher; }
std::string publisher( composition c ) { return c.publisher; }
std::string publisher( painting ) { return "n/a"; }

/* And finally, for the thorny issue of periods. We sort-of managed
 * to come up with a list of periods which we can sort-of apply to
 * everything, but the years covered differ in each discipline. So
 * the overloads will take care of this. */

std::pair< int, int > period( book b )
{
    switch ( b.style )
    {
        case style_t::antique: return { -1200, 455 };
        case style_t::medieval: return { 455, 1485 };
        case style_t::renaissance: return { 1485, 1660 };
        case style_t::baroque: return { 1600, 1680 };
        case style_t::classical: return { 1660, 1790 };
        case style_t::romantic: return { 1770, 1850 };
        case style_t::modern: return { 1850, 2021 };
        default: assert( false );
    }
}

std::pair< int, int > period( composition c )
{
    switch ( c.style )
    {
        case style_t::antique: return { -1300, 500 };
        case style_t::medieval: return { 500, 1400 };
        case style_t::renaissance: return { 1400, 1600 };
        case style_t::baroque: return { 1580, 1750 };
        case style_t::classical: return { 1750, 1820 };
        case style_t::romantic: return { 1800, 1910 };
        case style_t::modern: return { 1890, 2021 };
        default: assert( false );
    }
}

std::pair< int, int > period( painting p )
{
    switch ( p.style )
    {
        case style_t::antique: return { -3000, 500 };
        case style_t::medieval: return { 500, 1400 };
        case style_t::renaissance: return { 1300, 1600 };
        case style_t::baroque: return { 1600, 1730 };
        case style_t::classical: return { 1780, 1850 };
        case style_t::romantic: return { 1800, 1860 };
        case style_t::modern: return { 1860, 2021 };
        default: assert( false );
    }
}

/* Finally, we will check that we can indeed call the functions
 * uniformly on different types input types. */

int main() /* demo */
{
    book antigone{ "Sophocles", "Antigone", "n/a", style_t::antique },
         miserables{ "Victor Hugo", "Les Misérables",
                     "A. Lacroix, Verboeckhoven & Cie.",
                     style_t::romantic };

    composition
        bach_mass{ "J. S. Bach", "Mass", "b minor",
                   "Bach Gesellshaft",
                   { "soprano 1", "soprano 2", "alto", "tenor",
                     "bass", "flute 1", "flute 2", "oboe/d'amore 1",
                     "oboe/d'amore 2", "oboe 3", "bassoon 1",
                     "bassoon 2", "horn", "trumpet 1", "trumpet 2",
                     "trumpet 3", "timpani", "violin 1", "violin 2",
                     "viola", "basso continuo" },
                    style_t::baroque },

        fantasia{ "Bohuslav Martinů", "Fantasia H.301", "n/a",
                  "Max Eschig",
                  { "theremin", "oboe",
                    "violin 1", "violin 2", "viola", "violoncello",
                    "piano" },
                  style_t::modern };

    painting babel{ "Pieter Bruegel the Elder",
                    "The Tower of Babel",
                    "oil on wood", style_t::renaissance },
            boon{ "James Brooks", "Boon", "oil on canvas",
                  style_t::modern };

    /* Getting a description: */

    assert( description( bach_mass ) ==
            "Mass in b minor by J. S. Bach" );
    assert( description( babel ) ==
            "The Tower of Babel by Pieter Bruegel the Elder "
            "(oil on wood)" );
    assert( description( antigone ) == "Antigone by Sophocles" );

    /* And periods: */

    assert( period( bach_mass ) == std::pair( 1580, 1750 ) );
    assert( period( fantasia ) == std::pair( 1890, 2021 ) );
    assert( period( boon ) == std::pair( 1860, 2021 ) );
}
