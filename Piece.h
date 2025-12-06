#pragma once
#include <string>

class Piece
{
protected:
    bool isWhite;
    std::string pieceType;
public:
    Piece(const std::string& type, bool white) : pieceType(type), isWhite(white) {}
    virtual ~Piece() {}
    bool getColor() const { return isWhite; }
    const std::string& getType() const { return pieceType; }

    // board is an 8x8 array of Piece* (nullptr for empty)
    virtual bool canMove(int sr, int sc, int er, int ec, Piece* board[8][8]) const = 0;
    virtual std::string getSymbol() const = 0;
};
