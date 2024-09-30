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

// Utility to check if board dimensions are valid for Connect Four
auto isValidBoardDimension = [](const Board& board) {
    return board.size() == connectFourRows && all_of(board.begin(), board.end(), [](const Line& line) {
        return line.size() == connectFourCols;
    });
};

// Utility to count tokens and validate that the difference in number of tokens is <= 1
auto checkTokenDifference = [](const Board& board) {
    int countX = 0, countO = 0;
    for (const auto& row : board) {
        countX += count(row.begin(), row.end(), 'X');
        countO += count(row.begin(), row.end(), 'O');
    }
    return abs(countX - countO) <= 1;
};

// Utility to check for invalid tokens (anything other than 'X', 'O', or empty ' ')
auto checkForInvalidTokens = [](const Board& board) {
    for (const auto& row : board) {
        if (any_of(row.begin(), row.end(), [](char token) { return token != 'X' && token != 'O' && token != ' '; })) {
            return false;
        }
    }
    return true;
};

// Check if the board is correctly filled with valid tokens
auto validateBoard = [](const Board& board) {
    if (!isValidBoardDimension(board)) {
        throw runtime_error("Board dimensions are incorrect. It should be 7x6.");
    }
    if (!checkTokenDifference(board)) {
        throw runtime_error("The number of tokens for the two players differs by more than 1.");
    }
    if (!checkForInvalidTokens(board)) {
        throw runtime_error("Board contains invalid tokens.");
    }
};

auto concatenate = [](const auto& first, const auto& second){
    auto result{first};
    result.insert(result.end(), make_move_iterator(second.begin()), make_move_iterator(second.end()));
    return result;
};

auto concatenate3 = [](const auto& first, const auto& second, const auto& third){
    return concatenate(concatenate(first, second), third);
};

// Project coordinates on the board
auto projectCoordinates = [](const auto& board, const auto& coordinates) {
    Line result;
    result.reserve(coordinates.size());
    for (const auto& coord : coordinates) {
        result.push_back(board[coord.first][coord.second]);
    }
    return result;
};

// Check for winning conditions (4 in a row, column, or diagonal)
auto checkLineForWin = [](const Line& line, char playerToken) {
    int consecutive = 0;
    for (char token : line) {
        if (token == playerToken) {
            consecutive++;
            if (consecutive == 4) return true;
        } else {
            consecutive = 0;
        }
    }
    return false;
};

// Find horizontal, vertical, and diagonal coordinates for Connect Four
auto horizontalCoordinates = [](const Board& board) {
    Lines lines;
    for (int row = 0; row < connectFourRows; ++row) {
        vector<pair<int, int>> line;
        for (int col = 0; col < connectFourCols; ++col) {
            line.push_back(make_pair(row, col));
        }
        lines.push_back(projectCoordinates(board, line));
    }
    return lines;
};

auto verticalCoordinates = [](const Board& board) {
    Lines lines;
    for (int col = 0; col < connectFourCols; ++col) {
        vector<pair<int, int>> line;
        for (int row = 0; row < connectFourRows; ++row) {
            line.push_back(make_pair(row, col));
        }
        lines.push_back(projectCoordinates(board, line));
    }
    return lines;
};

auto diagonalCoordinates = [](const Board& board) {
    Lines diagonals;

    // Main diagonals (bottom-left to top-right)
    for (int row = 0; row < connectFourRows - 3; ++row) {
        for (int col = 0; col < connectFourCols - 3; ++col) {
            vector<pair<int, int>> diag;
            for (int k = 0; k < 4; ++k) {
                diag.push_back(make_pair(row + k, col + k));
            }
            diagonals.push_back(projectCoordinates(board, diag));
        }
    }

    // Secondary diagonals (top-left to bottom-right)
    for (int row = 3; row < connectFourRows; ++row) {
        for (int col = 0; col < connectFourCols - 3; ++col) {
            vector<pair<int, int>> diag;
            for (int k = 0; k < 4; ++k) {
                diag.push_back(make_pair(row - k, col + k));
            }
            diagonals.push_back(projectCoordinates(board, diag));
        }
    }

    return diagonals;
};

// Combine all lines (horizontal, vertical, diagonal)
auto allLinesColumnsAndDiagonals = [](const Board& board) {
    auto horizontal = horizontalCoordinates(board);
    auto vertical = verticalCoordinates(board);
    auto diagonal = diagonalCoordinates(board);
    return concatenate3(horizontal, vertical, diagonal);
};

// Check if a player has won
auto hasWon = [](const Board& board, char playerToken) {
    const auto lines = allLinesColumnsAndDiagonals(board);
    return any_of(lines.begin(), lines.end(), [&](const Line& line) {
        return checkLineForWin(line, playerToken);
    });
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
