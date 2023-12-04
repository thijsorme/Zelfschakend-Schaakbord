#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <headers/game.h>


enum class PiecesType {
    None,
    Pawn, 
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
};

enum class Player {
    None,
    White,
    Black,
};

class Move {
    public:
    std::string boardCopy;
    int pos1;
    int pos2;
    Move(int p1, int p2, std::string bc);
};

class Piece {
    public:
    Piece(PiecesType t, Player c);
    Piece();
    ~Piece();

    PiecesType type;
    Player color;
    bool equals(PiecesType pt, Player pc);
};

class PieceSprite {
    public:
    PieceSprite(SDL_Texture *tex, int s,PiecesType p, Player c);
    PieceSprite();
    void reconstructor(SDL_Texture *tex, int s,PiecesType p, Player c);
    Sprite sprite;
    PiecesType piece;
    Player color;
};

class Board {
    public:
    Board();
    ~Board();

    bool playLegal;

    std::string boardStr;
    Player currentTurn;
    bool castleQW;
    bool castleKW;
    bool castleQB;
    bool castleKB;
    int enPassantLocation;
    // int halfMoves;
    // int fullMoves;
    void setBoard(std::string f = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w kqKQ - 0 1");
    
    Piece pieceTypeOf(char p);
    Piece pieceTypeAt(int location);
    char pieceCharOf(PiecesType pt, Player pc);

    std::vector<Move> makeMove(int loc1, int loc2);
    void flipCurrentTurn();

    std::vector<int> getMoves(int location, bool legalCheck = false);
    std::vector<int> getMoves(Piece piece, int location, bool legalCheck = false);
};
