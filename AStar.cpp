//
//  AStar.cpp
//  SLUG
//
//  Created by Steph on 7/15/16.
//
//

#include "AStar.hpp"
inline int manhatten(const Position * start, const Position * end){
    int v = end->x - start->x;
    if(v<0)v=-v;
    int c = end->y - start->y;
    if(c<0)c=-c;
    return c+v;
}