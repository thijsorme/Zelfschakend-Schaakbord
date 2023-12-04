#pragma once
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>

// Een tile van het 17x17 grid waar het schaakbord uit bestaat
class Node {
public:
    // de vorige node in de reeks.
    Node* parent;

    int x;
    int y;

    // scores van volgens A*
    int g = 0;
    int h = 0;
    int f = 0;

    Node(Node* parent, int xPos, int yPos);

    bool equal(Node other);
};

class Coord {
public:
    int x;
    int y;
    Coord(int x, int y);
};

class AStar {
    public:

    // is coord x,y bezet? (0-16)
    static bool pieceOnCoord(std::string bits, int x, int y);

    /// @brief A* algoritme voor over het bord
    /// @param bits bitset die weergeeft waar wel of niet een stuk staat 
    /// @param x1 start x (0-16)
    /// @param y1 start y (0-16)
    /// @param x2 eind x (0-16)
    /// @param y2 eind y (0-16)
    /// @param compact verwijder onnodige info (1,1; 2,2; 3,3; -> 1,1; 3,3;)
    /// @return lijst met coordinaten voor snelste kloppende route
    static std::vector<Coord> aStar(std::string bits, int x1, int y1, int x2, int y2, bool compact = true);
};