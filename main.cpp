#include <iostream>
#include <string>
#include <cctype>
#include <windows.h>
#include "Piece.h"

// declare factories implemented in Pieces.cpp
extern Piece* createKing(bool);
extern Piece* createQueen(bool);
extern Piece* createRook(bool);
extern Piece* createBishop(bool);
extern Piece* createKnight(bool);
extern Piece* createPawn(bool);

/* ---------- Console colour helpers ---------- */
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
WORD   origAttr = 0;
void initColor()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsole, &info);
    origAttr = info.wAttributes;
}
void setColor(WORD c) { SetConsoleTextAttribute(hConsole, c); }
void resetColor()     { SetConsoleTextAttribute(hConsole, origAttr); }

/* ---------- Board ---------- */
class Board
{
    Piece* squares[8][8];
public:
    Board()
    {
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                squares[r][c] = nullptr;
    }
    ~Board()
    {
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                delete squares[r][c];
    }
    Piece* getPieceAt(int r, int c) const { return squares[r][c]; }
    void   setPieceAt(int r, int c, Piece* p) { squares[r][c] = p; }
    void   movePiece(int sr, int sc, int er, int ec)
    {
        delete squares[er][ec];
        squares[er][ec] = squares[sr][sc];
        squares[sr][sc] = nullptr;
    }

    // Return pointer-to-array so pieces can inspect board (type: Piece* (*)[8])
    Piece* (*getBoardArray())[8] { return squares; }

    void setupInitial();
    void display() const;
    bool findKing(bool white, int& r, int& c) const;
    bool isSquareAttacked(int tr, int tc, bool attackerWhite) const;
    bool inCheck(bool white) const;
    bool moveCausesCheck(int sr, int sc, int er, int ec, bool white) const;
    bool hasLegalMove(bool white) const;
    bool isCheckmate(bool white) const { return inCheck(white) && !hasLegalMove(white); }
};

/* ---------- Board implementation ---------- */
void Board::setupInitial()
{
    // White back rank (row 0) and pawns (row 1)
    squares[0][0] = createRook(true);
    squares[0][1] = createKnight(true);
    squares[0][2] = createBishop(true);
    squares[0][3] = createQueen(true);
    squares[0][4] = createKing(true);
    squares[0][5] = createBishop(true);
    squares[0][6] = createKnight(true);
    squares[0][7] = createRook(true);
    for (int c = 0; c < 8; ++c) squares[1][c] = createPawn(true);

    // Empty middle
    for (int r = 2; r <= 5; ++r)
        for (int c = 0; c < 8; ++c)
            squares[r][c] = nullptr;

    // Black back rank (row 7) and pawns (row 6)
    squares[7][0] = createRook(false);
    squares[7][1] = createKnight(false);
    squares[7][2] = createBishop(false);
    squares[7][3] = createQueen(false);
    squares[7][4] = createKing(false);
    squares[7][5] = createBishop(false);
    squares[7][6] = createKnight(false);
    squares[7][7] = createRook(false);
    for (int c = 0; c < 8; ++c) squares[6][c] = createPawn(false);
}

void Board::display() const
{
    setColor(FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN);
    std::cout << "\n   a  b  c  d  e  f  g  h\n";
    for (int r = 7; r >= 0; --r)
    {
        std::cout << r + 1 << "|";
        for (int c = 0; c < 8; ++c)
        {
            Piece* p = squares[r][c];
            if (p)
            {
                if (p->getColor())
                    setColor(FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN);
                else
                    setColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
                std::cout << ' ' << p->getSymbol() << ' ';
            }
            else
            {
                setColor(FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN);
                std::cout << " . ";
            }
        }
        resetColor();
        std::cout << "|" << r + 1 << '\n';
    }
    setColor(FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN);
    std::cout << "   a  b  c  d  e  f  g  h\n\n";
    resetColor();
}

bool Board::findKing(bool white, int& r, int& c) const
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (squares[i][j] && squares[i][j]->getColor() == white && squares[i][j]->getType() == "King")
            {
                r = i; c = j; return true;
            }
    return false;
}

bool Board::isSquareAttacked(int tr, int tc, bool attackerWhite) const
{
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
        {
            Piece* p = squares[r][c];
            if (p && p->getColor() == attackerWhite)
            {
                // canMove expects Piece* board[8][8]
                if (p->canMove(r, c, tr, tc, const_cast<Piece* (*)[8]>(squares)))
                {
                    // If target square has own piece, capturing not allowed — but attacking is still valid
                    return true;
                }
            }
        }
    return false;
}

bool Board::inCheck(bool white) const
{
    int kr, kc;
    if (!findKing(white, kr, kc)) return false;
    return isSquareAttacked(kr, kc, !white);
}

bool Board::moveCausesCheck(int sr, int sc, int er, int ec, bool white) const
{
    Piece* moving = squares[sr][sc];
    Piece* victim = squares[er][ec];

    // perform move temporarily
    const_cast<Board*>(this)->squares[er][ec] = moving;
    const_cast<Board*>(this)->squares[sr][sc] = nullptr;

    bool chk = inCheck(white);

    // undo
    const_cast<Board*>(this)->squares[sr][sc] = moving;
    const_cast<Board*>(this)->squares[er][ec] = victim;
    return chk;
}

bool Board::hasLegalMove(bool white) const
{
    for (int sr = 0; sr < 8; ++sr)
        for (int sc = 0; sc < 8; ++sc)
        {
            Piece* p = squares[sr][sc];
            if (!p || p->getColor() != white) continue;
            for (int er = 0; er < 8; ++er)
                for (int ec = 0; ec < 8; ++ec)
                {
                    if (sr == er && sc == ec) continue;
                    if (squares[er][ec] && squares[er][ec]->getColor() == white) continue;
                    if (p->canMove(sr, sc, er, ec, const_cast<Piece* (*)[8]>(squares)) &&
                        !moveCausesCheck(sr, sc, er, ec, white))
                        return true;
                }
        }
    return false;
}

/* ---------- small helpers ---------- */
bool parseSquare(const std::string& in, int& r, int& c)
{
    if (in.size() != 2) return false;
    char f = std::tolower(in[0]), rk = in[1];
    if (f < 'a' || f > 'h' || rk < '1' || rk > '8') return false;
    c = f - 'a';  r = rk - '1';
    return true;
}

/* ---------- driver ---------- */
int main()
{
    // ensure unicode output for piece symbols
    SetConsoleOutputCP(CP_UTF8);
    initColor();
    Board bd;
    bd.setupInitial();
    bool whiteTurn = true;
    std::string from, to;
    std::cout << "=====================================================\n";
    std::cout << "------------------- CHESS GAME ----------------------\n";
    std::cout << "=====================================================\n";
    std::cout << "Moves like e2 e4, Ctrl+C to quit.\n";
    std::cout << "-----------------------------------------------------\n";

    while (true)
    {
        bd.display();
        if (bd.inCheck(whiteTurn))
            std::cout << (whiteTurn ? "White" : "Black") << " is in CHECK!\n";
        std::cout << (whiteTurn ? "White" : "Black") << " move: ";
        if (!(std::cin >> from >> to)) break;

        int sr, sc, er, ec;
        if (!parseSquare(from, sr, sc) || !parseSquare(to, er, ec))
        {
            std::cout << "Invalid input\n"; continue;
        }
        Piece* p = bd.getPieceAt(sr, sc);
        if (!p) { std::cout << "No piece at source.\n"; continue; }
        if (p->getColor() != whiteTurn) { std::cout << "That's not your piece.\n"; continue; }
        if (bd.getPieceAt(er, ec) && bd.getPieceAt(er, ec)->getColor() == whiteTurn)
        { std::cout << "Destination occupied.\n"; continue; }
        if (!p->canMove(sr, sc, er, ec, bd.getBoardArray()))
        { std::cout << "Illegal move.\n"; continue; }
        if (bd.moveCausesCheck(sr, sc, er, ec, whiteTurn))
        { std::cout << "Move leaves king in check.\n"; continue; }

        bd.movePiece(sr, sc, er, ec);

        bool opp = !whiteTurn;
        if (bd.isCheckmate(opp))
        {
            bd.display();
            std::cout << (whiteTurn ? "White" : "Black") << " WINS! CHECKMATE\n";
            break;
        }
        if (!bd.inCheck(opp) && !bd.hasLegalMove(opp))
        {
            bd.display();
            std::cout << "STALEMATE! DRAW\n";
            break;
        }
        whiteTurn = !whiteTurn;
    }
    std::cout << "Game ended.\n";
    return 0;
}
