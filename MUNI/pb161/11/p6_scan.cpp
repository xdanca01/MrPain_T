#include <memory>
#include <set>
#include <vector>
#include <cassert>

/* Implement a class template ‹scan›, which computes a «generalized
 * prefix sum». This is essentially a fold, but instead of simply
 * returning the final value, it also returns all the intermediate
 * results. It should be possible to iterate instances of ‹scan›
 * using a range ‹for› loop.
 *
 * Example: a scan of a sequence with elements 1, 2, 3 and 4, using
 * ‹std::plus›, and initial value 0 should yield the sequence 1, 3,
 * 6, 10.
 *
 * The constructor, which should enable class template argument
 * deduction, takes 3 arguments:
 *
 *  • a ‹const› reference to a container with ‹value_type = T›,
 *  • a lambda with the signature ‹S( T, S )›, and
 *  • an initial value of type ‹S›, by value.
 *
 * NB. Do not assume that values of either type ‹S› or ‹T› can be
 * copied. Values of type ‹S› can be default-constructed though. */

int main()
{
    std::vector v{ 1, 2, 3, 4 };
    const std::set s{ 1, 2 };

    std::vector< std::unique_ptr< int > > pv;
    pv.push_back( std::make_unique< int >( 1 ) );
    pv.push_back( std::make_unique< int >( 2 ) );

    int i = 0;

    for ( int x : scan( v, std::plus<>(), 0 ) )
        switch ( i++ )
        {
            case 0: assert( x == 1 ); break;
            case 1: assert( x == 3 ); break;
            case 2: assert( x == 6 ); break;
            case 3: assert( x == 10 ); break;
            default: assert( false );
        }

    auto ptr_sum = []( int x, const std::unique_ptr< int > &y )
    {
        return std::make_unique< int >( x + *y );
    };

    i = 0;
    scan ptr_scan( v, ptr_sum, std::make_unique< int >( 0 ) );
    const auto &const_scan = ptr_scan;

    for ( const auto &x : const_scan )
        switch ( i++ )
        {
            case 0: assert( *x == 1 ); break;
            case 1: assert( *x == 3 ); break;
            case 2: assert( *x == 6 ); break;
            case 3: assert( *x == 10 ); break;
            default: assert( false );
        }

    auto deref_sum = []( const std::unique_ptr< int > &x, int y )
    {
        return *x + y;
    };

    i = 0;

    for ( int x : scan( pv, deref_sum, 0 ) )
        switch ( i++ )
        {
            case 0: assert( x == 1 ); break;
            case 1: assert( x == 3 ); break;
            default: assert( false );
        }

    scan ss( s, std::plus<>(), 0 );

    return 0;
}
