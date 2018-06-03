//
//  SlugController.h
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#ifndef __SLUG__SlugController__
#define __SLUG__SlugController__

#include <stdio.h>
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Slug.h"
#include "Drawable.h"
#include "DrawStack.h"
#include "InputStack.h"
#include "DropMenu.hpp"
const int CONTROLLER_MODE_INVALID = -1;
const int CONTROLLER_MODE_MOVE = 0;
const int CONTROLLER_MODE_ATTACK = 1;

extern DrawStack * mainStack;
extern InputStack * interactionStack;
extern Swipable * swiper;
extern Map * map;

class SlugController : public Drawable, private DropMenuDelegate{
private:
    InputLayer * interactionLayer;
    DrawLayer * renderLayer;
    
    DropMenu * actionMenu;
    
    SlugAction * currentAction;
    
    Sprite * arrowLeft;
    Sprite * arrowRight;
    Sprite * arrowUp;
    Sprite * arrowDown;
    Sprite * movementSquare;
    Sprite * attackSquare;
    SpriteSheet * sheet;
    Slug * slug;
    const SLUG * proto;
    Swipable * oldSwiper;
    
    bool inputBad();
    int mode;
    void renderSlugMovement();
    void renderSlugAttack();
    inline bool handleAttackSquare(int px, int py, bool draw);
    inline bool handleSquare(int px, int py, bool draw);
    bool handlesMove(int x, int y);
    bool handlesAttack(int x, int y);
public:
    void onChoice(int c);
    void displayMenu();
    void hideMenu();
    
    SlugController();
    ~SlugController();
    void update();
    void draw();
    void setSlug(Slug *slug);
    bool handlesInput(int x, int y);
};

#endif /* defined(__SLUG__SlugController__) */
