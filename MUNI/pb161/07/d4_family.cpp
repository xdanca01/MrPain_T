#include <memory>
#include <string>
#include <vector>
#include <cassert>

/* For many tasks, shared pointers (reference counting) are quite
 * adequate (see also Python). However, they do have a weak spot:
 * reference cycles. If you manage to create a loop of shared
 * pointers, the pointers on this cycle (and anything outside the
 * cycle they point to) will never be freed. That is unfortunate,
 * since it reintroduces memory leaks into the rather leak-free
 * subset of C++ that we have been using until now.
 *
 * However, if we are a little careful, C++ allows us to have cyclic
 * data structures with reference counting without introducing
 * memory leaks: the ‹std::weak_ptr› class template. */

/* We will implement a bit of genealogy -- that is, family trees.
 * This will simply consist of a graph of ‹person› instances (we
 * will not delve into too much detail). Each ‹person› will have two
 * parents, a father and a mother, and a list of children. We will
 * want to maintain an invariant: the list of children contains
 * exactly those ‹person› instances that have this ‹person› set as
 * one of their parents. Since a fixed number of pointers (parents)
 * are easier to manage than the arbitrary number of children, we
 * will treat parents as the primary information and children as
 * derived. Like before, we will split the class into a shared
 * (data) part and into thin «interface» part. */

class person_data
{
    std::shared_ptr< person_data > mother, father;
    std::vector< std::weak_ptr< person_data > > children;
    std::string name;
    friend class person;
};

/* The interface: the data is stored behind a shared pointer, but
 * like in earlier examples, we pretend the ‹person› instances are
 * values with sharing semantics. The family graph is, on the
 * outside, still quite immutable (we can only add and remove
 * nodes), so the abstraction is still reasonably solid.  */

class person
{
    using data_ptr = std::shared_ptr< person_data >;
    data_ptr _d;
public:

    /* Construct a ‹person› instance from an existing data pointer.
     * We would actually like to make this ‹private›, but that would
     * give us problems because we actually delegate the constructor
     * call to ‹std::vector›: we would have to make that a friend
     * class (but that would punch holes into the model... let's not
     * bother for now). */

    explicit person( data_ptr p ) : _d( p ) {}

    /* We need to be able to construct parent-less instances, since
     * the data ends somewhere and we can no longer provide the data
     * about parents. */

    explicit person( std::string name )
        : _d( std::make_shared< person_data >() )
    {
        _d->name = name;
    }

    /* The standard constructor for ‹person›, with two parents. We
     * take ‹person› by value since it's really just a pointer
     * anyway (we could perhaps save an refcount increment/decrement
     * pair by passing via ‹const› references). We also use
     * constructor delegation: in the initialization section, we
     * invoke the above ‘parent-less‘ constructor. This constructor
     * is also in charge of maintaining (half of) the
     * above-mentioned invariant by inserting our data pointer into
     * the ‹children› list of both the parents. */

    person( std::string name, person mother, person father )
        : person( name )
    {
        _d->mother = mother._d;
        _d->father = father._d;

        _d->mother->children.emplace_back( _d );
        _d->father->children.emplace_back( _d );
    }

    /* The other half of the invariant is maintained here, with the
     * help of ‹shared_ptr› destructors: if a ‹person› is completely
     * destroyed (i.e. no copies remain, i.e. the reference count on
     * the corresponding ‹person_data› drops to zero), all
     * ‹weak_ptr› instances pointing to it will automatically turn
     * into ‹null› pointers. We then simply filter those out to
     * obtain the correct list of children. */

    std::vector< person > children() const
    {
        std::vector< person > out;
        for ( const auto &c_weak : _d->children )
            if ( auto c = c_weak.lock() )
                out.emplace_back( c );
        return out;
    }

    /* A few simple accessors. */

    bool valid() const { return !!_d; }
    std::string name() const { return _d->name; }
    person mother() const { return person{ _d->mother }; }
    person father() const { return person{ _d->father }; }

    /* Equality: we base equality on «object identity»: copies of
     * the same person (even those that arise in a roundabout way,
     * without calling the copy constructor, e.g. those that arise
     * from the above ‹mother› and ‹father› accessors which
     * construct new ‹person› instances) will compare as equal. */

    bool operator==( person o ) const { return o._d == _d; }
};

int main() /* demo */
{
    person unknown( "unknown" );
    person a( "a", unknown, unknown );
    person b( "b", unknown, unknown );

    assert( a.mother().valid() );
    assert( a.father().valid() );
    assert( a.mother() == unknown );
    assert( a.father() == unknown );
    assert( !unknown.mother().valid() );
    assert( a.mother().name() == "unknown" );

    {
        person c( "c", a, b );
        person d( "d", a, b );

        person x( "x", unknown, unknown );
        person e( "e", c, x );

        /* Check that the ‹children› containers are correctly filled
         * in by the constructors. */

        assert( c.mother() == a );
        assert( a.children().size() == 2 );
        assert( b.children().size() == 2 );
        assert( c.children().size() == 1 );
        assert( x.children().size() == 1 );

        for ( const auto &ch : x.children() )
            assert( ch == e );

        for ( const auto &ch : c.children() )
            assert( ch == e );

        /* The instances ‹c›, ‹d›, ‹x› and ‹e› are destroyed at this
         * point (with no surviving copies). */
    }

    /* Check that the invariant is maintained. */

    assert( a.children().empty() );
    assert( b.children().empty() );
}
