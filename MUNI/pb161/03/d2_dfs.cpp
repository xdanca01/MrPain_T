#include <vector>
#include <map>
#include <set>
#include <cassert>

/* In this demo, we will do some basic exploration of directed
 * graphs. Probably the simplest possible algorithm for that is
 * «recursive depth-first search», so that is what we will use. We
 * will be interested in the question ‘is vertex ⟦a⟧ reachable from
 * vertex ⟦b⟧?’. */

/* The input graph is given using adjacency lists: the ‹graph› type
 * gives the successors for each vertex present in the graph. Please
 * note that in principle, the set of vertices does not need to be
 * contiguous, or composed of only small numbers (hence the
 * ‹std::map› and not an ‹std::vector›). */

using edges = std::vector< int >;
using graph = std::map< int, edges >;

/* Besides the graph itself, we will need to represent the «visited
 * set» – the set of vertices that have already been visited by the
 * algorithm. In a graph with loops, not keeping track of this
 * information would lead to infinite recursion. In an acyclic
 * graph, it could still lead to exponential running time. Since in
 * pseudocode, this is a literal set, using ‹std::set› sounds like a
 * good idea. Indeed, ‹std::set› is a container which keeps at most
 * one copy of any element, and provides efficient (logarithmic
 * time) lookup and insertion. */

using visited = std::set< int >;

/* The main recursive function needs 2 auxiliary arguments: the set
 * of already-visited vertices ‹seen› and the boolean ‹moved›, which
 * guards against the case when we ask whether a vertex is reachable
 * from itself – this is traditionally only answered in affirmative
 * when there is a path from that vertex to itself, but a naive
 * solution would always answer ‹true›. Hence, we need to ensure at
 * least one edge was traversed before returning ‹true›. The
 * question this function answers is ‘is there a path which starts
 * in vertex ‹from›, does not visit any of the vertices in ‹seen›
 * and ends in ‹to›?’
 *
 * Notice that ‹seen› is passed around by reference: there is only a
 * single instance of this set, shared by all recursive calls. That
 * is, if one branch of the search visits a vertex, it will also be
 * avoided by any subsequent sibling branches (not just by the
 * recursive calls made within the branch). */

bool is_reachable_rec( const graph &g, int from, int to,
                       visited &seen, bool moved )
{
    /* The base case of the recursion is when we reach the target
     * vertex and have already traversed at least one edge. In this
     * case, we return ‹true›: we have found a path connecting the
     * two vertices. */

    if ( from == to && moved )
        return true;

    /* The main loop looks at each successor of ‹from› and calls
     * ‹is_reachable› recursively, asking whether there is a path
     * from the successor to the goal state, avoiding the current
     * state. The result of the ‹g.at› call is a (reference to) the
     * ‹edges› container (i.e. the ‹std::vector› of vertices). Hence
     * ‹next› ranges over the successors of the vertex ‹from›. */

    for ( auto next : g.at( from ) )

        /* In case ‹next› was not yet seen (it is not present in the
         * visited set), mark it as visited and proceed to explore
         * it recursively. */

        if ( !seen.count( next ) )
        {
            seen.insert( next );
            if ( is_reachable_rec( g, next, to, seen, true ) )
                return true;
        }

    /* We have failed to find a satisfactory path, having exhausted
     * all the options. Return ‹false›. */

    return false;
}

/* Finally, we provide a simple wrapper around the recursive
 * function above, providing initial values for the two auxiliary
 * arguments. Check whether ‹to› can be reached by following one or
 * more edges if we start at ‹from›. */

bool is_reachable( const graph &g, int from, int to )
{
    visited seen;
    return is_reachable_rec( g, from, to, seen, false );
}

int main() /* demo */
{
    graph g{ { 1, { 2, 3, 4 } },
             { 2, { 1, 2 } },
             { 3, { 3, 4 } },
             { 4, {} },
             { 5, { 3 }} };

    assert(  is_reachable( g, 1, 1 ) );
    assert( !is_reachable( g, 4, 4 ) );
    assert(  is_reachable( g, 1, 2 ) );
    assert(  is_reachable( g, 1, 3 ) );
    assert(  is_reachable( g, 1, 4 ) );
    assert( !is_reachable( g, 4, 1 ) );
    assert(  is_reachable( g, 3, 3 ) );
    assert( !is_reachable( g, 3, 1 ) );
    assert(  is_reachable( g, 5, 4 ) );
    assert( !is_reachable( g, 5, 1 ) );
    assert( !is_reachable( g, 5, 2 ) );
}
