//
//  SlugSegment.cpp
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#include "SlugSegment.h"
#include "Slug.h"
#include "Sprite.h"

extern float RETINA_FACTOR;

SlugSegment::SlugSegment(Slug * owner, Uint8 connections, int size, int inset, int x, int y, bool isHead){
    this->x = x;
    this->y = y;
    this->connections = connections;
    this->owner = owner;
    this->size = size;
    this->inset = inset;
    source.x = 0;
    source.y = 0;
    this->isHead = isHead;
}

void SlugSegment::draw(int x, int y){
    if(connections & SLUG_EAST_CONNECTION)
        destination.w = source.w = size + inset;
    else
        destination.w = source.w = size;
    
    if(connections & SLUG_SOUTH_CONNECTION)
        destination.h = source.h = size + inset;
    else
        destination.h = source.h = size;
    destination.x = x;
    destination.y = y;
    destination.w *= RETINA_FACTOR;
    destination.h *= RETINA_FACTOR;
    //printf("in print method %i %i %i %i\n", destination.x, destination.y, source.w, source.h);
    SDL_RenderCopy(globalRenderer, this->owner->sprite, &source, &destination);
    if(isHead){
        owner->headSprite->draw(x, y);
        if(owner->getIsDone()){
            doneCheckmark->draw(x-inset,y-inset);
        }
    }
}

Slug * SlugSegment::getOwner(){
    return owner;
}

void SlugSegment::addConnection(Uint8 connection){
    connections |= connection;
}

void SlugSegment::removeConnection(Uint8 connection){
    connections &= ~connection;
}
