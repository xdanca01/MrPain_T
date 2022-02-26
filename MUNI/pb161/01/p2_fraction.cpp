#include <vector>
#include <cassert>

/* Write a function which evaluates a continued fraction: given a
 * vector of coefficients of the continued fraction, it computes a
 * numerator and a denominator of a traditional fraction with the
 * same value.
 *
 * A continued fraction is a representation of a rational number ⟦q⟧
 * as a sum of ⟦a₀⟧ and the reciprocal of a second number, ⟦q₀⟧,
 * which is itself written as a continued fraction: ⟦q₀ = a₀ + 1/q₁⟧
 * where ⟦q₁ = a₁ + 1/q₂⟧, ⟦q₂ = a₂ + 1/q₃⟧ and so on. The sequence
 * ⟦a₀, a₁, a₂, …⟧ are the «coefficients» of the continued fraction.
 * For a rational number, one of the ⟦qₙ⟧ eventually becomes 0 and
 * the sequence ends there.
 *
 * For more details, see e.g. wikipedia. */

/* Define a traditional fraction as a ‹struct› with two integer
 * attributes, ‹p› and ‹q› (the numerator and the denominator,
 * respectively). */

struct fraction;

fraction eval_continued( const std::vector< int > &coeff );

int main()
{
    std::vector< int > c_1{ 4, 2,  6, 7 },
                       c_2{ 3, 4, 12, 4 },
                       c_3{ 0, 2,  4 };

    fraction f_1{ 415,  93 },
             f_2{ 649, 200 },
             f_3{   4,   9 };

    fraction r_1 = eval_continued( c_1 ),
             r_2 = eval_continued( c_2 ),
             r_3 = eval_continued( c_3 );

    assert( r_1.p == f_1.p && r_1.q == f_1.q );
    assert( r_2.p == f_2.p && r_2.q == f_2.q );
    assert( r_3.p == f_3.p && r_3.q == f_3.q );

    return 0;
}
