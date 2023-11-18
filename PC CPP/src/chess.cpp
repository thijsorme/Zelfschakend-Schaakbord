#include <headers/chess.h>
#include <sstream>
#include <iostream>

Move::Move(int p1, int p2, std::string bc){
    pos1 = p1;
    pos2 = p2;
    boardCopy = bc;
}

PieceSprite::PieceSprite(SDL_Texture *tex, int s,PiecesType p, Player c){
    sprite.texture = tex;
    sprite.size = s;
    piece = p;
    color = c;
}

PieceSprite::PieceSprite(){
    piece = PiecesType::None;
    color = Player::None;
}

void PieceSprite::reconstructor(SDL_Texture *tex, int s,PiecesType p, Player c) {
    sprite.texture = tex;
    sprite.size = s;
    piece = p;
    color = c;
}

bool Piece::equals(PiecesType otherT, Player otherC){
    return color == otherC && type == otherT;
}

// BOARD
Board::Board()
{
    setBoard();
}
Board::~Board(){}


Piece Board::pieceTypeOf(char p)
{
    if(p == 'r'){
        return Piece(PiecesType::Rook,Player::Black);
    }
    if(p == 'R'){
        return Piece(PiecesType::Rook,Player::White);
    }
    if(p == 'n'){
        return Piece(PiecesType::Knight,Player::Black);
    }
    if(p == 'N'){
        return Piece(PiecesType::Knight,Player::White);
    }
    if(p == 'b'){
        return Piece(PiecesType::Bishop,Player::Black);
    }
    if(p == 'B'){
        return Piece(PiecesType::Bishop,Player::White);
    }
    if(p == 'q'){
        return Piece(PiecesType::Queen,Player::Black);
    }
    if(p == 'Q'){
        return Piece(PiecesType::Queen,Player::White);
    }
    if(p == 'k'){
        return Piece(PiecesType::King,Player::Black);
    }
    if(p == 'K'){
        return Piece(PiecesType::King,Player::White);
    }
    if(p == 'p'){
        return Piece(PiecesType::Pawn,Player::Black);
    }
    if(p == 'P'){
        return Piece(PiecesType::Pawn,Player::White);
    }

    return Piece();
}

char Board::pieceCharOf(PiecesType pt, Player pc) {
    if(pt == PiecesType::Rook && pc == Player::Black){
        return 'r';
    }
    if(pt == PiecesType::Rook && pc == Player::White){
        return 'R';
    }
    if(pt == PiecesType::Knight && pc == Player::Black){
        return 'n';
    }
    if(pt == PiecesType::Knight && pc == Player::White){
        return 'N';
    }
    if(pt == PiecesType::Bishop && pc == Player::Black){
        return 'b';
    }
    if(pt == PiecesType::Bishop && pc == Player::White){
        return 'B';
    }
    if(pt == PiecesType::Queen && pc == Player::Black){
        return 'q';
    }
    if(pt == PiecesType::Queen && pc == Player::White){
        return 'Q';
    }
    if(pt == PiecesType::King && pc == Player::Black){
        return 'k';
    }
    if(pt == PiecesType::King && pc == Player::White){
        return 'K';
    }
    if(pt == PiecesType::Pawn && pc == Player::Black){
        return 'p';
    }
    if(pt == PiecesType::Pawn && pc == Player::White){
        return 'P';
    }

    throw;

}

Piece Board::pieceTypeAt(int location){
    if(location < 0 || location > 63) return Piece();
    return pieceTypeOf(boardStr[location]);
}

void Board::setBoard(std::string f)
{
    // accepted the fact this is shit
    std::stringstream splitme(f);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(splitme, segment, ' '))
    {
        seglist.push_back(segment);
    }
    if(seglist.size() != 6){
        throw;
    }

    boardStr = seglist[0];
    for (int i = 0; i < 64; i++)
    {   
        if(isdigit(boardStr[i])){
            int num = boardStr[i]-'0';
            boardStr.replace(i,1,std::string(num,'0'));
            i += num-1;
        } else if(boardStr[i] == '/'){
            boardStr.erase(i,1);
            i--;
        }
    }
    
    currentTurn = seglist[1] == "w" ? Player::White : Player::Black;
    castleKB = seglist[2].find('k') >= 0;
    castleQB = seglist[2].find('q') >= 0;
    castleKW = seglist[2].find('K') >= 0;
    castleQW = seglist[2].find('Q') >= 0;
    
    if(seglist[3] == "-") {
        enPassantLocation = -1;
    } else {
        enPassantLocation = seglist[3][1] == '3' ? 48 : 8;
        enPassantLocation += seglist[3][0] - 'a';
    }
    // halfMoves = std::stoi(seglist[4]);
    // fullMoves = std::stoi(seglist[5]);
}



// TODO: split into many returns 
// TODO: castle -> king moves first
std::vector<Move> Board::makeMove(int loc1, int loc2)
{
    Piece movePiece = pieceTypeAt(loc1);
    if(playLegal){
        if(movePiece.color != currentTurn) return std::vector<Move>();
        std::vector<int> legalMove = getMoves(movePiece,loc1);
        if (std::none_of(std::begin(legalMove), std::end(legalMove), [loc1,loc2](int i) { return i+loc1 == loc2; })) return std::vector<Move>();
        // ok to move
    }
    std::vector<Move> shifts;
    flipCurrentTurn();

    // en passant
    if(movePiece.type == PiecesType::Pawn && loc2 == enPassantLocation){
        if(movePiece.color == Player::Black){
            boardStr[loc2-8] = '0';
            shifts.push_back(Move(loc2-8,-1,boardStr));
        } else {
            boardStr[loc2+8] = '0'; 
            shifts.push_back(Move(loc2+8,-1,boardStr));
        }
    }
    enPassantLocation = -1;
    if(movePiece.type == PiecesType::Pawn && abs(loc1-loc2) == 16){
        enPassantLocation = (loc1+loc2)/2;
    }

    // castling
    if(movePiece.type == PiecesType::King && loc2 == 2 && castleQB) {
        boardStr[3] = boardStr[0];
        boardStr[0] = '0';
        shifts.push_back(Move(0,3,boardStr));
    } else if(movePiece.type == PiecesType::King && loc2 == 6 && castleKB) {
        boardStr[5] = boardStr[7];
        boardStr[7] = '0';
        shifts.push_back(Move(7,5,boardStr));
    } else if(movePiece.type == PiecesType::King && loc2 == 58 && castleQW) {
        boardStr[59] = boardStr[56];
        boardStr[56] = '0';
        shifts.push_back(Move(56,59,boardStr));
    } else if(movePiece.type == PiecesType::King && loc2 == 62 && castleQB) {
        boardStr[61] = boardStr[63];
        boardStr[63] = '0';
        shifts.push_back(Move(63,61,boardStr));
    }
    // promote pawns to queens (turn piece before move in queen)
    if(movePiece.type == PiecesType::Pawn && loc2/8 == 0){
        boardStr[loc1] = 'Q';
    } else if(movePiece.type == PiecesType::Pawn && loc2/8 == 7){
        boardStr[loc1] = 'q';
    } 

    // makeMove
    bool willTake = boardStr[loc2] != '0'; 
    boardStr[loc2] = '0';
    // if take
    if(willTake){
        shifts.push_back(Move(loc2,-1,boardStr));
    }
    boardStr[loc2] = boardStr[loc1];
    boardStr[loc1] = '0';
    shifts.push_back(Move(loc1,loc2,boardStr)); // last item boardstr doesnt matter

    // remove castle rights
    if(movePiece.type == PiecesType::King && movePiece.color == Player::Black){
        castleKB = false; 
        castleQB = false;
    } 
    if(movePiece.type == PiecesType::King && movePiece.color == Player::White){
        castleKW = false; 
        castleQW = false;
    }
    if(castleQB && boardStr[0] != 'r'){
        castleQB = false;
    }
    if(castleKB && boardStr[7] != 'r'){
        castleKB = false;
    }
    if(castleQW && boardStr[56] != 'R'){
        castleQW = false;
    }
    if(castleKW && boardStr[63] != 'R'){
        castleKW = false;
    }
    return shifts;
}

void Board::flipCurrentTurn(){
    currentTurn = currentTurn == Player::White ? Player::Black : Player::White;
}

bool checkColorAddMove(Player tileColor, Player pieceColor, std::vector<int>* moves, int addValue){
    if(tileColor == Player::None){
        moves->push_back(addValue);
        return false;
    } 
    if (tileColor != pieceColor){
        moves->push_back(addValue);
    }
    return true;
}

std::vector<int> Board::getMoves(Piece piece, int location, bool legalCheck){
    // hell
    std::vector<int> moves;
    if(legalCheck && piece.color != currentTurn) return moves;
    switch (piece.type)
    {
    case PiecesType::Rook:
        for(int i = 1; location%8+i <= 7; i++) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = -1; location%8+i >= 0; i--) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = 8; location+i <= 63; i+=8) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = -8; location+i >= 0; i-=8) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        return moves;
    case PiecesType::Knight: // +-6 +-10 +-15 +-17
        if(location < 56 && location%8>=2 && piece.color != pieceTypeAt(location+6).color) moves.push_back(6);
        if(location < 56 && location%8<=5 && piece.color != pieceTypeAt(location+10).color) moves.push_back(10);
        if(location < 48 && location%8>=1 && piece.color != pieceTypeAt(location+15).color) moves.push_back(15);
        if(location < 48 && location%8<=6 && piece.color != pieceTypeAt(location+17).color) moves.push_back(17);

        if(location > 7 && location%8<=5 && piece.color != pieceTypeAt(location-6).color) moves.push_back(-6);
        if(location > 7 && location%8>=2 && piece.color != pieceTypeAt(location-10).color) moves.push_back(-10);
        if(location > 15 && location%8<=6 && piece.color != pieceTypeAt(location-15).color) moves.push_back(-15);
        if(location > 15 && location%8>=1 && piece.color != pieceTypeAt(location-17).color) moves.push_back(-17);
        return moves;
    case PiecesType::Bishop:
        for(int i = -7; 0 < location+i && (location+i+7)%8 != 7 ; i-=7) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = -9; 0 < location+i && (location+i+9)%8 != 0 ; i-=9) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = 7; 63 > location+i && (location+i-7)%8 != 0 ; i+=7) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = 9; 63 > location+i && (location+i-9)%8 != 7 ; i+=9) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        return moves;
    case PiecesType::Queen:
        // rook
        for(int i = 1; location%8+i <= 7; i++) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = -1; location%8+i >= 0; i--) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = 8; location+i <= 63; i+=8) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = -8; location+i >= 0; i-=8) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }      
        // bishop   
        for(int i = -7; 0 < location+i && (location+i+7)%8 != 7 ; i-=7) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = -9; 0 < location+i && (location+i+9)%8 != 0 ; i-=9) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = 7; 63 > location+i && (location+i-7)%8 != 0 ; i+=7) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }
        for(int i = 9; 63 > location+i && (location+i-9)%8 != 7 ; i+=9) {
            if(checkColorAddMove(pieceTypeAt(location+i).color,piece.color,&moves,i)) break;
        }

        return moves;

    case PiecesType::King:
        if(location%8<7 && piece.color != pieceTypeAt(location+1).color) moves.push_back(1);
        if(location%8>0 && piece.color != pieceTypeAt(location-1).color) moves.push_back(-1);
        if(location < 56 && piece.color != pieceTypeAt(location+8).color) moves.push_back(8);
        if(location > 7 && piece.color != pieceTypeAt(location-8).color) moves.push_back(-8);
        if(location < 56 && location%8<7 && piece.color != pieceTypeAt(location+9).color) moves.push_back(9);
        if(location < 56 && location%8>0 && piece.color != pieceTypeAt(location+7).color) moves.push_back(7);
        if(location > 7 && location%8>0 && piece.color != pieceTypeAt(location-9).color) moves.push_back(-9);
        if(location > 7 && location%8<7 && piece.color != pieceTypeAt(location-7).color) moves.push_back(-7);

        // TODO: als niet in check op 1 en 2;
        if(piece.color == Player::White){
            if(castleKW && pieceTypeAt(location+1).type == PiecesType::None && pieceTypeAt(location+2).type == PiecesType::None){
                moves.push_back(2);
            }
            if(castleQW && pieceTypeAt(location-1).type == PiecesType::None && pieceTypeAt(location-2).type == PiecesType::None){
                moves.push_back(-2);
            }
        }
        if(piece.color == Player::Black){
            if(castleKB && pieceTypeAt(location+1).type == PiecesType::None && pieceTypeAt(location+2).type == PiecesType::None){
                moves.push_back(2);
            }
            if(castleQB && pieceTypeAt(location-1).type == PiecesType::None && pieceTypeAt(location-2).type == PiecesType::None){
                moves.push_back(-2);
            }
        }
        
        return moves;
    
    case PiecesType::Pawn:
        if(piece.color == Player::White){
            if(pieceTypeAt(location-8).type == PiecesType::None) moves.push_back(-8);
            // if piece at -7,-9 or en peasant tile
            if(location%8<7 && (pieceTypeAt(location-7).color == Player::Black || location-7 == enPassantLocation)) moves.push_back(-7);
            if(location%8>0 && (pieceTypeAt(location-9).color == Player::Black || location-9 == enPassantLocation)) moves.push_back(-9);
            if(location/8 == 6 && pieceTypeAt(location-16).type == PiecesType::None) moves.push_back(-16);
        }
        if(piece.color == Player::Black){
            if(pieceTypeAt(location+8).type == PiecesType::None) moves.push_back(8);
            // if piece at -7,-9 or en peasant tile
            if(location%8>0 && (pieceTypeAt(location+7).color == Player::White || location+7 == enPassantLocation)) moves.push_back(7);
            if(location%8<7 && (pieceTypeAt(location+9).color == Player::White || location+9 == enPassantLocation)) moves.push_back(9);
            if(location/8 == 1 && pieceTypeAt(location+16).type == PiecesType::None) moves.push_back(16);
            
        }

        return moves;
    default:
        break;
    }
    return std::vector<int>();
}

std::vector<int> Board::getMoves(int location, bool legalCheck){
    return getMoves(pieceTypeAt(location),location, legalCheck);
}


// PIECES

Piece::Piece(PiecesType t,Player c)
{
    type = t;
    color = c;
}
Piece::Piece()
{
    type = PiecesType::None;
    color = Player::None;
}

Piece::~Piece() {}