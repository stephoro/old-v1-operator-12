//
//  SlugSprite.h
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#ifndef __SLUG__SlugSprite__
#define __SLUG__SlugSprite__

#include <stdio.h>
#include "SDL.h"

extern SDL_Renderer *globalRenderer;
class SlugSprite{
private:
    SDL_Texture * texture;
    SDL_Rect drawRect;
    SDL_Rect source;
public:
    SlugSprite(SDL_Texture * texture, int size, int expansionSize, int sourceSize);

    void draw();
    
};

#endif /* defined(__SLUG__SlugSprite__) */
