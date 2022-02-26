#include <cassert>

/* Ternary (or 3-valued) logic uses 3 different truth values: true,
 * false and unknown (maybe). */

/* Define a suitable type ‹tristate› and 3 constants ‹yes›, ‹no› and
 * ‹maybe› (to avoid conflicts with built-in boolean constants),
 * along with the standard logical operators and equality. */

int main()
{
    assert( ( yes && no  ) == no );
    assert( ( no  && yes ) == no );
    assert( ( no  && no  ) == no );
    assert( ( yes && yes ) == yes );

    assert( ( ( yes && no ) && maybe ) == no );
    assert( ( ( yes && no ) || maybe ) == maybe );
    assert( ( ( yes && no ) || maybe ) == maybe );

    assert( ( yes   && yes   && maybe ) == maybe );
    assert( ( yes   && maybe && yes   ) == maybe );
    assert( ( maybe && yes   && yes   ) == maybe );

    assert( ( no || maybe || yes   ) == yes );
    assert( ( no || yes   || maybe ) == yes );
    assert( ( no || maybe || no    ) == maybe );

    return 0;
}
