//
//  SlugData.cpp
//  SLUG
//
//  Created by Steph on 3/18/16.
//
//

#include "SlugData.h"
#include "StringHelpers.h"
#include "TextureLoader.h"


SlugData::SlugData(bool isCopy){
    this->isCopy = isCopy;
    prototype = NULL;
    sprite = NULL;
    texture = NULL;
}

SlugData * SlugData::copy(){
    SlugData * newSlug = new SlugData(true);
    newSlug->prototype = prototype;
    newSlug->sprite = sprite;
    newSlug->texture = texture;
    return newSlug;
}

void SlugData::setProto(SLUG * proto){
    prototype = proto;
    Uint32 color = proto->color;
    printf("color: %i %i %i %i\n",(color & 0xFF000000) >> 24, (color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8,  color & 0x000000FF);
    this->texture = slugTexture(28, 4, 14, (color & 0x00FF0000) >> 16,  (color & 0x0000FF00) >> 8,  color & 0x000000FF);
}

void SlugData::setSheet(SpriteSheet * sheet){
    this->sheet = sheet;
    this->sprite = sheet->getSprite(prototype->xIndex, prototype->yIndex);
}

SlugData::~SlugData(){
    if(!isCopy && sprite != NULL)
        delete sprite;
    if(!isCopy && texture != NULL){
        SDL_free(sprite);
    }
}