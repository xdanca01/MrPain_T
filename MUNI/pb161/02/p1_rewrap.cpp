#include <string>
#include <cassert>
#include <iostream>

/* A different take on word-wrapping. The idea is very similar to last week –
 * break lines at the first opportunity after you ran out of space in your
 * current line. The twist: do this by modifying the input string.
 * Additionally, undo existing line breaks if they are in the wrong spot. */

void create_line( std::string &str)
{
    int i = 0;
    for(char c : str)
    {
        if(c == '\n')
        {
            str[i] = ' ';
        }
        ++i;
    }
}

void cut_line( std::string &str, int cols ){
    int char_count = 0, i = 0;
    for(char c : str)
    {
        ++char_count;
        if(char_count >= cols && c == ' ' )
        {
            char_count = 0;
            str[i] = '\n';
        }
        ++i;
    }
}

void rewrap( std::string &str, int cols ){
    create_line(str);
    cut_line(str, cols);
    std::cout << str << "\n";
    std::cout << "-------\n";
}

int main()
{
    std::string s = "lorem ipsum dolor sit amet";
    rewrap( s, 5 );
    assert( s == "lorem\nipsum\ndolor\nsit amet" );
    rewrap( s, 7 );
    assert( s == "lorem ipsum\ndolor sit\namet" );
    rewrap( s, 20 );
    assert( s == "lorem ipsum dolor sit\namet" );
    rewrap( s, 30 );
    assert( s == "lorem ipsum dolor sit amet" );
    rewrap( s, 1 );
    assert( s == "lorem\nipsum\ndolor\nsit\namet" );

    return 0;
}
