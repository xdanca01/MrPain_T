#include <string>
#include <cctype>
#include <cassert>

/* Besides sequences of numbers, another type of sequence frequently
 * appears in computer programs: «strings», which are made of
 * letters. In C++, the basic data type for working with strings is
 * ‹std::string›, and it is rather similar to a vector, though
 * strings provide additional methods, for operations commonly
 * performed on strings (but not so commonly on other sequences). */

/* In this demo, we will show some basic usage of ‹std::string›. The
 * following function, called ‹hamming› returns an integer (of type
 * ‹int›) and accepts 2 arguments. Notice that there are some new
 * elements in the declarations of those arguments: the ‹const›
 * qualifier, meaning that we do not intend to modify the values ‹a›
 * and ‹b›, and a «reference declarator», denoted ‹&›.
 *
 * These two often go together – in this arrangement, they declare a
 * «constant reference».  In a function argument list, this means
 * that the data will «not be copied» when the function is called,
 * but the function promises not to change the original. Since a
 * string might contain a lot of data, copying all of it might be
 * expensive: this is why we prefer to use a constant reference to
 * pass it into a function, if the function only needs to examine,
 * but not change, the content of the string. In other words, ‹a›
 * and ‹b› are not «values» in their own right; instead, they are
 * aliases (new names) for existing values, albeit such that the
 * original values «cannot be modified» through these new names. If
 * you try to, the compiler will complain.¹ */

int hamming( const std::string &a, const std::string &b )
{
    /* First, we declare a precondition: the strings must be of
     * equal size. In other words, calling ‹hamming› on two strings
     * of different length is a «programming» error: the caller is
     * responsible for ensuring that the condition holds. */

    assert( a.size() == b.size() );

    /* We declare a local variable to hold the computed distance, of
     * type ‹int› (the ‘default’ integral type in C++). */

    int distance = 0;

    /* Again, a standard C-style ‹for› loop. Notice that strings can
     * be indexed, just like arrays and vectors. Also notice that
     * the loop variable is now of type ‹size_t› – an unsigned
     * integer type. This is because the ‹size› methods of standard
     * containers in C++ return unsigned numbers,² and comparing
     * signed and unsigned integers can cause problems. */

    for ( size_t i = 0; i < a.size(); ++ i )
        if ( std::toupper( a[ i ] ) != std::toupper( b[ i ] ) )
            distance ++;

    /* And a return statement. */

    return distance;
}

/* That is all. If you have never heard of Hamming distance before,
 * it might be a good idea to look it up. */

int main() /* demo */
{
    assert( hamming("Python", "python") == 0 );
    assert( hamming("AbCd", "aBcD") == 0 );
    assert( hamming("string", "string") == 0 );
    assert( hamming("aabcd", "abbcd") == 1 );
    assert( hamming("abcd", "ghef") == 4 );
    assert( hamming("Abcd", "bbcd") == 1 );
    assert( hamming("gHefgh", "ghefkl") == 2 );
}

/* ¹ Of course, this being C++, there is a way around that. It is
 *   only needed very rarely, and only in ‘plumbing’ – low-level
 *   code which implements, for instance, new data types.
 *
 * ² Arguably, this is a design mistake in C++. There are proposals
 *   to fix it, but a change in this regard is going to take a long
 *   time, if it ever happens. In the meantime, it makes sense to
 *   use unsigned types for straightforward loop variables (i.e.
 *   those that count up). */
