# 13/09/2023 
# edited code from https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2

# https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2
# https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a176c5d2e4c2f21e9e92b12e39a6f0e67

import random


class Node():
    def __init__(self, parent, x, y):
        self.parent = parent
        self.x = x
        self.y = y

        self.g = 0
        self.h = 0
        self.f = 0

    def equals(self, other):
        return self.x == other.x and self.y == other.y

def piece_on_coord(maze,x,y):
    if x % 2 == 0 or y % 2 == 0:
        return False
    # index = 8 * ((y-1)/2) + (x-1)/2
    return bool(int(maze[4*(y-1)+(x-1)//2]))

def astar(maze, x1, y1, x2, y2):
    x1 = x1*2+1
    y1 = y1*2+1
    x2 = x2*2+1
    y2 = y2*2+1

    # Create start and end node
    start_node = Node(None, x1, y1)
    end_node = Node(None, x2, y2)

    # open is to be checked. closed has been checked
    open_list = []
    closed_list = []

    open_list.append(start_node)

    # Loop until finding the end
    while len(open_list) > 0:

        # get closed unchecked node
        current_node = open_list[0]
        current_index = 0
        for index, item in enumerate(open_list):
            if item.f < current_node.f:
                current_node = item
                current_index = index

        # Pop current off open list, add to closed list
        open_list.pop(current_index)
        closed_list.append(current_node)

        # Found it
        if current_node.equals(end_node):
            path = []
            current = current_node
            # backtrack path
            while current is not None:
                path.append((current.x, current.y))
                current = current.parent
            return path[::-1]


        children = []
        
        adjacent = [(0, -1), (0, 1), (-1, 0), (1, 0), (-1, -1), (-1, 1), (1, -1), (1, 1)]
        adjacent_close = [(0, -1), (0, 1), (-1, 0), (1, 0)]

        for close_position in adjacent_close:
            node_position_x = current_node.x + close_position[0]
            node_position_y = current_node.y + close_position[1]


            if node_position_x > 16 or node_position_x < 0 or node_position_y > 16 or node_position_y < 0:
                continue

            if piece_on_coord(maze,node_position_x,node_position_y):
                adjacent = adjacent_close
                break
                

        for new_position in adjacent: # Adjacent squares
            node_position_x = current_node.x + new_position[0]
            node_position_y = current_node.y + new_position[1]

            if node_position_x > 16 or node_position_x < 0 or node_position_y > 16 or node_position_y < 0:
                continue

            # Make sure walkable terrain
            if piece_on_coord(maze,node_position_x, node_position_y):
                continue

            # Create new node
            new_node = Node(current_node, node_position_x, node_position_y)

            # Append
            children.append(new_node)

        # Loop through children
        for child in children:

            # Child is on the closed list
            in_closed = False
            for closed_child in closed_list:
                if child.equals(closed_child):
                    in_closed = True
                    break
            if in_closed:
                continue

            # Create the f, g, and h values
            child.g = current_node.g + 1
            child.h = ((child.x - end_node.x) ** 2) + ((child.y - end_node.y) ** 2)
            child.f = child.g + child.h

            # Child is already in the open list
            in_open = False
            for open_node in open_list:
                if child.equals(open_node) and child.g > open_node.g:
                    in_open = True
                    break
            if in_open:
                continue

            # Add the child to the open list
            open_list.append(child)


def main():
    # 64 bits
    maze = "".join([ "0" for x in range(64)])

    print(maze)
    
    for x in range(10000):
        start_x = random.randint(0,7)
        start_y = random.randint(0,7)
        end_x = random.randint(0,7)
        end_y = random.randint(0,7)
        print(start_x,start_y,end_x,end_y)

        path = astar(maze, start_x, start_y, end_x, end_y)
        # print(path)


        dx = 0
        dy = 0
        tobedel = []
        for i,point in enumerate(path[:-1]):
            ndx = path[i+1][0] - point[0]
            ndy= path[i+1][1] - point[1]

            if dx == ndx and dy == ndy:
                tobedel.append(i)
            
            dx = ndx
            dy = ndy
        
        # print(tobedel)

        for num in tobedel[::-1]:
            path.pop(num)

        print(path)
        

if __name__ == '__main__':
    main()