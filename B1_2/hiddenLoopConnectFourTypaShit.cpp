#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "functional"
#include <list>

using namespace std;
using namespace std::placeholders;

enum Result {
    XWins,
    OWins,
    GameNotOverYet,
    Draw
};

enum Token {
    X,
    O,
    Blank
};

using Line = vector<Token>;

class ConnectFourBoard
{
private:
    vector<Line> _board;

public:
    ConnectFourBoard(int rows = 6, int cols = 7) 
        : _board(rows, Line(cols, Token::Blank)) {}

    ConnectFourBoard(const vector<Line>& initial) : _board{initial} {
        // Check for any invalid tokens
        for (const auto& row : _board) {
            for (const auto& token : row) {
                if (token != Token::X && token != Token::O && token != Token::Blank) {
                    throw invalid_argument("Board contains an invalid token.");
                }
            }
        }
    }

    bool notFilledYet() const
    {
        for (const auto& row : _board)
        {
            if (any_of(row.begin(), row.end(), [](Token t) { return t == Token::Blank; }))
            {
                return true;
            }
        }
        return false;
    }

    bool checkFourInARow(const vector<Token>& line, Token token) const
    {
        int count = 0;
        for (auto t : line)
        {
            count = (t == token) ? count + 1 : 0;
            if (count == 4) return true;
        }
        return false;
    }

    bool anyLineFilledWith(Token token) const
    {
        for (const auto& row : _board)
        {
            if (checkFourInARow(row, token))
            {
                return true;
            }
        }
        return false;
    }

    bool anyColumnFilledWith(Token token) const
    {
        for (std::size_t col = 0; col < _board[0].size(); ++col)
        {
            vector<Token> column;
            for (std::size_t row = 0; row < _board.size(); ++row)
            {
                column.push_back(_board[row][col]);
            }
            if (checkFourInARow(column, token))
            {
                return true;
            }
        }
        return false;
    }

    bool anyDiagonalFilledWith(Token token) const
    {
        int rows = _board.size();
        int cols = _board[0].size();

        // Check all diagonals from bottom-left to top-right
        for (int r = 3; r < rows; ++r)
        {
            for (int c = 0; c <= cols - 4; ++c)
            {
                if (_board[r][c] == token && _board[r-1][c+1] == token && 
                    _board[r-2][c+2] == token && _board[r-3][c+3] == token) {
                    return true;
                }
            }
        }

        // Check all diagonals from top-left to bottom-right
        for (int r = 0; r <= rows - 4; ++r) {
            for (int c = 0; c <= cols - 4; ++c) {
                if (_board[r][c] == token && _board[r+1][c+1] == token && 
                    _board[r+2][c+2] == token && _board[r+3][c+3] == token) {
                    return true;
                }
            }
        }

        return false;
    }

    bool isWinningBoard(Token token) const {
        return anyLineFilledWith(token) || anyColumnFilledWith(token) || anyDiagonalFilledWith(token);
    }

    const vector<Line>& getBoard() const {
        return _board;
    }

    bool isValidDimension() const {
        return _board.size() == 6 && _board[0].size() == 7;
    }
};

Result connectFourWinner(const ConnectFourBoard& board) {
    if (board.isWinningBoard(Token::X)) 
        return XWins;
    if (board.isWinningBoard(Token::O)) 
        return OWins;
    if (board.notFilledYet()) 
        return GameNotOverYet;
    return Draw;
}


// Test cases

// should fail -> 6x - 4o
TEST_CASE("Test difference in number of tokens for players") {
    ConnectFourBoard board({
        {Token::X, Token::X, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank},
        {Token::O, Token::O, Token::O, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::O, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank}
    });

    int xCount = 0, oCount = 0;
    for (const auto& row : board.getBoard()) {
        xCount += count(row.begin(), row.end(), Token::X);
        oCount += count(row.begin(), row.end(), Token::O);
    }

    CHECK(!(abs(xCount - oCount) <= 1)); // Difference in number of tokens should not be more than 1
}



// should succeed -> 6x - 5o
TEST_CASE("Test difference in number of tokens for players") {
    ConnectFourBoard board({
        {Token::X, Token::X, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank},
        {Token::O, Token::O, Token::O, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::O, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::O, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank}
    });

    int xCount = 0, oCount = 0;
    for (const auto& row : board.getBoard()) {
        xCount += count(row.begin(), row.end(), Token::X);
        oCount += count(row.begin(), row.end(), Token::O);
    }

    CHECK(abs(xCount - oCount) <= 1); // Difference in number of tokens should not be more than 1
}



TEST_CASE("Test for correct tokens") {
    ConnectFourBoard board({
        {Token::X, Token::O, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank},
        {Token::O, Token::O, Token::O, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::X, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank, Token::Blank}
    });

    for (const auto& row : board.getBoard()) {
        for (const auto& token : row) {
            CHECK((token == Token::X || token == Token::O || token == Token::Blank));
        }
    }
}


TEST_CASE("Test for wrong tokens") {
    vector<Line> wrongBoard = {
        {Token::X, Token::O, static_cast<Token>(3), Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::O, Token::O, Token::O, Token::Blank, Token::Blank, Token::Blank, Token::Blank},
        {Token::X, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank, Token::Blank}
    };

    CHECK_THROWS(ConnectFourBoard(wrongBoard));
}

TEST_CASE("Test for correct board dimensions") {
    ConnectFourBoard correctBoard;
    CHECK(correctBoard.isValidDimension());
}

TEST_CASE("Test for wrong board dimensions") {
    vector<Line> wrongBoard = {
        {Token::X, Token::O, Token::X, Token::X, Token::Blank, Token::Blank, Token::Blank}, // 7 Columns
        {Token::O, Token::O, Token::O, Token::Blank, Token::Blank, Token::Blank, Token::Blank}
    };
    
    ConnectFourBoard wrongDimensionBoard(wrongBoard);
    CHECK(!wrongDimensionBoard.isValidDimension());
}
