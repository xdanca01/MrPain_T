#include <vector>
#include <cassert>

/* Write a function object that will decide, when passed to
 * ‹foreach›, whether the iterated sequence was sorted in ascending
 * order. The result is obtained by calling ‹was_sorted› on the
 * object after the iteration ends.
 *
 * It might be useful to know that ‹std::any› can hold a value of
 * any type. Use normal assignment to store a value in an ‹any›
 * instance and ‹std::any_cast› to read the value back. */

struct check_sorted;

/* Unlike ‹std::foreach›, we take the function by reference, which
 * makes it possible to inspect its state after iteration ends. */

template< typename iter_t, typename fun_t >
void foreach( iter_t b, iter_t e, fun_t &&f )
{
    while ( b != e )
        f( *b++ );
}

int main()
{
    std::vector v_1{ 1, 2, 5, 7 };
    std::vector v_2{ 1.0, 1.3, 1.1 };

    auto check = []( const auto &seq )
    {
        check_sorted f;
        foreach( seq.begin(), seq.end(), f );
        return f.was_sorted();
    };

    assert(  check( v_1 ) );
    assert( !check( v_2 ) );

    std::vector< int > v_3( v_2.begin(), v_2.end() );
    assert(  check( v_3 ) );

    return 0;
}
