//
//  AStar.hpp
//  SLUG
//
//  Created by Steph on 7/15/16.
//
//

#ifndef AStar_hpp
#define AStar_hpp

#include <stdio.h>
#include "Map.h"
#include "BinaryHeap.hpp"

class Position{
public:
    Position(int x, int y){
        this->x = x;
        this->y = y;
        f = 0;
        g = 0;
        h = 0;
        cost = 1;
        visited = false;
        closed = false;
        parent = NULL;
    }
    Position * copy(){
        return new Position(x,y);
    }
    void print(){
        printf("(%i,%i)",x,y);
    }
    int x;
    int y;
    int f, g, h;
    int cost;
    bool visited;
    bool closed;
    Position * parent;
};

int manhatten(Position * start, Position * end);


class AStarSearch{
private:
    int width, height;
    Position *** grid;
    //new copy
    Position ** pathTo(Position * currentNode, int & len){
        Position * search = currentNode;
        len = 0;
        while(search != NULL){
            search = search->parent;
            len++;
        }
        int b = len;
        Position ** path = new Position*[len+1];
        while(b--){
            path[b] = currentNode;
            currentNode = currentNode->parent;
        }
        path[len] = NULL;
        return NULL;
    }
    void blankGrid(){
        if(grid != NULL)
            trashGrid();
        grid = new Position**[width];
        for(int x = 0; x < width; x++){
            Position ** col = grid[x] = new Position*[height];
            for(int y = 0; y < height; y++){
                col[y] = NULL;

                //printf("cost(1) %i 0s (%i %i %i)\n",pos.cost,pos.f,pos.g,pos.h);
            }
        }
    }
    void sweepGrid(){
        for(int x = 0; x < width; x++){
            Position ** col = grid[x];
            for(int y = 0; y < height; y++){
                Position * o = col[y];
                if(o != NULL)
                    delete o;
                col[y] = NULL;
            }
        }
    }
    void trashGrid(){
        for(int x = 0; x < width; x++){
            //grid has already been swept; impossible to delete otherwise
            delete [] grid[x];
        }
        delete [] grid;
        grid = NULL;
    }
    //must return new objects if null;
    Position ** getNeighbors(Position *node, int & length){
        length = 0;
        int max = 4;
        Position ** neighbors = new Position*[max];
        int k = -1;
        int x = node->x;
        int y = node->y;
        int dx, dy;
        while(k++ < 3){
            switch (k) {
                case 0:
                    dx = 0;
                    dy = -1;
                    break;
                case 1:
                    dx = 1;
                    dy = 0;
                    break;
                case 2:
                    dx = 0;
                    dy = 1;
                    break;
                default:
                    dx = -1;
                    dy = 0;
                    break;
            }
            int ax =x+dx;
            int ay =y+dy;
            int status = map->isOccupied(ax, ay, inquisitor);
            if(status&(MAP_TILE_EMPTY|MAP_TILE_OCCUPIED_SELF)){
                Position * neighbor = grid[ax][ay];
                if(neighbor == NULL)
                    neighbor = grid[ax][ay] = new Position(ax,ay);
                if(!neighbor->closed)
                    neighbors[length++] = neighbor;
            }
        }
        
        return neighbors;
    }
    Map * map;
    Slug * inquisitor;
public:
    AStarSearch(){
        grid = NULL;
    }
    ~AStarSearch(){
        if(grid != NULL)
            trashGrid();
    }
    

    
    
    
    Position ** search(Map * map, Slug * inquisitor, Position * start, Position * end, int & returnLength){
        this->map = map;
        this->inquisitor = inquisitor;
        if(start->x == end->x && start->y == end->y){
            Position ** ret = new Position*[2];
            ret[0] = start->copy();
            ret[1] = NULL;
            delete start;
            delete end;
            return ret;
        }
        
        if(grid != NULL && width == map->width && height == map->height){
            //we've already swept the grid! Reduce reuse (recycle)
        }else{
            width = map->width;
            height = map->height;
            blankGrid();
        }

        
        grid[start->x][start->y] = start;
        grid[end->x][end->y] = end;
        
        Position * closest = start;
        start->h = start->f = manhatten(start, end);
        
        BinaryHeap<Position> * openHeap = new BinaryHeap<Position>();
        openHeap->add(start, start->f);
        
        while (openHeap->getFill() > 0) {
            
            // Grab the lowest f(x) to process next.  Heap keeps this sorted for us.
            Position * currentNode = openHeap->remove();
            // End case -- result has been found, return the traced path.
            if (currentNode == end) {
                return pathTo(currentNode, returnLength);
            }
            
            // Normal case -- move currentNode from open to closed, process each of its neighbors.
            currentNode->closed = true;
            
            // Find all neighbors for the current node. Optionally find diagonal neighbors as well (false by default).
            int len;
            Position ** neighbors = getNeighbors(currentNode, len);
            
            for (int i = 0; i < len; i++) {
                Position * neighbor = neighbors[i];
                
                
                // The g score is the shortest distance from start to current node.
                // We need to check if the path we have arrived at this neighbor is the shortest one we have seen yet.
                int gScore = currentNode->g + neighbor->cost;
                bool beenVisited = neighbor->visited;
                
                if (!beenVisited || gScore < neighbor->g) {
                    
                    // Found an optimal (so far) path to this node.  Take score for node to see how good it is.
                    neighbor->visited = true;
                    neighbor->parent = currentNode;
                    neighbor->h = neighbor->h || manhatten(neighbor, end);
                    neighbor->g = gScore;
                    neighbor->f = neighbor->g + neighbor->h;
                    
                    if (closest == NULL || neighbor->h < closest->h) {
                        closest = neighbor;
                    }
                    if (!beenVisited) {
                        // Pushing to heap will put it in proper place based on the 'f' value.
                        openHeap->add(neighbor, neighbor->f);
                    } else {
                        // Already seen the node, but since it has been rescored we need to reorder it in the heap
                        openHeap->rescoreIndex(openHeap->search(neighbor), neighbor->f);
                    }
                }
            }
            delete [] neighbors;
        }
        
        delete openHeap;
        
        // No result was found - empty array signifies failure to find path.
        if (closest != NULL && closest != start) {
            return pathTo(closest, returnLength);
        }
        
        sweepGrid(); // deletes positions, but not grid itself
        
        
        Position ** ret = new Position*[1];
        ret[0] = NULL;
        return ret;
    }
};






#endif /* AStar_hpp */
