#include <iostream>
#include <functional>
#include <numeric>
#include <map>
#include <optional>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;
using namespace std::placeholders;

using Line = vector<char>;
using Board = vector<Line>;
using Lines = vector<Line>;

constexpr int connectFourRows = 6;
constexpr int connectFourCols = 7;

// Utility to create a range from 0 to n
auto toRange = [](int n) {
    vector<int> range(n);
    iota(range.begin(), range.end(), 0);
    return range;
};

// Generate diagonal coordinates
auto diagonalPoints = [](int startRow, int startCol, bool mainDiagonal) {
    Lines points;
    for (int k = 0; k < 4; ++k) {
        points.push_back({startRow + (mainDiagonal ? k : -k), startCol + k});
    }
    return points;
};

// Check board dimensions
auto isValidBoardDimension = [](const Board& board) {
    return board.size() == connectFourRows &&
           all_of(board.begin(), board.end(), [](const Line& line) {
               return line.size() == connectFourCols;
           });
};

// Count tokens and check their difference
auto checkTokenDifference = [](const Board& board) {
    int countX = accumulate(board.begin(), board.end(), 0, [](int acc, const Line& line) {
        return acc + count(line.begin(), line.end(), 'X');
    });
    int countO = accumulate(board.begin(), board.end(), 0, [](int acc, const Line& line) {
        return acc + count(line.begin(), line.end(), 'O');
    });
    return abs(countX - countO) <= 1;
};

// Check for invalid tokens
auto checkForInvalidTokens = [](const Board& board) {
    return all_of(board.begin(), board.end(), [](const Line& line) {
        return all_of(line.begin(), line.end(), [](char token) {
            return token == 'X' || token == 'O' || token == ' ';
        });
    });
};

// Validate board
auto validateBoard = [](const Board& board) {
    if (!isValidBoardDimension(board)) throw runtime_error("Invalid dimensions.");
    if (!checkTokenDifference(board)) throw runtime_error("Token count difference exceeds 1.");
    if (!checkForInvalidTokens(board)) throw runtime_error("Invalid tokens present.");
};

// Concatenate collections
auto concatenate = [](const auto& first, const auto& second) {
    auto result = first;
    result.insert(result.end(), make_move_iterator(second.begin()), make_move_iterator(second.end()));
    return result;
};

auto projectCoordinates = [](const Board& board, const auto& coordinates) {
    return transformAll<Line>(coordinates, [&board](const auto& coord) {
        return board[coord.first][coord.second];
    });
};

// Check for winning conditions
auto checkLineForWin = [](const Line& line, char playerToken) {
    return any_of(line.begin(), line.end(), [&](char token) {
        return token == playerToken;
    });
};

// Get coordinates for lines
auto getLineCoordinates = [](const Board& board) {
    Lines lines;
    for (int row = 0; row < connectFourRows; ++row) {
        lines.push_back(projectCoordinates(board, toRange(connectFourCols, row)));
    }
    return lines;
};

auto getColumnCoordinates = [](const Board& board) {
    Lines lines;
    for (int col = 0; col < connectFourCols; ++col) {
        lines.push_back(projectCoordinates(board, toRange(connectFourRows, col, true)));
    }
    return lines;
};

auto getDiagonalCoordinates = [](const Board& board) {
    Lines diagonals;

    for (int row = 0; row < connectFourRows - 3; ++row) {
        for (int col = 0; col < connectFourCols - 3; ++col) {
            diagonals.push_back(projectCoordinates(board, diagonalPoints(row, col, true)));
            diagonals.push_back(projectCoordinates(board, diagonalPoints(row + 3, col, false)));
        }
    }
    return diagonals;
};

// Combine all lines
auto allLinesColumnsAndDiagonals = [](const Board& board) {
    return concatenate(concatenate(getLineCoordinates(board), getColumnCoordinates(board)), getDiagonalCoordinates(board));
};

// Check if a player has won
auto hasWon = [](const Board& board, char playerToken) {
    return any_of(allLinesColumnsAndDiagonals(board).begin(), allLinesColumnsAndDiagonals(board).end(),
                  [&](const Line& line) { return checkLineForWin(line, playerToken); });
};

auto xWins = bind(hasWon, _1, 'X');
auto oWins = bind(hasWon, _1, 'O');



// Tests
TEST_CASE("Check valid board dimensions") {
    Board board(connectFourRows, Line(connectFourCols, ' '));
    CHECK_NOTHROW(validateBoard(board));
}

TEST_CASE("Check invalid board dimensions") {
    Board wrongBoard(connectFourRows - 1, Line(connectFourCols, ' '));
    CHECK_THROWS_AS(validateBoard(wrongBoard), runtime_error);
}

TEST_CASE("Check valid tokens") {
    Board board{
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK_NOTHROW(validateBoard(board));
}

TEST_CASE("Check invalid tokens") {
    Board board{
        {'X', 'O', 'Z', ' ', ' ', ' ', ' '}, // Z is invalid
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK_THROWS_AS(validateBoard(board), runtime_error);
}

TEST_CASE("Check token difference") {
    Board board{
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK_NOTHROW(validateBoard(board));
}

TEST_CASE("Check invalid token difference") {
    Board board{
        {'X', 'X', ' ', ' ', ' ', ' ', ' '}, // Too many X's
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK_THROWS_AS(validateBoard(board), runtime_error);
}

TEST_CASE("Check winning condition for X") {
    Board board{
        {'X', 'X', 'X', 'X', ' ', ' ', ' '},
        {'O', 'O', 'O', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK(xWins(board) == true);
}

TEST_CASE("Check winning condition for O") {
    Board board{
        {'O', 'O', 'O', 'O', ' ', ' ', ' '},
        {'X', 'X', 'X', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK(oWins(board) == true);
}
