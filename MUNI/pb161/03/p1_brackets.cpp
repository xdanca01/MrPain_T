#include <string>
#include <cassert>
#include <vector>

/* Check that curly and square brackets in a given string balance
 * out correctly. */

bool balanced( const std::string &s ){
    std::vector<char> zas;
    for(auto c : s){
        if(c == '}' || c == ')' || c == ']'){
            if(zas.empty()) return false;
            for(auto l = zas.rbegin(); l != zas.rend();++l){
                if(zas.empty()) return false;
                zas.pop_back();
                if((c == '}' && *l == '{') || (c == ')' && *l == '(') || (c == ']' && *l == '['))
                    break;
            }
        }
        else if(c == '{' || c == '(' || c == '[') zas.push_back(c);
    }
    if(zas.empty() == false) return false;
    return true;
}

int main()
{
    assert(  balanced( "{}" ) );
    assert(  balanced( "{(}" ) );
    assert(  balanced( "{{}}" ) );
    assert(  balanced( "{[]}" ) );
    assert(  balanced( "{[][]}" ) );
    assert(  balanced( "{[][]}[]" ) );
    assert( !balanced( "{[}]" ) );
    assert( !balanced( "{" ) );
    assert( !balanced( "]" ) );
    assert(  balanced( "{[x]y[z]} [x]" ) );

    return 0;
}
