#include <vector>
#include <string>
#include <cassert>
#include <iostream>

/* Write a function that breaks up a string into individual words. We consider
 * a word to be any string without whitespace (spaces, newlines, tabs) in it. */

/* Since we are lazy to type the long-winded type for a vector of
 * strings, we define a «type alias». The syntax is different from C,
 * but it should be clearly understandable. We will encounter this
 * construct many times in the future. */

using string_vec = std::vector< std::string >;

/* The output of ‹words› should be a vector of strings, where each
 * of the strings contains a single word from ‹in›. */

string_vec words( const std::string &in )
{
    std::string word;
    string_vec found_words;
    for(char c : in)
    {
        if(c == ' ' || c == '\n' || c == '\t')
        {
            if(!word.empty())
            {
                found_words.push_back(word);
                word.clear();
            }
        }
        else
        {
            word.push_back(c);
            
        }
    }
    if(!word.empty()) found_words.push_back(word);
    return found_words;
}

int main()
{
    assert( ( words( "lorem ipsum dolor" ) ==
              string_vec{ "lorem", "ipsum", "dolor" } ) );
    assert( ( words( "hello, world" ) == string_vec{ "hello,", "world" } ) );
    assert( ( words( "hello  world" ) == string_vec{ "hello", "world" } ) );
    assert( ( words( "hello\nworld\n" ) == string_vec{ "hello", "world" } ) );
    assert( ( words( " hello \n world" ) == string_vec{ "hello", "world" } ) );

    return 0;
}
