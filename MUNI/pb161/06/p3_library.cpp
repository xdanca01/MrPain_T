#include <string>
#include <utility>
#include <cassert>

/* A very simple library model: patrons can borrow books and
 * borrowed books can be moved around, and must be eventually
 * returned. The library should have the following methods:
 *
 *   • ‹add_book›, which creates a book record based on 2 arguments
 *     – the title (a string) and the number of copies (an integer) of
 *     the book – and returns a suitable object (a handle) to
 *     represent that book,
 *   • ‹add_patron› which creates a patron, given a name (a string),
 *     and again returns a suitable object to represent the patron.
 *
 * It should be possible to call ‹borrow› on objects which represent
 * patrons, passing either a reference to a library or another
 * patron as the first argument, and the book handle as a second
 * argument. It returns ‹true› if the borrowing was a success, or
 * ‹false› otherwise (no copies were available). If a patron is
 * destroyed, all books in their possession return to the library.
 * Destroying a book handle does nothing. */

class library;

/* Finally, the class ‹loan› holds information about a loan. Both
 * ‹library› and the patron object get a method ‹give› which returns
 * a ‹loan› object associated with the book passed to it, and
 * ‹take›, which accepts a ‹loan› object and takes ownership of the
 * associated book. If ‹give› is called on an object which does not
 * have a copy of the requested book, return an invalid (empty)
 * ‹loan› object.
 *
 * While a book is held in a ‹loan› instance, it is not in the
 * possession of any of the objects, but it is checked out from the
 * library. If the ‹loan› object is destroyed without being ‹take›n
 * by anyone, the book returns to the library. */

class loan;

int main()
{
    library l;
    auto rama  = l.add_book( "Rendezvous with Rama", 2 );
    auto rings = l.add_book( "Lord of the Rings", 1 );
    auto wrath = l.add_book( "Grapes of Wrath", 2 );

    auto alice = l.add_patron( "Alice" );

    {
        auto bob = l.add_patron( "Bob" );

        assert(  alice.borrow( l, rama ) );
        assert(  alice.borrow( l, rings ) );
        assert( !alice.borrow( l, rings ) );
        assert( !bob.borrow( l, rings ) );
        assert( !alice.borrow( bob, rings ) );
        assert( !alice.borrow( bob, wrath ) );
        assert(  bob.borrow( l, wrath ) );
        assert(  alice.borrow( bob, wrath ) );
        assert(  bob.borrow( l, wrath ) );
        assert(  bob.borrow( alice, rama ) );
        loan l_0;

        {
            loan l_1 = alice.give( rings );
            loan l_2 = std::move( l_1 );
            loan l_3 = alice.give( rama );
            l_0 = std::move( l_3 );
        }

        assert( !bob.borrow( alice, rama ) );
        assert(  bob.borrow( l, rings ) );
        assert(  alice.borrow( l, rama ) );
        bob.take( std::move( l_0 ) );
        assert(  alice.borrow( bob, rama ) );
        l.take( bob.give( wrath ) );

        assert(  bob.borrow( l, wrath ) );
        assert( !bob.borrow( l, wrath ) );
        assert( !alice.borrow( l, wrath ) );
    }

    assert(  alice.borrow( l, wrath ) );
    assert( !alice.borrow( l, wrath ) );

    return 0;
}
