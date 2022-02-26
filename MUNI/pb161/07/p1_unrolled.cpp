#include <cassert>

/* Another exercise, another data structure. This time we will look
 * at so-called «unrolled linked lists». We will need the data
 * structure itself, with ‹begin›, ‹end›, ‹empty› and ‹push_back›
 * methods. As usual, we will store integers. The difference between
 * a ‘normal’ singly-linked list and an unrolled list is that in the
 * latter, each node stores more than one item. In this case, we
 * will use 4 items per node. Of course, the last node might only be
 * filled partially. The iterator that ‹begin› and ‹end› return
 * should at least implement dereference, pre-increment and
 * inequality, as usual. We will not provide an interface for
 * erasing elements, because that is somewhat tricky. */

struct unrolled_node;     /* ref: 6 lines */
struct unrolled_iterator; /* ref: 22 lines */
class unrolled;           /* ref: 36 lines */

int main()
{
    /* Basic properties of an empty list. */
    unrolled u;
    assert( u.empty() );
    assert( !( u.begin() != u.end() ) );

    /* Basic properties of a non-empty list. */

    u.push_back( 3 );
    assert( !u.empty() );
    auto i = u.begin();

    assert( i != u.end() );
    ++ i;
    assert( !( i != u.end() ) );

    /* Helper function to check the content of the list. */

    auto check = [&]( int total )
    {
        int count = 0;
        for ( int x : u )
            assert( x == count++ + 3 );
        assert( count == total );
    };

    /* Add things to the list and check they are there. */

    check( 1 );

    for ( int i = 0; i < 10; ++i )
    {
        u.push_back( 4 + i );
        check( 2 + i );
    }

    return 0;
}
