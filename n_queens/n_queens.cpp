#include <iostream>

enum TileState {
    TileStateOpen,
    TileStateOccupied,
    TileStateConflict
};

constexpr auto BoardSize = 8;
using Board = TileState[BoardSize][BoardSize];

constexpr auto BoxLineEW = "\u2500";
constexpr auto BoxLineNS = "\u2502";

constexpr auto BoxCornerNW = "\u250c";
constexpr auto BoxCornerNE = "\u2510";
constexpr auto BoxCornerSE = "\u2518";
constexpr auto BoxCornerSW = "\u2514";

constexpr auto BoxJointA = "\u253c";
constexpr auto BoxJointEWN = "\u2534";
constexpr auto BoxJointNSE = "\u251c";
constexpr auto BoxJointEWS = "\u252c";
constexpr auto BoxJointNSW = "\u2524";

constexpr auto SymbolTriangle = "\u25ff";
constexpr auto SymbolCircle = "\u25cf";

class GridDrawer {
public:
    static void draw_top_rule(unsigned columns, unsigned column_width);
    static void draw_middle_rule(unsigned columns, unsigned column_width);
    static void draw_bottom_rule(unsigned columns, unsigned column_width);
};

void GridDrawer::draw_top_rule(unsigned columns, unsigned column_width)
{
    std::cerr << BoxCornerNW;
    for (unsigned i = 1; i < column_width * columns; ++i)
        std::cerr << (i % column_width == 0 ? BoxJointEWS : BoxLineEW);
    std::cerr << BoxCornerNE << "\n";
}

void GridDrawer::draw_middle_rule(unsigned columns, unsigned column_width)
{
    std::cerr << BoxJointNSE;
    for (unsigned i = 1; i < column_width * columns; ++i)
        std::cerr << (i % column_width == 0 ? BoxJointA : BoxLineEW);
    std::cerr << BoxJointNSW << "\n";
}

void GridDrawer::draw_bottom_rule(unsigned columns, unsigned column_width)
{
    std::cerr << BoxCornerSW;
    for (unsigned i = 1; i < column_width * columns; ++i)
        std::cerr << (i % column_width == 0 ? BoxJointEWN : BoxLineEW);
    std::cerr << BoxCornerSE << "\n";
}

constexpr auto ColumnWidth = 4;

void draw_board(Board board)
{
    GridDrawer::draw_top_rule(BoardSize, ColumnWidth);

    for (int i = 0; i < BoardSize; ++i) {
        std::cerr << BoxLineNS;

        for (int j = 0; j < BoardSize; ++j) {
            std::cerr << " ";

            switch (board[i][j]) {
            case TileStateOccupied:
                std::cerr << SymbolCircle;
                break;
            case TileStateConflict:
                std::cerr << SymbolTriangle;
                break;
            default:
                std::cerr << " ";
                break;
            }

            std::cerr << " " << BoxLineNS;
        }

        std::cerr << "\n";

        if (i < BoardSize - 1)
            GridDrawer::draw_middle_rule(BoardSize, ColumnWidth);
        else
            GridDrawer::draw_bottom_rule(BoardSize, ColumnWidth);
    }
}

void place_queen(Board board, unsigned row, unsigned col)
{
    board[row][col] = TileStateOccupied;

    // Mark other spaces in the same column as conflicted.
    for (unsigned i = 0; i < BoardSize; ++i)
        if (board[i][col] == TileStateOpen)
            board[i][col] = TileStateConflict;

    // Mark other spaces in the same row as conflicted.
    for (unsigned i = 0; i < BoardSize; ++i)
        if (board[row][i] == TileStateOpen)
            board[row][i] = TileStateConflict;

    // Mark other spaces on the negative diagonal as conflicted.
    int j = 0;
    for (int i = (BoardSize / 2) - col + 1; i < BoardSize; ++i) {
        if (i >= 0 && board[i][j] == TileStateOpen)
            board[i][j] = TileStateConflict;

        ++j;
    }

    // Mark other spaces on the positive diagonal as conflicted.
    j = BoardSize - 1;
    for (int i = BoardSize - row - 1; i < BoardSize; ++i) {
        if (board[j][i] == TileStateOpen)
            board[j][i] = TileStateConflict;

        --j;
    }
}

int main()
{
    Board board = { {TileStateOpen} };

    for (unsigned i = 0; i < 1; ++i) {
        place_queen(board, 6, 5);
        draw_board(board);
        std::cerr << "Round " << i << "\n\n";
    }
}
