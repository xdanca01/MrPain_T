#include <string>
#include <tuple>
#include <cassert>

/* The final example will deal with the function call operator, also
 * known as ‹operator()›. This will allow us to construct objects
 * which can be «called», just like lambdas. However, there is one
 * thing that lambdas can't do very well in C++, and that is provide
 * multiple «overloads». Likewise, standard top-level functions
 * cannot be easily passed as arguments to other functions, since
 * overload sets are not first-class in C++: instead, we have to
 * wrap up the overload set in a callable object. We will see that
 * in a minute. */

/* We will re-use the same construction that we have seen in
 * ‹apply.cpp›, but we will allow different value types to appear in
 * the tree, instead of just integers. We will again define ‹null›
 * as the empty tree: */

struct null {};

/* Even though the ‹struct› above is empty, we need to define
 * equality if we want to use it. Since it will be rather useful in
 * writing tests later, we will define the (trivial) equality
 * operator here: */

bool operator==( null, null ) { return true; }

/* Like other functions defined on recursive data types, we first
 * need to define the base case for ‹map›, i.e. the case when the
 * subtree is empty: */

template< typename fun_t >
auto map( null, const fun_t & )
{
    return null();
}

/* And instead of ‹sum›, we will have a generic mapping operator: */

template< typename val_t, typename left_t, typename right_t,
          typename fun_t >
auto map( const std::tuple< val_t, left_t, right_t > &tuple,
          const fun_t &fun )
{
    const auto &[ val, left, right ] = tuple;
    return std::tuple{ fun( val ),
                       map( left, fun ), map( right, fun ) };
}

/* The call operator uses syntax very similar to the indexing
 * operator which we have seen before, just with parentheses instead
 * of square brackets. Since ‹()› is the name of the operator, the
 * arguments need to come in «another» pair of parens. Please keep
 * that in mind! */

struct to_string
{
    std::string operator()( int i ) const
    {
        return std::to_string( i );
    }

    std::string operator()( const std::string &s ) const
    {
        return s;
    }
};

/* A small thing that we have not seen yet. Normally, if we want to
 * construct an object, we need to call its constructor, e.g.
 * ‹std::string( "hello" )›. Sometimes, this is quite tedious, like
 * in this example. C++ offers a feature called «user-defined
 * literals», where we are allowed to overload certain operators
 * which then make it possible to create object instances using
 * «literal syntax». We will not get into the details of creating
 * such user-defined literals, but we will use the one that the
 * standard library provides for constructing ‹std::string›
 * instances. To use them, we need to use the following declaration
 * first, to get the literal operators into scope: */

using namespace std::literals;

int main() /* demo */
{
    /* After the ‹using namespace› above, we can say "hello"s to
     * mean ‹std::string( "hello" )›. Saves us a bit of typing. */

    std::tuple a{ "hello"s, null(), null() };
    std::tuple b{ 7, null(), null() };
    std::tuple c{ "x"s, a, b };

    std::tuple b_str{ "7"s, null(), null() };
    std::tuple c_str{ "x"s, a, b_str };

    assert( map( a, to_string() ) == a );
    assert( map( b, to_string() ) == b_str );
    assert( map( c, to_string() ) == c_str );
}
