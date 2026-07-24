#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

//==================================================
// Piece (Abstract Class) & Pieces
class Piece
{
protected:
    bool white;

public:
    Piece(bool w) { white = w; }
    virtual ~Piece() {}
    bool isWhite() { return white; }
    virtual char getSymbol() = 0;
    virtual bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b) = 0;
};

class Pawn : public Piece
{
public:
    Pawn(bool w) : Piece(w) {}
    char getSymbol()
    {
        if (white)
            return 'P';
        return 'p';
    }
    bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b)
    {
        int dir = white ? 1 : -1;
        int start = white ? 1 : 6;
        if (fc == tc && tr == fr + dir && b[tr][tc] == NULL)
            return true;
        if (fc == tc && fr == start && tr == fr + 2 * dir && b[fr + dir][fc] == NULL && b[tr][tc] == NULL)
            return true;
        if (abs(fc - tc) == 1 && tr == fr + dir && b[tr][tc] != NULL && b[tr][tc]->isWhite() != white)
            return true;
        return false;
    }
};

class Rook : public Piece
{
public:
    Rook(bool w) : Piece(w) {}
    char getSymbol()
    {
        if (white)
            return 'R';
        return 'r';
    }
    bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b)
    {
        if (fr != tr && fc != tc)
            return false;
        int rs = 0, cs = 0;
        if (tr > fr)
            rs = 1;
        if (tr < fr)
            rs = -1;
        if (tc > fc)
            cs = 1;
        if (tc < fc)
            cs = -1;
        int r = fr + rs, c = fc + cs;
        while (r != tr || c != tc)
        {
            if (b[r][c] != NULL)
                return false;
            r = r + rs;
            c = c + cs;
        }
        if (b[tr][tc] == NULL)
            return true;
        if (b[tr][tc]->isWhite() != white)
            return true;
        return false;
    }
};

class Knight : public Piece
{
public:
    Knight(bool w) : Piece(w) {}
    char getSymbol()
    {
        if (white)
            return 'N';
        return 'n';
    }
    bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b)
    {
        int dr = abs(tr - fr), dc = abs(tc - fc);
        if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2)))
            return false;
        if (b[tr][tc] == NULL)
            return true;
        if (b[tr][tc]->isWhite() != white)
            return true;
        return false;
    }
};

class Bishop : public Piece
{
public:
    Bishop(bool w) : Piece(w) {}
    char getSymbol()
    {
        if (white)
            return 'B';
        return 'b';
    }
    bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b)
    {
        if (abs(tr - fr) != abs(tc - fc))
            return false;
        int rs = tr > fr ? 1 : -1, cs = tc > fc ? 1 : -1;
        int r = fr + rs, c = fc + cs;
        while (r != tr)
        {
            if (b[r][c] != NULL)
                return false;
            r = r + rs;
            c = c + cs;
        }
        if (b[tr][tc] == NULL)
            return true;
        if (b[tr][tc]->isWhite() != white)
            return true;
        return false;
    }
};

class Queen : public Piece
{
public:
    Queen(bool w) : Piece(w) {}
    char getSymbol()
    {
        if (white)
            return 'Q';
        return 'q';
    }
    bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b)
    {
        Rook r(white);
        Bishop bi(white);
        if (r.isValidMove(fr, fc, tr, tc, b))
            return true;
        if (bi.isValidMove(fr, fc, tr, tc, b))
            return true;
        return false;
    }
};

class King : public Piece
{
public:
    King(bool w) : Piece(w) {}
    char getSymbol()
    {
        if (white)
            return 'K';
        return 'k';
    }
    bool isValidMove(int fr, int fc, int tr, int tc, vector<vector<Piece *>> &b)
    {
        if (abs(tr - fr) > 1 || abs(tc - fc) > 1)
            return false;
        if (b[tr][tc] == NULL)
            return true;
        if (b[tr][tc]->isWhite() != white)
            return true;
        return false;
    }
};
//==================================================

//==================================================
// Board
class Board
{
private:
    vector<vector<Piece *>> grid;

public:
    int enPassantCol;
    int enPassantRow;
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookLeftMoved;
    bool whiteRookRightMoved;
    bool blackRookLeftMoved;
    bool blackRookRightMoved;

    Board();
    ~Board();

    void setup();
    Piece *getPiece(int row, int col);
    void setPiece(int row, int col, Piece *piece);
    vector<vector<Piece *>> &getGrid();
};

Board::Board()
{
    grid.resize(8, vector<Piece *>(8, NULL));
    enPassantCol = -1;
    enPassantRow = -1;
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteRookLeftMoved = false;
    whiteRookRightMoved = false;
    blackRookLeftMoved = false;
    blackRookRightMoved = false;
    setup();
}

Board::~Board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (grid[i][j] != NULL)
            {
                delete grid[i][j];
                grid[i][j] = NULL;
            }
        }
    }
}

void Board::setup()
{
    grid[0][0] = new Rook(true);
    grid[0][1] = new Knight(true);
    grid[0][2] = new Bishop(true);
    grid[0][3] = new Queen(true);
    grid[0][4] = new King(true);
    grid[0][5] = new Bishop(true);
    grid[0][6] = new Knight(true);
    grid[0][7] = new Rook(true);
    for (int c = 0; c < 8; c++)
        grid[1][c] = new Pawn(true);

    grid[7][0] = new Rook(false);
    grid[7][1] = new Knight(false);
    grid[7][2] = new Bishop(false);
    grid[7][3] = new Queen(false);
    grid[7][4] = new King(false);
    grid[7][5] = new Bishop(false);
    grid[7][6] = new Knight(false);
    grid[7][7] = new Rook(false);
    for (int c = 0; c < 8; c++)
        grid[6][c] = new Pawn(false);
}

Piece *Board::getPiece(int row, int col)
{
    return grid[row][col];
}

void Board::setPiece(int row, int col, Piece *piece)
{
    grid[row][col] = piece;
}

vector<vector<Piece *>> &Board::getGrid()
{
    return grid;
}
//==================================================

//==================================================
// Display
class Display {
public:
    void printBoard(Board& board) {
        cout << "\n    a   b   c   d   e   f   g   h\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";

        for (int r = 7; r >= 0; r--) {
            cout << r + 1 << " |";

            for (int c = 0; c < 8; c++) {
                Piece* p = board.getPiece(r, c);

                if (p == NULL)
                    cout << " . |";
                else
                    cout << " " << p->getSymbol() << " |";
            }

            cout << " " << r + 1
                 << "\n  +---+---+---+---+---+---+---+---+\n";
        }

        cout << "    a   b   c   d   e   f   g   h\n\n";
    }

    void printMessage(string msg) {
        cout << msg << "\n";
    }

    void printTurn(bool white) {
        if (white)
            cout << "White> ";
        else
            cout << "Black> ";
    }

    bool readMove(string& from, string& to) {
        cin >> from;

        if (from == "quit")
            return false;

        cin >> to;

        for (int i = 0; i < from.size(); i++)
            from[i] = tolower(from[i]);

        for (int i = 0; i < to.size(); i++)
            to[i] = tolower(to[i]);

        return true;
    }

    char readPromotion() {
        cout << "Promote (Q/R/B/N): ";

        char ch;
        cin >> ch;

        return toupper(ch);
    }
};
//==================================================

//==================================================
// Game
class Game {
    Board board;
    Display display;
    bool whiteTurn;

    bool inCheck(bool white);
    bool isLegalMove(int fr, int fc, int tr, int tc);
    void doMove(int fr, int fc, int tr, int tc);
    bool hasMove(bool white);

public:
    Game();
    void run();
};

bool attacked(Board& board, int row, int col, bool byWhite) {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (board.getPiece(r, c) && board.getPiece(r, c)->isWhite() == byWhite)
                if (board.getPiece(r, c)->isValidMove(r, c, row, col, board.getGrid()))
                    return true;
    return false;
}

Game::Game() { whiteTurn = true; }

bool Game::inCheck(bool white) {
    char ch = white ? 'K' : 'k';
    int kr = -1, kc = 0;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (board.getPiece(r, c) && board.getPiece(r, c)->getSymbol() == ch) {
                kr = r; kc = c;
            }
    if (kr == -1) return true;

    if (white) return attacked(board, kr, kc, false);
    return attacked(board, kr, kc, true);
}

bool Game::isLegalMove(int fr, int fc, int tr, int tc) {
    if (fr < 0 || fr > 7 || fc < 0 || fc > 7 || tr < 0 || tr > 7 || tc < 0 || tc > 7)
        return false;

    Piece* p = board.getPiece(fr, fc);
    if (p == NULL || p->isWhite() != whiteTurn)
        return false;

    char sym = p->getSymbol();

    // قلعه
    if ((sym == 'K' || sym == 'k') && fr == tr && abs(tc - fc) == 2) {
        int row = whiteTurn ? 0 : 7;
        bool shortSide = tc > fc;
        if (whiteTurn && board.whiteKingMoved) return false;
        if (!whiteTurn && board.blackKingMoved) return false;
        if (shortSide) {
            if (whiteTurn && board.whiteRookRightMoved) return false;
            if (!whiteTurn && board.blackRookRightMoved) return false;
            if (board.getPiece(row, 5) || board.getPiece(row, 6)) return false;
            if (whiteTurn && attacked(board, row, 5, false)) return false;
            if (whiteTurn && attacked(board, row, 6, false)) return false;
            if (!whiteTurn && attacked(board, row, 5, true)) return false;
            if (!whiteTurn && attacked(board, row, 6, true)) return false;
        } else {
            if (whiteTurn && board.whiteRookLeftMoved) return false;
            if (!whiteTurn && board.blackRookLeftMoved) return false;
            if (board.getPiece(row, 1) || board.getPiece(row, 2) || board.getPiece(row, 3)) return false;
            if (whiteTurn && attacked(board, row, 2, false)) return false;
            if (whiteTurn && attacked(board, row, 3, false)) return false;
            if (!whiteTurn && attacked(board, row, 2, true)) return false;
            if (!whiteTurn && attacked(board, row, 3, true)) return false;
        }
        if (inCheck(whiteTurn)) return false;
        return true;
    }

    // en passant
    if ((sym == 'P' || sym == 'p') && abs(fc - tc) == 1
        && tr == fr + (whiteTurn ? 1 : -1)
        && tr == board.enPassantRow && tc == board.enPassantCol) {
        Piece* cap = board.getPiece(fr, tc);
        board.setPiece(tr, tc, p);
        board.setPiece(fr, fc, NULL);
        board.setPiece(fr, tc, NULL);
        bool bad = inCheck(whiteTurn);
        board.setPiece(fr, fc, p);
        board.setPiece(tr, tc, NULL);
        board.setPiece(fr, tc, cap);
        if (bad) return false;
        return true;
    }

    if (!p->isValidMove(fr, fc, tr, tc, board.getGrid()))
        return false;

    Piece* cap = board.getPiece(tr, tc);
    board.setPiece(tr, tc, p);
    board.setPiece(fr, fc, NULL);
    bool bad = inCheck(whiteTurn);
    board.setPiece(fr, fc, p);
    board.setPiece(tr, tc, cap);
    if (bad) return false;

    return true;
}

void Game::doMove(int fr, int fc, int tr, int tc) {
    Piece* p = board.getPiece(fr, fc);
    char sym = p->getSymbol();

    if ((sym == 'K' || sym == 'k') && fr == tr && abs(tc - fc) == 2) {
        int row = p->isWhite() ? 0 : 7;
        board.enPassantCol = -1;
        board.enPassantRow = -1;
        if (tc > fc) {
            board.setPiece(row, 6, board.getPiece(row, 4));
            board.setPiece(row, 5, board.getPiece(row, 7));
            board.setPiece(row, 4, NULL);
            board.setPiece(row, 7, NULL);
            if (row == 0) { board.whiteKingMoved = true; board.whiteRookRightMoved = true; }
            else { board.blackKingMoved = true; board.blackRookRightMoved = true; }
        } else {
            board.setPiece(row, 2, board.getPiece(row, 4));
            board.setPiece(row, 3, board.getPiece(row, 0));
            board.setPiece(row, 4, NULL);
            board.setPiece(row, 0, NULL);
            if (row == 0) { board.whiteKingMoved = true; board.whiteRookLeftMoved = true; }
            else { board.blackKingMoved = true; board.blackRookLeftMoved = true; }
        }
        return;
    }

    int oldEpC = board.enPassantCol;
    int oldEpR = board.enPassantRow;
    board.enPassantCol = -1;
    board.enPassantRow = -1;

    if ((sym == 'P' || sym == 'p') && abs(tr - fr) == 2) {
        board.enPassantCol = fc;
        board.enPassantRow = (fr + tr) / 2;
    }

    if ((sym == 'P' || sym == 'p') && abs(fc - tc) == 1 && tr == oldEpR && tc == oldEpC) {
        delete board.getPiece(fr, tc);
        board.setPiece(fr, tc, NULL);
    }

    if (board.getPiece(tr, tc) != NULL)
        delete board.getPiece(tr, tc);

    board.setPiece(tr, tc, p);
    board.setPiece(fr, fc, NULL);

    if (sym == 'K') board.whiteKingMoved = true;
    if (sym == 'k') board.blackKingMoved = true;
    if (sym == 'R' && fc == 0) board.whiteRookLeftMoved = true;
    if (sym == 'R' && fc == 7) board.whiteRookRightMoved = true;
    if (sym == 'r' && fc == 0) board.blackRookLeftMoved = true;
    if (sym == 'r' && fc == 7) board.blackRookRightMoved = true;

    if (sym == 'P' && tr == 7) {
        char ch = display.readPromotion();
        delete board.getPiece(tr, tc);
        if (ch == 'R') board.setPiece(tr, tc, new Rook(true));
        else if (ch == 'B') board.setPiece(tr, tc, new Bishop(true));
        else if (ch == 'N') board.setPiece(tr, tc, new Knight(true));
        else board.setPiece(tr, tc, new Queen(true));
    }
    if (sym == 'p' && tr == 0) {
        char ch = display.readPromotion();
        delete board.getPiece(tr, tc);
        if (ch == 'R') board.setPiece(tr, tc, new Rook(false));
        else if (ch == 'B') board.setPiece(tr, tc, new Bishop(false));
        else if (ch == 'N') board.setPiece(tr, tc, new Knight(false));
        else board.setPiece(tr, tc, new Queen(false));
    }
}

bool Game::hasMove(bool white) {
    bool save = whiteTurn;
    whiteTurn = white;
    for (int r1 = 0; r1 < 8; r1++)
        for (int c1 = 0; c1 < 8; c1++)
            if (board.getPiece(r1, c1) && board.getPiece(r1, c1)->isWhite() == white)
                for (int r2 = 0; r2 < 8; r2++)
                    for (int c2 = 0; c2 < 8; c2++)
                        if (isLegalMove(r1, c1, r2, c2)) {
                            whiteTurn = save;
                            return true;
                        }
    whiteTurn = save;
    return false;
}

void Game::run() {
    display.printMessage("Chess | move: e2 e4 | castle: e1 g1 | quit");
    string from, to;
    while (true) {
        display.printBoard(board);
        if (inCheck(whiteTurn))
            display.printMessage("Check!");
        if (!hasMove(whiteTurn)) {
            if (inCheck(whiteTurn)) {
                if (whiteTurn) display.printMessage("Checkmate! Black wins.");
                else display.printMessage("Checkmate! White wins.");
            } else {
                display.printMessage("Stalemate!");
            }
            break;
        }
        display.printTurn(whiteTurn);
        if (!display.readMove(from, to))
            break;
        if (from.size() != 2 || to.size() != 2) {
            display.printMessage("Example: e2 e4");
            continue;
        }
        int fc = tolower(from[0]) - 'a';
        int fr = from[1] - '1';
        int tc = tolower(to[0]) - 'a';
        int tr = to[1] - '1';
        if (isLegalMove(fr, fc, tr, tc)) {
            doMove(fr, fc, tr, tc);
            whiteTurn = !whiteTurn;
        } else {
            display.printMessage("Illegal move.");
        }
    }
}

//==================================================

//==================================================
// main()
int main() {
    Game game;
    game.run();
    return 0;
}
//==================================================

