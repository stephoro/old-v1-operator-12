//
//  SLUG_Button.h
//  SLUG
//
//  Created by Steph on 3/20/16.
//
//

#ifndef __SLUG__SLUG_Button__
#define __SLUG__SLUG_Button__

#include <stdio.h>
#include "Interactable.h"
#include "Drawable.h"
#include "Sprite.h"

extern bool isRetina;



class SLUG_Button: public Interactable, public Drawable{
private:
    void (*myFunc)();
    char * name;
    char * colors;
public:
    SDL_Rect * rect;
    SDL_Texture * texture;
    SDL_Rect * textureSourceRect;
    SDL_Rect * textureFillRect;
    int x, y, w, h, tw, th;
    SLUG_Button(int x, int y, int w, int h, char * name, char * colors, void (*targetFun)());
    ~SLUG_Button();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    bool onInput(int x, int y);
    void draw();
    void update();
};



#endif /* defined(__SLUG__SLUG_Button__) */
