//
//  SpriteSheet.cpp
//  SLUG
//
//  Created by Steph on 3/4/16.
//
//

#include "SpriteSheet.h"
#include "Sprite.h" //fix the circular dependency
extern bool isRetina;
extern float RETINA_FACTOR;
SpriteSheet::SpriteSheet(std::string path, int spriteWidth, int spriteHeight, int drawWidth, int drawHeight){
    if(isRetina){
        drawWidth <<=1;
        drawHeight <<=1;
    }
    this->spriteWidth = spriteWidth;
    this->spriteHeight = spriteHeight;
    this->drawWidth = drawWidth;
    this->drawHeight = drawHeight;
    
    sheet = loadTexture(path);
    int w, h;
    SDL_QueryTexture(sheet, NULL, NULL, &w, &h);
    width = w;
    height = h;
    xSprites = w / spriteWidth;
    ySprites = h / spriteHeight;

}

SpriteSheet::~SpriteSheet(){
    SDL_free(sheet);
}

Sprite * SpriteSheet::getSprite(int position){
    //Create texture from surface pixels
    int x = position % xSprites;
    int y = position/xSprites;
    return new Sprite(this, drawWidth, drawHeight, x*spriteWidth, y*spriteHeight, spriteWidth, spriteHeight);
}

Sprite * SpriteSheet::getSprite(int x, int y){
    //Create texture from surface pixels
    return new Sprite(this, drawWidth, drawHeight, x*spriteWidth, y*spriteHeight, spriteWidth, spriteHeight);
}

Sprite * SpriteSheet::getSprite(int x, int y, int w, int h){
    //Create texture from surface pixels
    int dw = w, dh = h;
    
    return new Sprite(this, dw, dh, x, y, w, h);
}


Sprite * SpriteSheet::getSprite(int x, int y, int w, int h, int dw, int dh){
    //Create texture from surface pixels
    
    return new Sprite(this, dw, dh, x, y, w, h);
}
