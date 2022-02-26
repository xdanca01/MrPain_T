#include "chess.hpp"

//
void tc1(){
    chess game;
    position from = position(1, 2), to = position(1, 3);
    piece_type p = piece_type::rook;
    assert(game.play(from, to, p) == result::ok);
    assert(game.play(from, to, p) == result::no_piece);
    assert(game.play(position(2, 2), to, p) == result::bad_piece);
    assert(game.play(position(1, 9), to, p) == result::bad_move);
    assert(game.play(position(1, 8), position(1, 7), p) == result::blocked);
    assert(game.play(position(1, 7), position(1, 6), p) == result::ok);
    assert(game.play(to, from, p) == result::bad_move);
    //assert(game.play(position(), position(), p) == result::ok);
}

void tc2(){
    chess game;
    position from = position(1, 2), to = position(1, 3);
    piece_type p = piece_type::rook;
    assert(game.play(position(5, 2), position(5, 4), p) == result::ok);
    assert(game.play(position(4, 7), position(4, 5), p) == result::ok);
    assert(game.play(position(5, 4), position(4, 5), p) == result::capture);
    assert(game.play(position(4, 8), position(4, 5), p) == result::capture);
    assert(game.play(position(4, 2), position(4, 4), p) == result::ok);
    assert(game.play(position(3, 8), position(8, 3), p) == result::ok);
    assert(game.play(position(7, 1), position(7, 3), p) == result::bad_move);
    assert(game.play(position(7, 1), position(8, 3), p) == result::capture);
    assert(game.play(position(2, 8), position(3, 6), p) == result::ok);
    assert(game.play(position(7, 2), position(7, 4), p) == result::ok);
    assert(game.play(position(5, 8), position(3, 8), p) == result::ok);
    assert(game.play(position(7, 4), position(7, 5), p) == result::ok);
    assert(game.play(position(6, 7), position(6, 5), p) == result::ok);
    assert(game.play(position(7, 5), position(6, 6), p) == result::capture);
    assert(game.play(position(6, 5), position(6, 4), p) == result::no_piece);
}

void my_tests(){
    tc1();
    tc2();
}

chess::chess(){
    new_game();
}

result chess::find_check(position from, position to){
    result res = result::bad_move;
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    std::vector<position> positions = {from, to};
    //add position between
    if(col_diff == 2) positions.push_back(position((from.file+to.file)/2,from.rank));
    //check for each position check
    position comp = position(1, 1);
    for(auto p : positions){
        //col
        for(int i = 1;i < 9;++i){
            comp.file = i;
            //row
            for(int j = 1;j < 9;++j){
                comp.rank = j;
                //check only opposite owner fields
                if(at(comp).is_empty == false && at(from).owner != at(comp).owner)
                    //king would make recursion so exclude it
                    if(at(comp).piece != piece_type::king){
                        //if move can be done, then king can be captured
                        res = check_move(comp, p, piece_type::rook);
                        //if king can be captured, then king is in check
                        if(res == result::capture)
                            return result::in_check;
                        //if piece can move to position, then king can be checked
                        else if(res == result::ok)
                            return result::would_check;
                    }
                    //check king (cant be close less than 1)
                    else if(abs(to.file-comp.file) < 1 | abs(to.rank-comp.rank) < 1){
                        return result::would_check;
                    }
            }
        }
    }
    return res;
}

result chess::promote(position at, piece_type promotion){
    if(promotion == piece_type::pawn || promotion == piece_type::king) return result::bad_promote;
    board[at.file - 1][at.rank - 1].piece = promotion;
    return result::ok;
}

void chess::set_result(result& res, result comp){
    //previous result was ok or capture, needs to be changed
    if(res == result::ok || res == result::capture) res = comp;
    else if(int(comp) < int(res)) res = comp;
}

void chess::create_pawns(){
    occupant pawn = occupant(false, player::white, piece_type::pawn);
    //white down
    pawn.owner = player::white;
    for(int i = 0; i < 8;++i) board[i][1] = pawn;
    //black top
    pawn.owner = player::black;
    for(int i = 0; i < 8;++i) board[i][6] = pawn;
}

void chess::create_rooks(){
    occupant rook = occupant(false, player::white, piece_type::rook);
    board[0][0] = rook;
    board[7][0] = rook;
    //black change owner
    rook.owner = player::black;
    board[0][7] = rook;
    board[7][7] = rook;
}

void chess::create_knights(){
    occupant knight = occupant(false, player::white, piece_type::knight);
    board[1][0] = knight;
    board[6][0] = knight;
    //black change owner
    knight.owner = player::black;
    board[1][7] = knight;
    board[6][7] = knight;
}

void chess::create_bishops(){
    occupant bishop = occupant(false, player::white, piece_type::bishop);
    board[2][0] = bishop;
    board[5][0] = bishop;
    //black change owner
    bishop.owner = player::black;
    board[2][7] = bishop;
    board[5][7] = bishop;
}

void chess::create_queens(){
    occupant queen = occupant(false, player::white, piece_type::queen);
    board[3][0] = queen;
    queen.owner = player::black;
    board[3][7] = queen;
}

void chess::create_kings(){
    occupant king = occupant(false, player::white, piece_type::king);
    board[4][0] = king;
    king.owner = player::black;
    board[4][7] = king;
}

result chess::check_move_rook(position from, position to){
    result res = result::ok;
    occupant fig1 = at(from), fig2 = at(to);
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    //check movement(row or column not both)
    if((row_diff >  0 && col_diff >  0)
    || (row_diff == 0 && col_diff == 0))
        return result::bad_move;
    //check collision
    int my_min, my_max;
    if(row_diff > 0){
        my_min = std::min(from.rank, to.rank);
        my_max = std::max(from.rank, to.rank);
    }
    else{
        my_min = std::min(from.file, to.file);
        my_max = std::max(from.file, to.file);
    }
    //no piece should be in way (not checking end)
    for(int i = my_min + 1; i < my_max; ++i){
        if(row_diff){
            if(at(position(from.file, i)).is_empty != true) return result::blocked;
        }
        else
            if(at(position(i, from.rank)).is_empty != true) return result::blocked;
    }
    if(fig2.is_empty == false) set_result(res, result::capture);
    if(fig2.owner == fig1.owner) return result::blocked;
    return res;
}

result chess::check_move_queen(position from, position to){
    result res = result::ok;
    occupant fig1 = at(from);
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    //No move or wrong move
    if( (row_diff == 0 && col_diff == 0) || 
        (row_diff != col_diff && row_diff != 0 && col_diff != 0))
            return result::bad_move;
    //bishop move
    else if(row_diff == col_diff) set_result(res, check_move_bishop(from, to));
    //rook move
    else set_result(res, check_move_rook(from, to));
    return res;
}

result chess::check_move_pawn(position from, position to, piece_type promotion){
    result res = result::ok;
    occupant fig1 = at(from), fig2 = at(to);
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    //wrong move(more than 2(row), more than 1(col), more than 1(col,row)(1+1)
    if  (row_diff >  2 || col_diff >  1 || 
        (row_diff >= 1 && col_diff >= 1 && col_diff + row_diff > 2) || 
        (row_diff == 0 && col_diff >= 1))
            return result::bad_move;
    //move
    else if(row_diff == 2 || row_diff == 1){
        //check starting position(first move)
        if(row_diff == 2)
        {
            //owner
            if(fig1.owner == player::white && from.rank != 2) return result::bad_move;
            else if (fig1.owner == player::black && from.rank != 7) return result::bad_move;
            //blocked between 2 fields
            if(at(position(from.file, (from.rank+to.rank)/2)).is_empty == false) set_result(res, result::blocked);
        }
        //check direction(cant go back)
        if(fig1.owner == player::white && from.rank > to.rank) return result::bad_move;
        else if(fig1.owner == player::black && from.rank < to.rank) return result::bad_move;
        //blocked, same color
        if(fig2.is_empty == false && fig1.owner == fig2.owner) set_result(res, result::blocked);
    }
    //pawn to any piece --- check promotion
    if( (fig1.owner == player::white && to.rank == 8) || 
        (fig1.owner == player::black && to.rank == 1))
            //cant promote to king or pawn
            set_result(res, promote(from, promotion));
    //capture
    if(row_diff == 1 && col_diff == 1){
        //check en passan (same col, row differs 1 -- abs(TO-prev[1]) && abs(TO-prev[0]), piece was pawn)
        if( last_valid_move[0].file == to.file && last_valid_move[1].file == to.file &&
            abs(to.rank-last_valid_move[0].rank) == 1 &&
            abs(to.rank-last_valid_move[1].rank) == 1 &&
            at(last_valid_move[1]).piece == piece_type::pawn) set_result(res, result::capture);
        else if(fig2.is_empty == true) return result::bad_move;
        else res = result::capture;
    }
    return res;
}
result chess::check_move_knight(position from, position to){
    result res = result::ok;
    occupant fig1 = at(from), fig2 = at(to);
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    if ( !(row_diff == 2 && col_diff == 1) && !(col_diff == 2 && row_diff == 1))
        return result::bad_move;
    else if(fig2.is_empty == false && fig2.owner == fig1.owner) set_result(res, result::blocked);
    if(fig2.is_empty == false && fig2.owner != fig1.owner) set_result(res, result::capture);
    return res;
}

result chess::check_move_king(position from, position to){
    result res = result::ok;
    occupant fig1 = at(from), fig2 = at(to);
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    //can move only 1 tile (castling can move 2)
    if(row_diff != 1 && col_diff != 1 && (col_diff != 2 && row_diff != 0)) return result::bad_move;
    //castling
    if(col_diff == 2)
    {
        //rook and king are 1 field between each other
        bool right = (8-to.file) == 1;
        //cant do castling (king moved || right rook moved || left rook moved)
        if  (fig1.moved == true || 
            (right == true  && at(position(8, to.rank)).moved == true) ||
            (right == false && at(position(1, to.rank)).moved == true))
                set_result(res, result::has_moved);
        //check blocks between castling
        //from king to right rook
        if(right == true){
            for(int i = from.file + 1; i < 8; ++i)
                //piece between right rook and king
                if(at(position(i, to.rank)).is_empty == false){
                    set_result(res, result::blocked);
                    break;
                }
        }
        //from left rook to king
        else if(right == false){
            for(int i = 2; i < from.file; ++i)
                //piece between left rook and king
                if(at(position(i, to.rank)).is_empty == false){
                    set_result(res, result::blocked);
                    break;
                }
        }
    }
    //check at to
    result comp = find_check(from, to);
    if(comp == result::would_check || comp == result::in_check) set_result(res, comp);
    if(fig2.is_empty == false && fig2.owner != fig1.owner) set_result(res, result::capture);
    return res;
}

result chess::check_move_bishop(position from, position to){
    result res = result::ok;
    occupant fig1 = at(from), fig2 = at(to);
    int row_diff = std::abs(from.rank - to.rank), col_diff = std::abs(from.file - to.file);
    // col = row
    if(row_diff != col_diff || row_diff <= 0 || col_diff <= 0) return result::bad_move;
    //check blocking between from -> to
    for(int i = std::abs(from.file - to.file) - 1; i > 0; --i){
        occupant o;
        // to ---- from
        if(from.file > to.file){
            // "to/from"
            if(from.rank > to.rank) o = at(position(to.file+i, to.rank+i));
            // "to\from"
            else o = at(position(to.file+i, to.rank-i));
        }
        // from ---- to
        else{
            // "from/to"
            if(to.rank > from.rank) o = at(position(from.file+i, from.rank+i));
            // "from\to"
            else o = at(position(from.file+i, from.rank-i));
        }
    }
    //Some piece at TO
    if(fig2.is_empty == false){
        //Blocked at TO by own piece
        if(fig2.owner == fig1.owner) set_result(res, result::blocked);
        //check capture
        else set_result(res, result::capture);
    }
    return res;
}

result chess::check_move(position from, position to, piece_type promotion){
    result res = result::ok;
    occupant fig1 = at(from);
    if(fig1.piece == piece_type::rook) set_result(res, check_move_rook(from, to));
    else if(fig1.piece == piece_type::queen) set_result(res, check_move_queen(from, to));
    else if(fig1.piece == piece_type::pawn) set_result(res, check_move_pawn(from, to, promotion));
    else if(fig1.piece == piece_type::knight) set_result(res, check_move_knight(from, to));
    else if(fig1.piece == piece_type::king) set_result(res, check_move_king(from, to));
    else if(fig1.piece == piece_type::bishop) set_result(res, check_move_bishop(from, to));
    return res;
}

result chess::check_bounds(position from, position to){
    if(from.file > 8 || from.file < 1 || from.rank > 8 || from.rank < 1
    || to.file > 8   || from.file < 1 || to.rank   > 8 || to.rank  < 1)
        return result::bad_move;
    return result::ok;
}

result chess::play( position from, position to, piece_type promote ){
    //Check position from, to (bounds)
    result res = check_bounds(from, to);
    if(res != result::ok) return res;
    occupant fig1 = at(from), fig2 = at(to);
    //Check that field is not empty
    if(fig1.is_empty == true) return result::no_piece;
    //Check, that player is owner
    else if(fig1.owner != actual_player) return result::bad_piece;
    //check move validity
    res = check_move(from, to, promote);
    //if ok set next round (player, save previous play, move fig)
    if(res == result::ok || res == result::capture){
        //en passan
        if(res == result::capture && fig1.piece == piece_type::pawn && fig2.is_empty == true)
            board[last_valid_move[1].file-1][last_valid_move[1].rank-1].is_empty = true;
        //castling
        else if(fig1.piece == piece_type::king && abs(from.file-to.file) == 2){
            //right rook
            if(to.file == 7) {
                board[7][0].moved = true;
                board[5][0] = board[7][0];
                board[7][0].is_empty = true;
            }
            //left rook
            else{
                board[0][0].moved = true;
                board[2][0] = board[0][0];
                board[0][0].is_empty = true;
            }
        }
        //move fig1
        fig1.moved = true;
        board[to.file-1][to.rank-1] = fig1;
        board[from.file-1][from.rank-1].is_empty = true;
        //save move
        last_valid_move[0] = from;
        last_valid_move[1] = to;
        actual_player = (actual_player == player::white) ? player::black : player::white;
    }
    return res;
}

occupant chess::at( position p) const{
    return board[p.file-1][p.rank-1];
}

void chess::new_game(){
    chess::create_rooks();
    chess::create_knights();
    chess::create_bishops();
    chess::create_kings();
    chess::create_queens();
    chess::create_pawns();
}

/* You can put implementations of non-template methods and functions
 * into this file, as you see fit. Do not put anything after the
 * ‹main› function, and make sure its prototype remains untouched.
 * Feel free to add testing code to ‹main›. */

int main()
{
    my_tests();
    /* tests go here */
}