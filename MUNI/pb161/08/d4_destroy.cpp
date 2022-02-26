#include <cassert>
#include <memory>

/* In this (entirely synthetic, sorry) example, we will look at
 * object destruction, especially in the context of polymorphism. */

/* We first set up a few counters to track constructor and
 * destructor calls. */

static int bad_base_counter = 0, bad_derived_counter = 0,
           good_base_counter = 0, good_derived_counter = 0;

class bad_base
{
public:
    virtual int bad_dummy() { return 0; }

    bad_base() { bad_base_counter ++; }

    /* We will knowingly break the «virtual destructor rule» here,
     * to see «why» the rule exists. */

    ~bad_base() { bad_base_counter --; }
};

class good_base
{
public:
    virtual int good_dummy() { return 0; }

    good_base() { good_base_counter ++; }

    /* Notice the ‹virtual›. */

    virtual ~good_base() { good_base_counter --; }
};

/* Let's add some innocent derived classes. */

class bad_derived : public bad_base
{
public:
    bad_derived() { bad_derived_counter ++; }
    ~bad_derived() { bad_derived_counter --; }
};

class good_derived : public good_base
{
public:
    good_derived() { good_derived_counter ++; }

    /* It is good practice to also add ‹override› to destructors of
     * derived classes. This will tell the compiler we expect the
     * base class to have a ‹virtual› destructor which we are
     * extending. The compiler will emit an error if the base class
     * destructor is (through some unfortunate accident) not marked
     * as ‹virtual›. */

    ~good_derived() override { good_derived_counter --; }
};

int main() /* demo */
{

    /* For regular variables, everything works as expected:
     * constructors and destructors of all classes in the hierarchy
     * are called. */
    {
        bad_base bb;
        assert( bad_base_counter == 1 );
        bad_derived bd;
        assert( bad_base_counter == 2 );
        assert( bad_derived_counter == 1 );
    }

    assert( bad_base_counter == 0 );
    assert( bad_derived_counter == 0 );

    /* Same thing with virtual destructors. */
    {
        good_base gb;
        assert( good_base_counter == 1 );
        good_derived gd;
        assert( good_base_counter == 2 );
        assert( good_derived_counter == 1 );
    }

    assert( good_base_counter == 0 );
    assert( good_derived_counter == 0 );

    /* However, problems start if an instance is destroyed through a
     * pointer whose static type disagrees with the dynamic type.
     * This cannot happen with references (unless the destructor is
     * called explicitly), but it is entirely plausible with
     * pointers, including smart pointers. Let's first demonstrate
     * the case that works: ‹good_derived›. */

    using good_ptr = std::unique_ptr< good_base >;

    /* Please make good note of the fact, that the static type of
     * the pointer refers to ‹good_base›, but the actual value
     * stored in it has dynamic type ‹good_derived›. */

    {
        good_ptr gp = std::make_unique< good_derived >();
        assert( good_base_counter == 1 );
        assert( good_derived_counter == 1 );
    }

    /* Since the ‹unique_ptr› went out of scope, the instance stored
     * behind it was destroyed. The counters should be both zero
     * again. */

    assert( good_base_counter == 0 );
    assert( good_derived_counter == 0 );

    /* Let's observe what happens with the ‹bad_base› and
     * ‹bad_derived› combination. */

    using bad_ptr = std::unique_ptr< bad_base >;

    {
        bad_ptr bp = std::make_unique< bad_derived >();
        assert( bad_base_counter == 1 );
        assert( bad_derived_counter == 1 );
    }

    /* The pointer went out of scope. Since the destructor was
     * called using «static dispatch», only the «base class»
     * destructor was called. This is of course very problematic,
     * since resources were leaked and invariants broken. */

    assert( bad_base_counter == 0 );
    assert( bad_derived_counter == 1 );

    /* Please note that some compilers (recent ‹clang› versions)
     * will «emit a warning» if this happens. Unfortunately, this is
     * not the case with ‹gcc› 9.2 which we are using (and which is
     * a rather recent compiler). It is therefore unadvisable to
     * rely on the compiler to catch this type of problem. Stay
     * vigilant. */
}
