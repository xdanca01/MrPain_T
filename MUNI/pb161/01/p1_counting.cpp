#include <string>
#include <cassert>

/* In this exercise, we will work with strings in a read-only way:
 * by counting things in them. Write two functions, ‹word_count› and
 * ‹line_count›: the former will count words (runs of characters
 * without spaces) and the latter will count the number of non-empty
 * lines. Use range ‹for› to look at the content of the string. */

/* Here are the prototypes of the functions -- you can simply turn
 * those into definitions. We pass arguments by ‹const› references:
 * for now, consider this to be a bit of syntax, the purpose of
 * which is to avoid making a copy of the string. It will be
 * explained in more detail later. Also notice that in a prototype,
 * the arguments do not need to be named (but you will have to give
 * them names to use them). */



int word_count( const std::string &s )
{
    int wc = 0;
    bool char_before = false;
    for(char c : s)
    {
        if(c == '\n' || c == ' ')
        {
            char_before = false;
        }
        else
        {
            if(!char_before)
            {
                ++wc;
            }
            char_before = true;
        }
    }
    return wc;
}

int line_count( const std::string &s )
{
    bool first = true;
    int lc = 0;
    for(char c : s)
    {
        if(c == '\n') first = true;
        else if(c == ' ');
        else
        {
            if(first) ++lc;
            first = false;
        }
    }
    return lc;
}

int main()
{
    assert( line_count( "hello\nworld" ) == 2 );
    assert( line_count( "hello\nworld\n" ) == 2 );
    assert( line_count( "hello\n\nworld\n" ) == 2 );
    assert( line_count( "\nhello\n\nworld\n" ) == 2 );
    assert( line_count( "\nhello\n \nworld\n" ) == 2 );
    assert( line_count( "x" ) == 1 );
    assert( line_count( "x\n" ) == 1 );
    assert( line_count( "" ) == 0 );

    assert( word_count( "hello" ) == 1 );
    assert( word_count( "hello world" ) == 2 );
    assert( word_count( "hello world " ) == 2 );
    assert( word_count( "hello\nworld " ) == 2 );
    assert( word_count( "" ) == 0 );

    return 0;
}
