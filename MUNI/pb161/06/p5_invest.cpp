#include <utility>
#include <cassert>

/* We will revisit (again) our familiar example of a bank account.
 * This time, we add exceptions to the story: withdrawals that would
 * exceed the overdraft limit will throw. We will also add a class
 * dual to ‹loan› from the last time: an ‹investment›, which will
 * deduct money from an account upon construction, accrue interest,
 * and upon destruction, deposit the money into the original
 * account. */

/* We will use this class as the exception type. It is okay to keep
 * it empty. */

class insufficient_funds;

/* First the ‹account› class, which has the usual methods:
 * ‹balance›, ‹deposit› and ‹withdraw›. The starting balance is 0.
 * The balance must be non-negative at all times: an attempt to
 * withdraw more money than available should throw an exception of
 * type ‹insufficient_funds›. */

class account; /* reference implementation: 13 lines */

/* And finally the class ‹investment›, which has a three-parameter
 * constructor: it takes a reference to an ‹account›, the sum to
 * invest and a yearly interest rate (in percent, as an integer).
 * Upon construction, it must withdraw the sum from the account, and
 * upon destruction, deposit the original sum plus the interest. The
 * method ‹next_year› should update the accrued interest. */

class investment; /* reference implementation: 15 lines */

int main()
{
    /* create an account and deposit some money into it */
    account a; a.deposit( 100 );
    const account &c = a;
    assert( c.balance() == 100 );

    bool caught = false; /* remember that we caught an exception */

    try { investment i( a, 200, 0 ); }
    catch ( const insufficient_funds & ) { caught = true; }

    assert( caught );

    {
        investment i( a, 50, 10 );
        i.next_year();
    }

    assert( c.balance() == 105 );

    {
        /* 0-interest investment does nothing */
        investment i( a, 105, 0 );
        i.next_year();
        i.next_year();
    }

    assert( c.balance() == 105 );

    {
        /* interest 100% doubles the investment every year */
        investment i( a, 100, 100 );
        i.next_year();
        investment j = std::move( i );
        j.next_year();
        i = std::move( j );
    }

    assert( c.balance() == 405 );
    a.withdraw( 400 );
    assert( c.balance() == 5 );

    return 0;
}
