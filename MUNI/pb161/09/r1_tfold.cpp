#include <cassert>
#include <memory>
#include <functional>

/* Fold a proper binary tree using an associative and commutative
 * binary function (proper meaning that each node either has both
 * children, or none). */

template< typename value_t >
struct tree
{
    std::unique_ptr< tree > left, right;
    value_t value;

    static auto make_tree( const tree &t )
    {
        return std::make_unique< tree >( t );
    }

    tree( const tree &t )
        : left(  t.left  ? make_tree( *t.left )  : nullptr ),
          right( t.right ? make_tree( *t.right ) : nullptr ),
          value( t.value )
    {}

    tree( value_t value, const tree &l, const tree &r )
        : left( make_tree( l ) ),
          right( make_tree( r ) ),
          value( std::move( value ) )
    {}

    tree( value_t value ) : value( std::move( value ) ) {}
};

/* Given a binary function ‹f› and a tree instance ‹t›, compute a
 * single value that is the result of folding the entire tree. Since
 * ‹f› is both associative and commutative, it does not matter in
 * which order you combine the individual values. */

// … tfold( … f, … t )

int main()
{
    using t_int = tree< int >;
    using t_dbl = tree< double >;

    t_int t_1( 7, t_int( 2 ), t_int( 3, t_int( 4 ), t_int( 5 ) ) );
    t_dbl t_2( 7, t_dbl( 2 ), t_dbl( 3, t_dbl( 4 ), t_dbl( 5 ) ) );

    assert( tfold( std::plus<>(), t_1 ) == 21 );
    assert( tfold( std::multiplies<>(), t_1 ) == 840 );
    assert( tfold( std::plus<>(), t_2 ) == 21 );

    return 0;
}
