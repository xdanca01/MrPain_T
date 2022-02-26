#include <vector>
#include <cassert>

/* We will assume some familiarity with C (or at least some
 * C-style braces-and-semicolons language, like Java). First
 * things first: subroutines, statements, types and «values». In
 * C++, variables, containers and so on hold «values». Assignment
 * updates those values and does not rebind the name to a different
 * object.  If you come from Java or Python, this is a bit of a
 * culture shock. See also lectures. */

/* In this demo, we will implement the mother of all programming
 * language examples, the Fibonacci sequence (forget hello world,
 * this is not that kind of a course). First the function
 * (subroutine) «signature» -- in order come:
 *
 *  • «return type» – in this case ‹std::vector< int >›, then
 *  • the name of the subroutine – ‹fibonacci›, and finally
 *  • the argument list: ‹int n›. */

std::vector< int > fibonacci( int n )

    /* A vector is a «sequence container» -- it holds a sequence of
     * values. In C++, containers are «generic», that is,
     * parametrized by the type of their elements, and these type
     * parameters are specified in angle brackets. In this case, we
     * are declaring that ‹fibonacci› returns a vector (sequence) of
     * integers (‹int› is the ‘default’ integer type in C++). The
     * curly braces after the signature enclose the function «body».
     * */

{
    /* The body is a sequence of statements, separated by semicolons
     * (with the exception of compound statements – which are
     * enclosed in braces and are «not» followed by a semicolon).
     * The first statement in this function is a «local variable
     * declaration», which consists of the «type» (the already
     * familiar ‹std::vector< int >›), possible «declarators» (like
     * pointers and references… again, we will get to those later –
     * there are none in this particular case) and the «name» of the
     * variable: ‹fib›. */

    std::vector< int > fib;

    /* Vectors are generalized arrays: unlike traditional C arrays,
     * they can be resized on demand. To set the size of a vector,
     * we can use its method ‹resize›: to call a «method» of an
     * object (and vector is an object), we use the following
     * syntax:
     *
     *  • the variable holding the object – ‹fib›, then
     *  • a dot,
     *  • then the name of the method to call,
     *  • then an argument list, enclosed in parentheses.
     *
     * Of course, like everything in C++, method calls can get a lot
     * more complicated, and it is a topic that we will likewise
     * revisit. */

    fib.resize( n, 1 );

    /* Now that ‹fib› is an appropriately sized vector, with the
     * number 1 stored at each index, we can go on to rewrite the
     * values to the actual Fibonacci sequence. We will use a ‹for›
     * loop, which you probably know from C – the ‹for› statement
     * has 3 sections enclosed in parentheses and separated by
     * semicolons:
     *
     *  • initialisation, which usually declares or initializes the
     *    loop variable,
     *  • the loop condition,
     *  • the iteration expression, which is executed after every
     *    iteration, before the loop condition.
     *
     *  The head of the loop is followed by a statement, which is
     *  the «body»: the code that is repeatedly executed. Often,
     *  this is a compound statement (enclosed in braces) but it
     *  doesn't have to. */

    for ( int i = 2; i < n; ++ i )

        /* In this case, the body consists of a single statement: an
         * assignment, which updates the ‹i›-th position in the
         * vector ‹fib› with the sum of the values stored at the two
         * preceding indices. Square brackets after a variable name
         * indicate the «indexing operator» and works analogously to
         * array indexing in C. */

        fib[ i ] = fib[ i - 1 ] + fib[ i - 2 ];

    /* The return statement does two things, like in most imperative
     * languages: it provides the «return value», and it immediately
     * stops execution of the function, transferring control back to
     * the caller. */

    return fib;
}

/* All demonstrations and exercises in this collection contain a
 * short collection of «test cases». In the demos, they usually
 * serve to show how the code explained in the main part works, and
 * for you to change and experiment. */

int main() /* demo */
{
    std::vector fib_7{ 1, 1, 2, 3, 5, 8, 13 };
    std::vector fib_1{ 1 };

    assert( fibonacci( 7 ) == fib_7 );
    assert( fibonacci( 1 ) == fib_1 );
}
