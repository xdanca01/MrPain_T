#include <algorithm> /* std::max */
#include <cassert>
#include <climits>

/* A fixed point of a function ⟦f⟧ is an ⟦x⟧ such that ⟦f(x) = x⟧. A
 * function is monotonic if ⟦∀x, y. x ≤ y → f(x) ≤ f(y)⟧. Assume
 * that ‹f› is a monotonic function and hence, since there are only
 * finitely many ‹int› values, that it has at least one fixed point.
 * Find the «greatest» fixed point of ‹f›. */

// auto fixpoint = …

int f( int x ) { return x / 2; }
int g( int x ) { return x - x / 20; }
int h( int x ) { return std::max( x / 5, 20 ); }
int i( int x ) { return x < INT_MAX ? x + 1 : x; }

int main()
{
    assert( fixpoint( f ) == 0 );
    assert( fixpoint( g ) == 19 );
    assert( fixpoint( h ) == 20 );
    assert( fixpoint( i ) == INT_MAX );

    return 0;
}
