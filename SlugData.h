//
//  SlugData.h
//  SLUG
//
//  Created by Steph on 3/18/16.
//
//

#ifndef __SLUG__SlugData__
#define __SLUG__SlugData__

#include <stdio.h>
#include "SDL.h"
#include "SlugAttack.h"
#include "Sprite.h"
#include "SpriteSheet.h"

class SlugData{
public:
    SLUG * prototype;
    bool isCopy;
    SlugData * copy();
    SpriteSheet * sheet;
    Sprite * sprite;
    SDL_Texture * texture;
    void setProto(SLUG * proto);
    void setSheet(SpriteSheet * sheet);
    SlugData(bool isCopy = false);
    ~SlugData();
};

#endif /* defined(__SLUG__SlugData__) */
