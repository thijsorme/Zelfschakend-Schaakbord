#include <headers/astar.h>


Node::Node(Node* parent, int xPos, int yPos){
    this->parent = parent;
    this->x = xPos;
    this->y = yPos;
}

bool Node::equal(Node other){
    return this->x == other.x && this->y == other.y;
}

Coord::Coord(int x, int y){
    this->x = x;
    this->y = y;
}

bool AStar::pieceOnCoord(std::string bits, int x, int y){
    if(x%2 == 0){
        return y == 0 || y == 16;        
    }
    if(y%2 == 0){
        return false;
    } 
    int findBit = (4*(y-1)+(x-1)/2);
    return bits[findBit] != '0';
}

std::vector<Coord> AStar::aStar(std::string bits, int x1, int y1, int x2, int y2, bool compact){
    if(pieceOnCoord(bits,x2,y2)) {
        std::cout << "Seat taken" << std::endl;
        return std::vector<Coord>();
    }
    Node startNode(nullptr, x1, y1);
    Node endNode(nullptr, x2, y2);

    // to be checked
    std::vector<Node> openList;
    // checked
    std::vector<Node> closedList;

    openList.push_back(startNode);

    while(openList.size() > 0){
        // controleer node met beste score
        Node currentNode = openList[0];
        int currentIndex = 0;

        for (int i = 0; i < openList.size(); i++)
        {
            if(openList[i].f < currentNode.f){
                currentNode = openList[i];
                currentIndex = i;
            }
        }
        
        openList.erase(std::next(openList.begin(), currentIndex));
        closedList.push_back(currentNode);

        // als bij eindpunt, genereer path via gelinkte parents.
        if(currentNode.equal(endNode)){
            std::vector<Coord> path;
            while(!currentNode.equal(startNode)) {
                path.push_back(Coord(currentNode.x,currentNode.y));
                currentNode = *currentNode.parent;
            } 
            path.push_back(Coord(startNode.x,startNode.y));
            std::reverse(path.begin(), path.end());
            if(!compact) return path;

            // overbodige info erasen, daar waar dx en dy niet veranderen over het path
            int dx = 0;
            int dy = 0;
            std::vector<int> toBeDeleted;
            for (int i = 0; i < path.size()-1; i++)
            {
                int newDx = path[i+1].x - path[i].x;
                int newDy = path[i+1].y - path[i].y;

                if(dx == newDx && dy == newDy){
                    toBeDeleted.push_back(i);
                }
                dx = newDx;
                dy = newDy;
            }

            for (int i = toBeDeleted.size()-1; i >= 0; i--)
            {
                path.erase(std::next(path.begin(), toBeDeleted[i]));
            }
            return path;
        }

        std::vector<Node> children;

        int adjacent[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

        int checkPositions = 8;
        for (int i = 0; i < checkPositions; i++)
        {
            int nodeX = currentNode.x + adjacent[i][0];
            int nodeY = currentNode.y + adjacent[i][1];

            if(nodeX > 16 || nodeX < 0 || nodeY > 16 || nodeY < 0){
                continue;
            }

            if (pieceOnCoord(bits,nodeX,nodeY)){
                if(i<4){
                    checkPositions = 4;
                }
                continue;
            }
            children.push_back(Node(new Node(currentNode), nodeX, nodeY)); 
        }

        for(int i = 0; i < children.size(); i++){

            bool inClosed = false;
            for(int j = 0; j < closedList.size(); j++){
                if(children[i].equal(closedList[j])){
                    inClosed = true;
                    break;
                }
            }
            if(inClosed) continue;

            children[i].g = currentNode.g + 1;
            children[i].h = (children[i].x - endNode.x)*(children[i].x - endNode.x) + (children[i].y - endNode.y)*(children[i].y - endNode.y);
            children[i].f = children[i].g + children[i].h;

            bool inOpen = false;
            for(int j = 0; j < openList.size(); j++){
                if(children[i].equal(openList[j])){
                    if(children[i].f > (openList[i]).f){
                        inOpen = true;
                        break;
                    }
                }
            }
            if(inOpen) continue;

            openList.push_back(children[i]);
        }
    }

    throw std::invalid_argument("no solution");
}
