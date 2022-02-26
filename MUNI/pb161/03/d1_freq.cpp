#include <cassert>
#include <string>
#include <map>

/* In this demo, we will build up a histogram of word appearances.
 * Since we will want to process the input incrementally, we will
 * implement the word counter as a class with 2 methods: ‹process›,
 * which will add each word that appears in its argument to the
 * histogram, and ‹count›, which takes a single-word string as an
 * argument and returns how many times it has been encountered by
 * ‹process›. */

class freq
{

    /* All the heavy lifting will be done by the standard
     * associative container, ‹std::map›. We will use ‹std::string›
     * as the key type (holding the word of interest) and ‹int› as
     * the value type: the number of appearances of this word. */

    std::map< std::string, int > _counter;

public:

    /* We will first implement a helper method, for counting a
     * single word. It will be convenient to ignore empty strings
     * here, so we will do just that. Notice that we use the
     * indexing (subscript) operator to access the value which
     * ‹std::map› associates with the given key. Also notice that
     * the key is automatically added to the map in case it is not
     * yet present. */

    void add( const std::string &word )
    {
        if ( !word.empty() )
            _counter[ word ] ++;
    }

    /* Now the main workhorse: ‹process› takes an input string,
     * decomposes it into individual words and counts them. Notice
     * the use of ‹+=› to append a letter to an existing string. */

    void process( const std::string &str )
    {
        std::string word;

        for ( auto c : str )
            if ( std::isblank( c ) )
            {
                add( word );
                word.clear();
            }
            else
                word += c;

        /* Do not forget to add the last word, in case it was not
         * followed by a blank. */

        add( word );
    }

    /* We would clearly like to mark the ‹count› method, which
     * simply returns information about the observed frequency of a
     * word, as ‹const›. However, the subscript operator on an
     * ‹std::map› is not ‹const› – this is because, as we have
     * mentioned earlier, should the key not be present in the map,
     * it will be added automatically, thus changing the content of
     * the container.
     *
     * Instead, we can ask ‹std::map› to check whether the key is
     * present (by using ‹count›), without adding it. If the key is
     * missing, we simply return 0. Otherwise, we ask the map to
     * find the value associated with the key, again without adding
     * it if it is missing. Note that dereferencing the result of
     * ‹find› is undefined if the key is not present (in this case,
     * we know for sure that the key is present – we just checked).
     * All ‹std::map› methods which we used are marked ‹const› and
     * hence we can mark our ‹count› method ‹const› as well, as we
     * desired. */

    int count( const std::string &s ) const
    {
        return _counter.count( s ) ? _counter.find( s )->second : 0;
    }

};

int main() /* demo */
{
    freq f;

    /* We create a ‹const› alias for ‹f›, so that we check that it
     * is indeed possible to call ‹count› on it. */
    const freq &cf = f;

    assert( cf.count( "hello" ) == 0 );
    assert( cf.count( "" ) == 0 );

    f.process( "hello world" );
    assert( cf.count( "hello" ) == 1 );
    assert( cf.count( "hell" ) == 0 );
    assert( cf.count( "world" ) == 1 );
    assert( cf.count( " world" ) == 0 );

    f.process( "hello hello" );
    assert( cf.count( "hello" ) == 3 );
    assert( cf.count( "world" ) == 1 );

    f.process( "world hello world hello world" );
    assert( cf.count( "hello" ) == 5 );
    assert( cf.count( "world" ) == 4 );
}
