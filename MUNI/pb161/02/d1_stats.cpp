#include <vector>
#include <cmath>
#include <cassert>

/* In this demo, we will do some basic descriptive statistics. */

/* Last week, we have used «constant references» to pass «input»
 * arguments into functions. We will now see how to use non-constant
 * (mutable) references to implement «output» and «in/out»
 * arguments. The syntax for a mutable reference is simply the type,
 * the reference declarator (‹&›) and the name of the argument, i.e.
 * dropping the ‹const› (compare ‹data› vs ‹median› in the following
 * function signature. */

void stats( const std::vector< double > &data,
            double &median, double &mean, double &stddev )
{
    int n = data.size();
    double sum = 0, square_error_sum = 0;

    for ( double x_i : data )
        sum += x_i;

    /* Notice that we do not «read» the value of ‹median› before
     * overwriting it with the resulting value: this is a hallmark
     * of an «output argument» – it is never read before being
     * written by the function. */

    mean = sum / n;

    if ( n % 2 == 1 )
        median = data[ n / 2 ];
    else
        median = ( data[ n / 2 ] + data[ n / 2 - 1 ] ) / 2;

    /* However, after we have assigned a value to ‹mean›, we can
     * continue to use it like a normal read-write variable. It is
     * important that the read cannot be reached without executing
     * the write first (e.g. it would be a problem if the write
     * above was conditional). */

    for ( double x_i : data )
        square_error_sum += ( x_i - mean ) * ( x_i - mean );

    double variance = square_error_sum / ( n - 1 );
    stddev = std::sqrt( variance );

    /* No return statement: the function was declared with ‹void› as
     * its return type, meaning that it does not return anything.
     * The values are all passed to the caller via output arguments.
     * */
}

int main() /* demo */
{
    double median, mean, stddev;
    std::vector< double > sample = { 2, 4, 4, 4, 5, 5, 5, 7, 9 };
    stats( sample, median, mean, stddev );

    assert( mean == 5 );
    assert( median == 5 );
    assert( stddev == 2 );

    sample.push_back( 1100 );
    stats( sample, median, mean, stddev );

    assert( median == 5 );
    assert( mean > 100 );
    assert( stddev > 100 );
}
