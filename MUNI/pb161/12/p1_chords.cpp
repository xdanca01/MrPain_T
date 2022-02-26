#include <cassert>
#include <string>

/* We will write a simple program to compute and format chords (as
 * in music). Partition the code as you see fit. */

/* The entire western musical scale has 12 notes in it, one semitone
 * (100 cents) apart. Chords are built up from minor (300 cents) and
 * major (400 cents) thirds. We will only deal with chords in the
 * root position, i.e. where the root note is in the bass and we'll
 * use the German names:
 *
 *  • c, d, e, f, g, a and h are the ‘base’ notes
 *  • cis, dis, eis = f, fis, gis, ais, his = c are sharps,
 *  • ces = h, des, es, fes = e, ges, as and b are flats.
 *
 * Base notes are 200 cents apart, except the e/f and h/c pairs,
 * which are 100 cents apart. A flat subtracts, and a sharp adds,
 * 100 cents to the base note. The simplified rules for using note
 * names in chords are as follows:
 *
 *  • key is E, G, A, H, D or any note with a sharp → use sharps,
 *  • key is F or a note with a flat → use flats,
 *  • flats and sharps are not mixed in basic chords,
 *  • ignore double flats and double sharps
 *  • instead eis, his, ces and fes, use f, c, h and e.
 *
 * A (pure) fifth is 700 cents and a minor 7th is 1000 cents.
 * Intervals (in cents) are composed using addition, mod 1200. By
 * convention, ‘c’ is 0. For instance, if the root is ‘g’, that is
 * 700 cents, adding a pure fifth yields 1400 mod 1200 = 200 = ‘d’.
 * Notes ‘g’ and ‘d’ are a fifth apart. */

/* The major fifth chord starts at the key note (tonic) + a major
 * third + minor third, e.g. ‘c’ → ‘c e g’ or ‘e’ → ‘e gis h’. */

std::string major_5( std::string key );

/* The root of a minor fifth chord is a sixth above the key note and
 * adds a minor third + a major third, e.g. ‘c‘ → ‘a c e’ or ‘e’ →
 * ‘cis e gis’. Alternatively, you could think of it as a minor
 * third «below» the key note, the key note itself, and a major
 * third above. */

std::string minor_5( std::string key );

/* The root of a dominant 7th chord is a fifth above the key note
 * (tonic): for key ‘c’, the root of the dominant is ‘g’. On top of
 * the root, add a major third, a minor third, and another minor
 * third. E.g. ‘f’ → ‘c e g b’ */

std::string dominant_7( std::string key );

/* ref: 42 lines in 4 helper functions, major_5, minor_5 &
 * dominant_7 are each 1 line */

int main()
{
    assert( major_5( "c" )  == "c e g" );
    assert( major_5( "a" )  == "a cis e" );
    assert( major_5( "f" )  == "f a c" );
    assert( major_5( "fis" )  == "fis ais cis" );
    assert( major_5( "e" )  == "e gis h" );

    assert( minor_5( "es" ) == "c es g" );
    assert( minor_5( "c" )  == "a c e" );
    assert( minor_5( "d" )  == "h d fis" );
    assert( minor_5( "e" )  == "cis e gis" );
    assert( minor_5( "a" )  == "fis a cis" );

    assert( dominant_7( "c" )  == "g h d f" );
    assert( dominant_7( "d" )  == "a cis e g" );
    assert( dominant_7( "b" )  == "f a c es" );
    assert( dominant_7( "as" ) == "es g b des" );
    assert( dominant_7( "h" )  == "fis ais cis e" );

    return 0;
}
