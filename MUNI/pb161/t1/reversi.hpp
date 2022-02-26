/* The subject of this task is the game of reversi (also known as
 * othello), played by two players on a 3D board (a box) of a given
 * shape (given as 3 even, non-negative numbers). Size 0 in a given
 * direction means the board is infinite in that direction.
 *
 * The cells are cubes (a cube has 8 vertices, 12 edges and 6
 * faces). The coordinates start at the center (which is a vertex)
 * and extend in two directions (positive and negative) along the 3
 * axes. The 8 cells which share the center vertex have coordinates
 * [1, 1, 1], [1, 1, -1], [1, -1, 1], [1, -1, -1], …
 *
 * The rules are a straightforward extension of the standard 2D
 * rules into three dimensions:
 *
 *  • each player starts with 4 stones placed around the center such
 *    that no two (of the same colour) share a face, with white
 *    taking the [1, 1, 1] cell,
 *  • players take turns in placing a new stone, which must be
 *    placed adjacent (share an edge, vertex or a face) to an
 *    opposing player's stone, and enemy stones must form a
 *    straight, uninterrupted line to one of current players'
 *    own stones (along straight lines – sharing a face, along
 *    diagonals which share an edge, or along diagonals which
 *    share a vertex),
 *  • the colour of all opposing stones on all such lines connecting
 *    the new stone to existing stones of the current player is
 *    flipped.
 *
 * The white player starts. The game ends when no new stones can be
 * placed and the player with more stones wins. It must be possible
 * to make a copy of an in-progress game. */

class reversi
{
public:
    reversi( int x_size, int y_size, int z_size );

    /* Place a stone at the given coordinates. If the placement was
     * legal, returns ‹true› and the next call places a stone of the
     * opposing player; otherwise, no change is made, the function
     * returns ‹false› and the same player must try a different
     * move.
     *
     * As a special case, if the current player has no legal move
     * left, but the game is not finished, ‹play› must be called
     * with ⟦x = y = z = 0⟧ to continue. Doing this is illegal in
     * any other circumstances.
     *
     * It is undefined behaviour to call ‹play› when the game is
     * already over. */

    bool play( int x, int y, int z );

    /* Return true if the game is finished (no further moves are
     * possible). */

    bool finished() const;

    /* Only defined if the game is already over (i.e. ‹finished›
     * would return ‹true›). Returns the difference in the number of
     * stones of each player: positive for white's victory, negative
     * for black's victory, 0 for a draw. */

    int result() const;
};
