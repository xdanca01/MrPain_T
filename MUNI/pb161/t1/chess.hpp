#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
/* The goal of this task is to implement the standard rules of
 * chess. */

struct position
{
    int file; /* column 'letter', a = 1, b = 2, ... */
    int rank; /* row number, starting at 1 */
    position() = default;
    position(int col, int row) : file(col), rank(row) {};
};

enum class piece_type
{
    pawn, rook, knight, bishop, queen, king
};

enum class player { white, black};

/* The following are the possible outcomes of ‹play›. The outcomes
 * are shown in the order of precedence, i.e. the first applicable
 * is returned.
 *
 * ├┄┄┄┄┄┄┄┄┄┄┄┄┄┄▻┼◅┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┤
 * │ ‹capture›     │ the move was legal and resulted in a capture │
 * │ ‹ok›          │ the move was legal and was performed         │
 * │ ‹no_piece›    │ there is no piece on the ‹from› square       │
 * │ ‹bad_piece›   │ the piece on ‹from› is not ours              │
 * │ ‹bad_move›    │ this move is not available for this piece    │
 * │ ‹blocked›     │ another piece is in the way                  │
 * │ ‹lapsed›      │ «en passant» capture is no longer allowed    │
 * │ ‹has_moved›   │ one of the castling pieces has already moved │
 * │ ‹in_check›    │ the player is currently in check and the     │
 * │               │ move does not get them out of it             │
 * │ ‹would_check› │ the move would place the player in check     │
 * │ ‹bad_promote› │ promotion to a pawn or king was attempted    │
 *
 * Attempting an «en passant» when the pieces are in the wrong place
 * is a ‹bad_move›. In addition to ‹has_moved›, (otherwise legal)
 * castling may give:
 *
 *  • ‹blocked› – some pieces are in the way,
 *  • ‹in_check› – the king is currently in check,
 *  • ‹would_check› – would pass through or end up in check. */

enum class result
{
    capture, ok, no_piece, bad_piece, bad_move, blocked, lapsed,
    in_check, would_check, has_moved, bad_promote
};

struct occupant
{
    bool is_empty = true;
    bool moved = false;
    player owner;
    piece_type piece;
    occupant() = default;
    occupant(bool state, player own, piece_type p) : is_empty(state), owner(own), piece(p){} 
};

class chess
{
    //column,row
    occupant board[8][8];
    //actual player
    player actual_player = player::white;
    //last valid move (from, to)
    position last_valid_move[2];
public:

    /* Construct a game of chess in its default starting position.
     * The first call to ‹play› after construction moves a piece of
     * the white player. */

    chess();
    void create_rooks();
    void create_knights();
    void create_bishops();
    void create_kings();
    void create_queens();
    void create_pawns();
    void new_game();
    result check_move(position from, position to, piece_type promotion);
    result check_move_rook(position from, position to);
    result check_move_queen(position from, position to);
    result check_move_pawn(position from, position to, piece_type promotion);
    result check_move_knight(position from, position to);
    result check_move_king(position from, position to);
    result check_move_bishop(position from, position to);
    result promote(position at, piece_type promotion);
    result find_check(position from, position to);
    void set_result(result& res, result comp);

    /* Move a piece currently at ‹from› to square ‹to›:
     *
     *  • in case the move places a pawn at its 8th rank (rank 8 for
     *    white, rank 1 for black), it is promoted to the piece given
     *    in ‹promote› (otherwise, the last argument is ignored),
     *  • castling is described as a king move of more than one square,
     *  • if the result is an error (not ‹capture› nor ‹ok›), calling
     *    ‹play› again will attempt another move by the same player. */

    result play( position from, position to,
                 piece_type promote = piece_type::rook);

    result check_bounds(position from, position to);

    /* Which piece is at the given position? */

    occupant at( position ) const;
};
