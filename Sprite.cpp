//
//  Sprite.cpp
//  SLUG
//
//  Created by Steph on 3/4/16.
//
//

#include "Sprite.h"
#include "SpriteSheet.h"

void Sprite::draw(int x, int y){
    drawRect.x = x;
    drawRect.y = y;
    SDL_RenderCopy(globalRenderer, spriteSheet->sheet, &source, &drawRect);
}

void Sprite::update(){
    
}


Sprite::Sprite(SpriteSheet * spriteSheet, int drawWidth, int drawHeight, int x, int y, int w, int h){
    drawRect.w = drawWidth;
    drawRect.h = drawHeight;
    source.x = x;
    source.y = y;
    source.w = w;
    source.h = h;
    this->spriteSheet = spriteSheet;
}
