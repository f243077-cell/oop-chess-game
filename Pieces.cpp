#include "Piece.h"
#include <cmath>

// small integer absolute helper
static int absVal(int v) { return v < 0 ? -v : v; }

/* ---------- King ---------- */
class King : public Piece
{
public:
    King(bool white) : Piece("King", white) {}
    std::string getSymbol() const override { return isWhite ? "♔" : "♚"; }
    bool canMove(int sr, int sc, int er, int ec, Piece* b[8][8]) const override
    {
        return absVal(er - sr) <= 1 && absVal(ec - sc) <= 1
               && !(er == sr && ec == sc);
    }
};

/* ---------- Queen ---------- */
class Queen : public Piece
{
public:
    Queen(bool white) : Piece("Queen", white) {}
    std::string getSymbol() const override { return isWhite ? "♕" : "♛"; }
    bool canMove(int sr, int sc, int er, int ec, Piece* b[8][8]) const override
    {
        int dr = er - sr, dc = ec - sc;
        if (dr == 0 && dc == 0) return false;
        if (dr != 0 && dc != 0 && absVal(dr) != absVal(dc)) return false;
        int rStep = (dr == 0 ? 0 : (dr > 0 ? 1 : -1));
        int cStep = (dc == 0 ? 0 : (dc > 0 ? 1 : -1));
        for (int r = sr + rStep, c = sc + cStep; r != er || c != ec; r += rStep, c += cStep)
            if (b[r][c]) return false;
        return true;
    }
};

/* ---------- Rook ---------- */
class Rook : public Piece
{
public:
    Rook(bool white) : Piece("Rook", white) {}
    std::string getSymbol() const override { return isWhite ? "♖" : "♜"; }
    bool canMove(int sr, int sc, int er, int ec, Piece* b[8][8]) const override
    {
        if (sr != er && sc != ec) return false;
        int rStep = (er > sr ? 1 : (er < sr ? -1 : 0));
        int cStep = (ec > sc ? 1 : (ec < sc ? -1 : 0));
        for (int r = sr + rStep, c = sc + cStep; r != er || c != ec; r += rStep, c += cStep)
            if (b[r][c]) return false;
        return !(sr == er && sc == ec);
    }
};

/* ---------- Bishop ---------- */
class Bishop : public Piece
{
public:
    Bishop(bool white) : Piece("Bishop", white) {}
    std::string getSymbol() const override { return isWhite ? "♗" : "♝"; }
    bool canMove(int sr, int sc, int er, int ec, Piece* b[8][8]) const override
    {
        if (absVal(er - sr) != absVal(ec - sc)) return false;
        int rStep = (er > sr ? 1 : -1), cStep = (ec > sc ? 1 : -1);
        for (int r = sr + rStep, c = sc + cStep; r != er || c != ec; r += rStep, c += cStep)
            if (b[r][c]) return false;
        return true;
    }
};

/* ---------- Knight ---------- */
class Knight : public Piece
{
public:
    Knight(bool white) : Piece("Knight", white) {}
    std::string getSymbol() const override { return isWhite ? "♘" : "♞"; }
    bool canMove(int sr, int sc, int er, int ec, Piece* /*b*/[8][8]) const override
    {
        int dr = absVal(er - sr), dc = absVal(ec - sc);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
};

/* ---------- Pawn ---------- */
class Pawn : public Piece
{
public:
    Pawn(bool white) : Piece("Pawn", white) {}
    std::string getSymbol() const override { return isWhite ? "♙" : "♟"; }
    bool canMove(int sr, int sc, int er, int ec, Piece* b[8][8]) const override
    {
        int dir = isWhite ? 1 : -1; // white moves to increasing row
        if (sc == ec && er - sr == dir && !b[er][ec]) return true;
        int startRow = isWhite ? 1 : 6;
        if (sc == ec && sr == startRow && er - sr == 2 * dir &&
            !b[sr + dir][sc] && !b[er][ec]) return true;
        if (absVal(ec - sc) == 1 && er - sr == dir &&
            b[er][ec] && b[er][ec]->getColor() != isWhite) return true;
        return false;
    }
};

/* Factory helpers so main/Board can create pieces without exposing class names */
extern "C" Piece* createKing(bool white)   { return new King(white); }
extern "C" Piece* createQueen(bool white)  { return new Queen(white); }
extern "C" Piece* createRook(bool white)   { return new Rook(white); }
extern "C" Piece* createBishop(bool white) { return new Bishop(white); }
extern "C" Piece* createKnight(bool white) { return new Knight(white); }
extern "C" Piece* createPawn(bool white)   { return new Pawn(white); }
