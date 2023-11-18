#include <iostream>
#include <headers/game.h>
#include <headers/chess.h>
#include <headers/astar.h>
#include "headers/SimpleSerial.h"


// Settings
#define CONNECTARDUINO true
bool playLegal = true;


#if CONNECTARDUINO
char com_port[] = "\\\\.\\COM5";
DWORD COM_BAUD_RATE = CBR_9600;
SimpleSerial Serial;
#endif
bool arduinoConnected;

//
bool clearScreen = true;
int indexSelected;
int previousIndexSelected = -1;
const int tile = 125;

Board board;

Sprite selectCircle;
PieceSprite spriteBishopW, spriteKingW, spriteKnightW, spritePawnW, spriteQueenW, spriteRookW, spriteBishopB, spriteKingB, spriteKnightB, spritePawnB, spriteQueenB, spriteRookB;
PieceSprite* allPieces[12] = {&spriteBishopW, &spriteKingW, &spriteKnightW, &spritePawnW, &spriteQueenW, &spriteRookW, &spriteBishopB, &spriteKingB, &spriteKnightB, &spritePawnB, &spriteQueenB, &spriteRookB};

std::vector<int> posMovesIndexes;

void draw(Game *game){
    (*game).renderFill(255,255,255);
    // drawing tiles
    for (int y = 0; y < 8; y += 2)
    {
        for (int x = 0; x < 8; x += 1)
        {
            (*game).renderRect((y + x%2) * tile, x * tile, tile, tile, 80, 170, 90);
        }
    }

    // drawing pieces
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Piece piece = board.pieceTypeAt(y*8+x);

            for (int i = 0; i < 12; i++)
            {
                if(piece.type == allPieces[i]->piece && piece.color == allPieces[i]->color) {
                    (*game).renderSprite(x*tile, y * tile, allPieces[i]->sprite);
                    break;
                }
            }
        }
    }

    // moveable
    for (int i = 0; i < posMovesIndexes.size(); i++)
    {
        int y = posMovesIndexes[i]/8;
        int x = posMovesIndexes[i]%8;
        (*game).renderSprite(x*tile, y * tile, selectCircle);
    }
    
}

void start(Game *game){
    board.setBoard("r2qkb1r/pp2nppp/3p4/2pNN1B1/2BnP3/3P4/PPP2PPP/R2bK2R w KQkq - 1 0");
    board.playLegal = playLegal;
    (*game).setWindowTitle("PWS CHESS");
    

    spriteBishopW.reconstructor((*game).textureFromPath("res/piecespng/BW.png"), tile, PiecesType::Bishop, Player::White);
    spriteKingW.reconstructor((*game).textureFromPath("res/piecespng/KW.png"), tile, PiecesType::King, Player::White);
    spriteKnightW.reconstructor((*game).textureFromPath("res/piecespng/NW.png"), tile, PiecesType::Knight, Player::White);
    spritePawnW.reconstructor((*game).textureFromPath("res/piecespng/PW.png"), tile, PiecesType::Pawn, Player::White);
    spriteQueenW.reconstructor((*game).textureFromPath("res/piecespng/QW.png"), tile, PiecesType::Queen, Player::White);
    spriteRookW.reconstructor((*game).textureFromPath("res/piecespng/RW.png"), tile, PiecesType::Rook, Player::White);
    spriteBishopB.reconstructor((*game).textureFromPath("res/piecespng/BB.png"), tile, PiecesType::Bishop, Player::Black);
    spriteKingB.reconstructor((*game).textureFromPath("res/piecespng/KB.png"), tile, PiecesType::King, Player::Black);
    spriteKnightB.reconstructor((*game).textureFromPath("res/piecespng/NB.png"), tile, PiecesType::Knight, Player::Black);
    spritePawnB.reconstructor((*game).textureFromPath("res/piecespng/PB.png"), tile, PiecesType::Pawn, Player::Black);
    spriteQueenB.reconstructor((*game).textureFromPath("res/piecespng/QB.png"), tile, PiecesType::Queen, Player::Black);
    spriteRookB.reconstructor((*game).textureFromPath("res/piecespng/RB.png"), tile, PiecesType::Rook, Player::Black);
    selectCircle.reconstructor((*game).textureFromPath("res/circle.png"), tile);
   
    #if CONNECTARDUINO
    Serial.init(com_port, COM_BAUD_RATE);
    if (Serial.connected_){
        std::cout << "Connected to Arduino!" << std::endl;
    }
    arduinoConnected = Serial.connected_;
    #endif
}
void intToSend(int num){
    char chr = char(num + ' ');
    bool is_sentX = Serial.WriteSerialPort(&chr);
    if (!is_sentX) {
        std::cout << "not send" << std::endl;
    }
}
// TODO: send to arduino
void sendMoveToArduino(std::vector<Coord> path){
    std::cout << "start sending " << std::endl;

    // first move
    intToSend(path[0].x);
    intToSend(path[0].y);
    // magnet move
    intToSend(17);
    intToSend(1);
    std::cout << "magnet sent " << std::endl;

    // rest of the moves
    for (int i = 1; i < path.size(); i++)
    {
        intToSend(path[i].x);
        intToSend(path[i].y);
    }

    // magnet off
    intToSend(17);
    intToSend(2);

}

void handleMove(){
    std::string boardCopy = board.boardStr;
    std::vector<Move> piecesToMove = board.makeMove(previousIndexSelected,indexSelected);
    
    if(!arduinoConnected) return;
    if(piecesToMove.size() > 0){
        for (int i = 0; i < piecesToMove.size(); i++)
        {
            // std::cout << piecesToMove[i].pos1 << "  " << piecesToMove[i].pos2 << std::endl;
            int startX = piecesToMove[i].pos1%8*2+1;
            int startY = piecesToMove[i].pos1/8*2+1;
            int endX = piecesToMove[i].pos2%8*2+1;
            int endY = piecesToMove[i].pos2/8*2+1;
            
            if(piecesToMove[i].pos2 == -1) {
                endX = startX <= 8 ? 0 : 16;
            // TODO: Y rechte lijn als kan
                endY = startY <= 8 ? startY+1 : startY-1;
            }
            // std::cout << startX << " " << startY << " " << endX << " " << endY << std::endl;
            // std::cout << boardCopy << std::endl;
            // std::cout << boardCopy[(4*(endY-1)+(endX-1)/2)] << std::endl;
            
            std::vector<Coord> path = AStar::aStar(boardCopy, startX, startY, endX, endY);
            boardCopy = piecesToMove[i].boardCopy;
            // for (int j = 0; j < path.size(); j++)
            // {
            //     std::cout << path[j].x << ", " << path[j].y << " -> ";
            // }
            // std::cout << std::endl;

            sendMoveToArduino(path);
        }       
    }
}

void handleBoardInteraction(Game *game){
    if(0 > indexSelected || indexSelected > 63) return;
    if(indexSelected == previousIndexSelected) {
        previousIndexSelected = indexSelected = -1;
        posMovesIndexes.clear();
    } else if(0 > previousIndexSelected) {
        posMovesIndexes = board.getMoves(indexSelected,playLegal);
        if(posMovesIndexes.size() == 0) return;
        for (int i = 0; i < posMovesIndexes.size(); i++)
        {
            posMovesIndexes[i] += indexSelected;
        }
        previousIndexSelected = indexSelected;
    } else {
        handleMove();
        (*game).setWindowTitle(board.currentTurn == Player::White ? "White's turn" : "Black's turn");
        previousIndexSelected = indexSelected = -1;
        posMovesIndexes.clear();
    }
}

void insertPieceAtCurrent(char piece){   
    board.boardStr[indexSelected] = piece;
    posMovesIndexes.clear();
    previousIndexSelected = indexSelected = -1;
    std::cout << indexSelected <<  std::endl;
    std::cout << board.boardStr <<  std::endl;
}
void updateIndexSelected(Game *game){
    int x = (*game).input.mouseX;
    int y = (*game).input.mouseY;
    x /= (*game).getScreenWidth()/8;
    y /= (*game).getScreenHeight()/8;
    
    indexSelected = 8*y + x;
}

void update(Game *game) {
    if((*game).input.LeftMBPressed){
        updateIndexSelected(game);
        handleBoardInteraction(game);
        
        clearScreen = true;
    }
    if((*game).input.KeypPressed){
        updateIndexSelected(game);
        if((*game).input.shiftDown){
            insertPieceAtCurrent('P');
        } else {
            insertPieceAtCurrent('p');
        }
        
        clearScreen = true;
    }
    if((*game).input.KeyrPressed){
        updateIndexSelected(game);
        if((*game).input.shiftDown){
            insertPieceAtCurrent('R');
        } else {
            insertPieceAtCurrent('r');
        }
        clearScreen = true;
    }
    if((*game).input.KeynPressed){
        updateIndexSelected(game);
        if((*game).input.shiftDown){
            insertPieceAtCurrent('N');
        } else {
            insertPieceAtCurrent('n');
        }
        
        clearScreen = true;
    }
    if((*game).input.KeybPressed){
        updateIndexSelected(game);
        if((*game).input.shiftDown){
            insertPieceAtCurrent('B');
        } else {
            insertPieceAtCurrent('b');
        }
        
        clearScreen = true;
    }
    if((*game).input.KeykPressed){
        updateIndexSelected(game);
        if((*game).input.shiftDown){
            insertPieceAtCurrent('K');
        } else {
            insertPieceAtCurrent('k');
        }
        
        clearScreen = true;
    }
    if((*game).input.KeyqPressed){
        updateIndexSelected(game);
        if((*game).input.shiftDown){
            insertPieceAtCurrent('Q');
        } else {
            insertPieceAtCurrent('q');
        }
        
        clearScreen = true;
    }
    if((*game).input.KeybsPressed){
        updateIndexSelected(game);
        insertPieceAtCurrent('0');
        clearScreen = true;
    }

    if(arduinoConnected){
        std::string incoming = Serial.ReadSerialPort(0, "json");
        std::cout << incoming;
    }

    if(clearScreen){
        clearScreen = false;
        draw(game);
    }
}

int main(int argc, char* argv[]){
    Game game;
    game.run(&start,&update);
    return 0;
}