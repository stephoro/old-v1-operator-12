//
//  Sprite.h
//  SLUG
//
//  Created by Steph on 3/4/16.
//
//



#ifndef __SLUG__Sprite__
#define __SLUG__Sprite__
#include "SDL.h"

#include "Drawable.h"
#include <stdio.h>

class SpriteSheet;
extern SDL_Renderer *globalRenderer;
class Sprite{
private:
    SpriteSheet *spriteSheet;
    SDL_Rect drawRect;
    SDL_Rect source;
public:
    Sprite(SpriteSheet * spriteSheet, int drawWidth, int drawHeight, int x, int y, int w, int h);
    /**
     
     */
    void draw(int x, int y);

    void update();

};
#endif /* defined(__SLUG__Sprite__) */
