//
//  DropMenu.hpp
//  SLUG
//
//  Created by Steph on 7/10/16.
//
//

#ifndef DropMenu_hpp
#define DropMenu_hpp

#include <stdio.h>
#include "Interactable.h"
#include "Swipable.hpp"
#include "Drawable.h"
#include "SDL.h"
#include "SDL_ttf.h"

extern bool isRetina;
extern SDL_Renderer * globalRenderer;
extern SDL_Rect screenBounds;
extern TTF_Font * menuFont;


class DropMenuDelegate{
public:
    virtual void onChoice(int c) = 0;
};


class DropMenu : public Drawable, public Interactable, public Swipable{
public:
    int itemWidth;
    int itemHeight;
    int itemYOffset;
    int menuXOffset;
    int menuYOffset;
    int deltaY;
    int length;
    bool needsUpdate;
    char ** items;
    SDL_Texture ** texts;
    SDL_Rect ** textRects;
    char * colors;
    char * fgcolors;
    DropMenuDelegate * delegate;
    DropMenu(int length, char ** items, char * colors, char * fgcolors, DropMenuDelegate * delegate);
    ~DropMenu();
    
    void draw();
    void update();
    
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    bool onInput(int x, int y);
    
    void addOffsetX(int delta);
    void addOffsetY(int delta);
};

#endif /* DropMenu_hpp */
