//
//  SpriteSheet.h
//  SLUG
//
//  Created by Steph on 3/4/16.
//
//

#ifndef __SLUG__SpriteSheet__
#define __SLUG__SpriteSheet__

#include <stdio.h>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "TextureLoader.h"

class Sprite;
extern SDL_Renderer * globalRenderer;
class SpriteSheet{
private:
    int spriteWidth;
    int spriteHeight;
    int drawWidth;
    int drawHeight;
    int xSprites;
    int ySprites;

public:
    int width;
    int height;
    SDL_Texture* sheet;
    SpriteSheet(std::string path, int spriteWidth, int spriteHeight, int drawWidth, int drawHeight);
    Sprite * getSprite(int x, int y);
    Sprite * getSprite(int position);
    Sprite * getSprite(int x, int y, int w, int h);
    Sprite * getSprite(int x, int y, int w, int h, int dw, int dh);
    ~SpriteSheet();
};

#endif /* defined(__SLUG__SpriteSheet__) */
