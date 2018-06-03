//
//  SlugSegment.h
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#ifndef __SLUG__SlugSegment__
#define __SLUG__SlugSegment__

#include <stdio.h>
#include "SDL.h"
#include "LinkedList.hpp"

extern SDL_Renderer * globalRenderer;
extern bool isRetina;

/** 
 The first bit true, signifies that we have a connection
 in the right or east direction
 */
const Uint8 SLUG_EAST_CONNECTION = 0x1;
/**
 The second bit true, signifies that we have a connection
 in the down or south direction
 */
const Uint8 SLUG_SOUTH_CONNECTION = 0x2;

class Slug;
class SlugSegment{
    /** the owner of this segment */
    Slug * owner;
    /** 
     The bit array of all the connections of this square.
     We can only have east and south connections.
     */
    Uint8 connections;
    /** the width/height of this segment */
    int size;
    /** the inset of this into  */
    int inset;
    SDL_Rect destination;
    SDL_Rect source;
public:
    bool isHead;
    int x, y;
    LinkedList<SlugSegment>::Node<SlugSegment> * reference;
    Slug * getOwner();
    SlugSegment(Slug * owner, Uint8 connections, int size, int inset, int x, int y, bool isHead);
    void draw(int x, int y);
    void addConnection(Uint8 connection);
    void removeConnection(Uint8 connection);
};

#endif /* defined(__SLUG__SlugSegment__) */
