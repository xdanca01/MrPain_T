#include <cassert>
#include <memory>

/* The class ‹element› represents a value which, for whatever
 * reason, cannot be duplicated. Our goal will be to write a
 * function which takes a vector of these, finds the smallest and
 * returns it. Do not change the definition of ‹element› in any
 * way. */

class element
{
    int value;
public:
    element( int v ) : value( v ) {}
    element( element &&v ) : value( v.value ) {}
    element &operator=( element &&v ) = default;
    bool less_than( const element &o ) const { return value < o.value; }
    bool equal( const element &o ) const { return value == o.value; }
};

using data = std::vector< element >;

/* Write function ‹least› (or a couple of function overloads) so
 * that calling ‹least( d )› where ‹d› is of type ‹data› returns the
 * least element in the input vector. */

// ??? least( ??? )

int main()
{
    auto mk = []( auto... vs )
    {
        data d;
        ( d.emplace_back( vs ), ... );
        return d;
    };

    data d_1 = mk( 1, 2, 3 ), d_2 = mk( 3, 2, 7 );
    assert( least( d_1 ).equal( 1 ) );
    assert( least( d_2 ).equal( 2 ) );
    const element &l = least( mk( 4, 3, 1, 2 ) );
    assert( l.equal( 1 ) );

    return 0;
}
