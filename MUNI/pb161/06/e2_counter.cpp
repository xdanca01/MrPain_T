#include <cassert>
#include <vector>

static int counter = 0;

/* Add constructors and a destructor to ‹counted› in such a way
 * that ‹counter› above always corresponds to the number of
 * instances of ‹counted› that exist at any given time. */

struct counted;

int main()
{
    assert( counter == 0 );
    counted c_1;
    assert( counter == 1 );

    {
        counted c_2;
        assert( counter == 2 );

        counted c_3 = c_2;
        assert( counter == 3 );

        std::vector< counted > vec;
        vec.emplace_back();
        vec.emplace_back();

        assert( counter == 5 );
    }

    assert( counter == 1 );

    return 0;

    // clang-tidy: -performance-unnecessary-copy-initialization
}
