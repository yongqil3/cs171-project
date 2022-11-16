#include "StudentAI.h"
#include <limits>
// The following part should be completed by students.
// The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col, int row, int p)
    : AI(col, row, p)
{
    board = Board(col, row, p);
    board.initializeGame();
    player = 2;
}
vector<Move> getallpossiblemove(Board board, int player)
{
    vector<vector<Move>> moves;
    vector<Move> possible_moves;
    moves = board.getAllPossibleMoves(player);
    for (unsigned int x = 0; x < moves.size(); x++)
    {
        for (unsigned int y = 0; y < moves[x].size(); y++)
        {
            possible_moves.push_back(moves[x][y]);
        }
    }
    return possible_moves;
}

int switchSide(int player)
{
    return player == 1 ? 2 : 1;
}

int getKingCount(Board board, int player)
{
    int whiteSideKingCount = 0;

    int blackSideKingCount = 0;
    // means in black side
    for (unsigned int x = 0; x < board.row; x++)
    {
        for (unsigned int y = 0; y < board.col; y++)
        {
            if (board.board[x][y].color == "W")
            {
                whiteSideKingCount++;
            }
            else if (board.board[x][y].color == "B")
            {
                blackSideKingCount++;
            }
        }
    }
    return player == 1 ? blackSideKingCount : whiteSideKingCount;
}

int getManCount(Board board, int player)
{
    int count = 0;
    for (int i = 0; i < board.row; i++)
    {
        for (int j = 0; j < board.col; j++)
        {
            if (board.board[i][j].color == "b" && player == 1)
            {
                count++;
            }
            else if (board.board[i][j].color == "w" && player == 2)
            {
                count++;
            }
        }
    }
    return count;
}

int score_board(Board board, int player)
{
    int score = 0;
    int opponent_score = 0;
    int king_weight = 2;
    int regular_weight = 1;
    for (unsigned int x = 0; x < board.col; x++)
    {
        for (unsigned int y = 0; y < board.row; y++)
        {
            if (player == 2)
            {
                score += getKingCount(board, player) * king_weight;
                score += getManCount(board, player) * regular_weight;
            }
            else
            {
                opponent_score += getKingCount(board, player) * 2;
                opponent_score += getManCount(board, player) * regular_weight;
                opponent_score += (1 - (y / board.row)) * regular_weight;
            }
        }
    }
    // if (player == 1)
    // {
    //     score += (board.whiteCount / (board.col * board.p / 2) * 0.1);
    //     opponent_score += (board.blackCount / (board.col * board.p / 2) * 0.1);
    // }
    // else
    // {
    //     score += (board.blackCount / (board.col * board.p / 2) * 0.1);
    //     opponent_score += (board.whiteCount / (board.col * board.p / 2) * 0.1);
    // }

    int result = 0;
    if (player == 1)
    {
        result = opponent_score - score;
    }
    else
    {
        result = score - opponent_score;
    }
    return result;
}

int alpha_beta(Board board, int depth, int alpha, int beta, int player)
{
    // this will currently value a win at the same rate as a tie
    if (depth == 0)
    {
        // return get_piece_count(t) - get_piece_count(switchSide(t));
        return score_board(board, player);
    }
    // vector<vector<Move>> moves = board.getAllPossibleMoves(t);
    vector<Move> moves = getallpossiblemove(board, player);
    for (int i = 0; i < moves.size(); i++)
    {
        Move my_move = moves[i];
        board.makeMove(my_move, player);
        int score = -alpha_beta(board, depth - 1, -beta, -alpha, switchSide(player));
        if (score >= alpha)
        {
            alpha = score;
        }
        board.Undo();
        if (alpha >= beta)
        {
            break;
        }
    }
    return alpha;
}

Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    }
    else
    {
        board.makeMove(move, player == 1 ? 2 : 1);
    }
    vector<Move> possible_moves = getallpossiblemove(board, player);
    Move best_move = possible_moves[rand() % possible_moves.size()];
    int score = numeric_limits<int>::min();
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();
    int current;
    for (int x = 0; x < possible_moves.size(); x++)
    {
        Move my_move = possible_moves[x];
        board.makeMove(my_move, player);
        current = -alpha_beta(board, 3, alpha, beta, switchSide(player));
        board.Undo();
        if (current >= score)
        {
            score = current;
            best_move = my_move;
        }
    }
    board.makeMove(best_move, player);
    return best_move;
}
