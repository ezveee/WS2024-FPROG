#include <iostream>
#include <functional>
#include <numeric>
#include <optional>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;
using namespace std::placeholders;

constexpr int connectFourRows = 6;
constexpr int connectFourCols = 7;

using Line = vector<char>;
using Board = vector<Line>;
using Lines = vector<Line>;


// functions to implement
    // isBoardValid
        // the board is valid if:
        // the dimensions are 7x6
        // the token difference between the players isnt bigger than 1
        // there are no invalid tokens
    
    // xWins
        // x wins if:
        // there are 4 xs in a line
        // or
        // there are 4 xs in a column
        // or
        // there are 4 xs in a diagonal

    // oWins
        // o wins if:
        // there are 4 os in a line
        // or
        // there are 4 os in a column
        // or
        // there are 4 xs in a diagonal

    // draw
        // is the board full and theres no win?

// the board
    // 7x6
    /*
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' '}
    */
    // 6 lines of 7 tokens
    // 7 columns of 6 tokens
    // 5 valid diagonals

    // functions
        // allLines
        // allColumns
        // allDiagonals
        // allLinesColumnsAndDiagonals




/*
    UTILITY
*/

// used to access coordinates
// given a line of length 7, it returns a vector {0, 1, 2, ... , 6}
auto toRange = [](const auto& collection){
    vector<int> range(collection.size());
    iota(begin(range), end(range), 0);
    return range;
};

template<typename DestinationType>
auto transformAll = [](const auto& source, auto fn){
    DestinationType result;
    result.reserve(source.size());
    transform(source.begin(), source.end(), back_inserter(result), fn);
    return result;
};

auto concatenate = [](const auto& first, const auto& second){
    auto result{first};
    result.insert(result.end(), make_move_iterator(second.begin()), make_move_iterator(second.end()));
    return result;
};

// would be used for diagonals
// auto concatenate3 = [](const auto& first, const auto& second, const auto& third){
//     return concatenate(concatenate(first, second), third);
// };

auto any_of_collection = [](const auto& collection, auto fn){
    return any_of(collection.begin(), collection.end(), fn);
};

auto all_of_collection = [](const auto& collection, auto fn){
    return all_of(collection.begin(), collection.end(), fn);
};



/*
    BOARD ACCESS AND PROJECTION
*/

using Coordinate = pair<int, int>;

auto accessAtCoordinates = [](const auto& board, const Coordinate& coordinate){
    return board[coordinate.first][coordinate.second];
};

// Coordinates for a row
auto projectCoordinates = [](const auto& board, const auto& coordinates){
    const auto boardElementFromCoordinates = bind(accessAtCoordinates, board, _1);
    return transformAll<Line>(coordinates, boardElementFromCoordinates);
};

auto lineCoordinates = [](const auto& board, const auto& lineIndex){
    const auto range = toRange(board[0]);  // Use range of column length (7)
    return transformAll<vector<Coordinate>>(range, [lineIndex](const auto& index){ return make_pair(lineIndex, index); });
};

auto line = [](const auto& board, const int lineIndex){
    return projectCoordinates(board, lineCoordinates(board, lineIndex));
};

// Coordinates for a column
auto columnCoordinates = [](const auto& board, const auto& columnIndex){
    const auto range = toRange(board);  // Use range of row length (6)
    return transformAll<vector<Coordinate>>(range, [columnIndex](const auto& index){ return make_pair(index, columnIndex); });
};

auto column = [](const auto& board, const auto& columnIndex){
    return projectCoordinates(board, columnCoordinates(board, columnIndex));
};


/*
    DIAGONALS (LEFT OUT FOR NOW)
*/



/*
    ALL LINES AND COLUMNS (NOT YET DIAGONALS)
*/

auto allLines = [](const auto& board) {
    const auto range = toRange(board);
    return transformAll<Lines>(range, [board](auto index) { return line(board, index); });
};

auto allColumns = [](const auto& board) {
    const auto range = toRange(board[0]);
    return transformAll<Lines>(range, [board](auto index) { return column(board, index); });
};

auto allLinesAndColumns = [](const auto& board) {
    return concatenate(allLines(board), allColumns(board));
};



/*
    WIN CONDITIONS
*/

auto hasFourConsecutive = [](const auto& line, const auto& tokenToCheck) {
    return accumulate(line.begin(), line.end(), 0, [tokenToCheck](int consecutiveCount, const auto& token) -> int {
        if (consecutiveCount == 4) return consecutiveCount;
        return (token == tokenToCheck) ? consecutiveCount + 1 : 0;
    }) >= 4;
};


// X Wins and O Wins
auto xWins = [](const auto& board) {
    return any_of_collection(allLinesAndColumns(board), [](const auto& line){ return hasFourConsecutive(line, 'X'); });
};

auto oWins = [](const auto& board) {
    return any_of_collection(allLinesAndColumns(board), [](const auto& line){ return hasFourConsecutive(line, 'O'); });
};

auto full = [](const auto& board){
    return all_of_collection(board, [](const auto& line) { 
        return all_of_collection(line, [](const auto& token){ return token != ' '; });
    });
};

auto draw = [](const auto& board){
    return full(board) && !xWins(board) && !oWins(board);
};



/*
    CHECK BOARD AND TOKEN VALIDITY
*/

auto areDimensionsValid = [](const auto& board) {
    return (board.size() == 6) &&
           std::all_of(board.begin(), board.end(), [](const auto& row) {
               return row.size() == 7;
           });
};

auto isTokenDifferenceValid = [](const auto& board) {
    int xCount = std::accumulate(board.begin(), board.end(), 0, [](int acc, const auto& row) {
        return acc + std::count_if(row.begin(), row.end(), [](char token) { return token == 'X'; });
    });

    int oCount = std::accumulate(board.begin(), board.end(), 0, [](int acc, const auto& row) {
        return acc + std::count_if(row.begin(), row.end(), [](char token) { return token == 'O'; });
    });

    return std::abs(xCount - oCount) <= 1;
};

auto areTokensValid = [](const auto& board) {
    return std::all_of(board.begin(), board.end(), [](const auto& row) {
        return std::all_of(row.begin(), row.end(), [](char token) {
            return token == 'X' || token == 'O' || token == ' ';
        });
    });
};

auto isBoardValid = [](const auto& board) {
    return areDimensionsValid(board) && 
           isTokenDifferenceValid(board) && 
           areTokensValid(board);
};






/*
    TESTS
*/

TEST_CASE("Check valid board dimensions") {
    Board board(connectFourRows, Line(connectFourCols, ' '));
    CHECK(areDimensionsValid(board));
}

TEST_CASE("Check invalid board dimensions") {
    Board wrongBoard(connectFourRows - 1, Line(connectFourCols, ' '));
    CHECK(!areDimensionsValid(wrongBoard));
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
    CHECK(areTokensValid(board));
}

TEST_CASE("Check invalid tokens") {
    Board board{
        {'X', 'O', 'Z', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '},
        {'X', 'O', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK(!areTokensValid(board));
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
    CHECK(isTokenDifferenceValid(board));
}

TEST_CASE("Check invalid token difference") {
    Board board{
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '},
        {'X', 'X', ' ', ' ', ' ', ' ', ' '}
    };
    CHECK(!isTokenDifferenceValid(board));
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
    CHECK(xWins(board));
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
    CHECK(oWins(board));
}

TEST_CASE("Check draw") {
    Board board{
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'}
    };
    CHECK(draw(board));
}

TEST_CASE("Check if board is valid") {
    Board board{
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'}
    };
    CHECK(isBoardValid(board));
}

TEST_CASE("Check if board is valid") {
    Board board{
        {'X', 'X', 'X', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'},
        {'X', 'X', 'X', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X', 'O', 'O', 'X'}
    };
    CHECK(!draw(board));
}