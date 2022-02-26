#include <cassert>
#include <string>
#include <memory>
#include <set>

/* We will do a simpler version of regular expressions that can only
 * capture finite languages, but somewhat more compactly than just
 * listing all the words that belong to the language. There will be
 * two operations: concatenation and alternative. */

/* In this and the next demo, we will make use of late dispatch,
 * which will be properly explained in the next chapter.  All you
 * need to know for now is, that, given:
 *
 *  • a class ‹base› and its derived class ‹derived›,
 *  • a pointer, ‹base *ptr›, that in fact points to an instance
 *    of class ‹derived›, and
 *  • a method ‹late› which is marked ‹virtual› in ‹base›, and
 *    ‹override› in ‹derived›,
 *
 * a call ‹ptr->late()› will execute the implementation of the
 * method from ‹derived› (and not from ‹base›, as would be the case
 * with a non-‹virtual› method). */

/* Our goal will be to implement class ‹finexp›, with the following
 * interface:
 *
 *  • an instance of ‹finexp› can be constructed from a string; the
 *    resulting ‹finexp› will match that exact string and nothing
 *    else
 *  • two instances of ‹finexp› can be combined using ‹*›: the
 *    resulting ‹finexp› matches if the input string can be split in
 *    such a way that the first part matches the left ‹finexp› and
 *    the second part matches the right ‹finexp›
 *  • two instances of ‹finexp› can be comined using ‹+›: the result
 *    matches a string if either of the operands does
 *
 * Hint: it might be a worthwhile exercise to compare the below
 * implementation with one based on ‹std::shared_ptr›. */

struct node;
using node_ptr = std::unique_ptr< node >;

/* TBD explain things! */

struct node
{
    std::string x;
    node_ptr l, r;

    virtual std::set< int > match( const std::string &s ) const
    {
        assert( !l && !r );
        if ( s.substr( 0, x.size() ) == x )
            return { int( x.size() ) };
        else
            return {};
    }

    node_ptr copy_into( node_ptr &&n ) const
    {
        n->l = l ? l->clone() : nullptr;
        n->r = r ? r->clone() : nullptr;
        return std::move( n );
    }

    virtual node_ptr clone() const
    {
        return copy_into( std::make_unique< node >( x ) );
    }

    node( std::string x ) : x( x ) {}
    node( const node_ptr &l_, const node_ptr &r_ )
        : l( l_->clone() ), r( r_->clone() )
    {}
    virtual ~node() = default;
};

struct alt : node
{
    using node::node;

    node_ptr clone() const override
    {
        return copy_into( std::make_unique< alt >( x ) );
    }

    std::set< int > match( const std::string &s ) const override
    {
        std::set< int > lout = l->match( s ), rout = r->match( s );
        rout.insert( lout.begin(), lout.end() );
        return rout;
    }
};

struct seq : node
{
    using node::node;

    node_ptr clone() const override
    {
        return copy_into( std::make_unique< seq >( x ) );
    }

    std::set< int > match( const std::string &s ) const override
    {
        std::set< int > out;

        for ( int i : l->match( s ) )
            for ( int j : r->match( s.substr( i ) ) )
                out.insert( i + j );

        return out;
    }
};

class finexp
{
    node_ptr n;
public:
    finexp( std::string s ) : n( new node( s ) ) {}
    finexp( node_ptr &&p ) : n( std::move( p ) ) {}
    finexp( const finexp &o ) : n( o.n->clone() ) {}

    finexp operator+( finexp b ) const
    {
        return { std::make_unique< alt >( n, b.n ) };
    }

    finexp operator*( finexp b ) const
    {
        return { std::make_unique< seq >( n, b.n ) };
    }

    friend bool match( const finexp &f, const std::string &s )
    {
        return f.n->match( s ).count( s.size() );
    }
};

int main() /* demo */
{
    finexp a( "a" ), b( "b" ), ab( "ab" ), ba( "ba" ),
           abba( "abba" );

    assert(  match( a, "a" ) );
    assert(  match( b, "b" ) );
    assert( !match( a, "b" ) );
    assert( !match( b, "a" ) );

    assert(  match( abba, "abba" ) );
    assert( !match( abba, "a" ) );
    assert( !match( abba, "abb" ) );
    assert( !match( a, "ab" ) );

    assert(  match( a + b, "a" ) );
    assert(  match( a + b, "b" ) );
    assert( !match( a + b, "ab" ) );
    assert( !match( a + b, "c" ) );

    assert(  match( a + abba, "a" ) );
    assert( !match( a + abba, "b" ) );
    assert(  match( a + abba, "abba" ) );

    assert(  match( ( ab + a ) * a, "aba" ) );
    assert(  match( ( a + ab ) * a, "aba" ) );
    assert( !match( ( ba + ab ) * a, "ba" ) );

    assert(  match( a * ( ba + ab ), "aba" ) );
    assert( !match( a * ( b + a ), "aba" ) );
}
