#include <vector>
#include <functional>
#include <cassert>

/* We already know that we can write class templates and function
 * templates. It is only logical that we can also create method
 * templates in C++ classes and in class templates. */

/* This example will be somewhat synthetic: we will have a class
 * which does not permit direct access to its elements, but allows
 * them to be folded using a function object. */

template< typename T >
class foldable
{
    std::vector< T > data;
public:

    /* A method to add elements to the internal container. */

    void push( const T &t ) { data.push_back( t ); }

    /* And the method template to accumulate the content using a
     * function object. */

    template< typename fun_t, typename init_t >
    init_t fold( const fun_t &fun, init_t init )
    {
        for ( const auto &e : data )
            init = fun( init, e );
        return init;
    }
};

int main() /* demo */
{
    foldable< int > f;
    f.push( 7 );
    f.push( 3 );
    assert( f.fold( std::plus<>(), 0 ) == 10 );
    f.push( 10 );
    assert( f.fold( std::multiplies<>(), 1 ) == 210 );
}
