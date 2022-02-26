#include <string>
#include <cassert>

/* Check that curly and square brackets in a given string balance
 * out correctly. */

bool balanced( const std::string & );

int main()
{
    assert(  balanced( "{}" ) );
    assert(  balanced( "{(}" ) );
    assert(  balanced( "{{}}" ) );
    assert(  balanced( "{[]}" ) );
    assert(  balanced( "{[][]}" ) );
    assert(  balanced( "{[][]}[]" ) );
    assert( !balanced( "{[}]" ) );
    assert( !balanced( "{" ) );
    assert( !balanced( "]" ) );
    assert(  balanced( "{[x]y[z]} [x]" ) );

    return 0;
}
