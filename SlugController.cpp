//
//  SlugController.cpp
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#include "SlugController.h"
#include "TurnManager.h"
#include "Swipable.hpp"
#include "StringHelpers.h"
extern float RETINA_FACTOR;


/*
 constructor
 */
SlugController::SlugController(){
    sheet = new SpriteSheet("images/arrs.png", 64, 64, 32, 32);
    arrowUp = sheet->getSprite(0);
    arrowRight = sheet->getSprite(1);
    arrowDown = sheet->getSprite(4);
    arrowLeft = sheet->getSprite(3);
    movementSquare = sheet->getSprite(2);
    attackSquare = sheet->getSprite(5);
    mode = CONTROLLER_MODE_MOVE;
    slug = NULL;
    actionMenu = NULL;
    interactionLayer = interactionStack->expandLayers();
    renderLayer = mainStack->expandLayers();
    pathfinder = new AStarSearch();
    foundPath = NULL;
    pathlen = -1;
    
}

/*
 destructor
 */
SlugController::~SlugController(){
    delete sheet;
    delete arrowUp;
    delete arrowRight;
    delete arrowDown;
    delete arrowLeft;
    delete attackSquare;
    delete movementSquare;
    if(actionMenu != NULL)
        delete actionMenu;
    delete pathfinder;
    if(foundPath != NULL){
        int p = pathlen;
        while(p--)
            delete foundPath[p];
        delete [] foundPath;
    }
}

/**
 handle one square, get if it is good for movement
 @param px The position x of the tile in tile coordinate system
 @param py The position y of the tile in tile coordinate system
 @param slug The slug to be compared at the point
 @param draw Whether the square should be rendered if valid
 */
inline bool SlugController::handleSquare(int px, int py, bool draw){
    int isOccupied = map->isOccupied(px, py, slug);
    bool squareGood = isOccupied == MAP_TILE_OCCUPIED_SELF || isOccupied == MAP_TILE_EMPTY;
    if(squareGood && draw){
        map->pointToDraw(px, py);
        movementSquare->draw(px, py);
    }
    return squareGood;
}

inline bool SlugController::handleAttackSquare(int px, int py, bool draw){
    int isOccupied = map->isOccupied(px, py, slug);
    bool squareGood = isOccupied == MAP_TILE_EMPTY || isOccupied == MAP_TILE_OCCUPIED_OTHER || (isOccupied == MAP_TILE_OCCUPIED_SELF && currentAction->canTargetSelf);
    if(squareGood && draw){
        map->pointToDraw(px, py);
        attackSquare->draw(px, py);
    }
    return squareGood;
}

void SlugController::update(){
    
}

void SlugController::draw(){
    if(!inputBad()){
        mode = slug->getControlMode();
        switch (mode) {
            case CONTROLLER_MODE_MOVE:
                renderSlugMovement();
                break;
                
            case CONTROLLER_MODE_ATTACK:
                renderSlugAttack();
                break;
                
            default:
                break;
        }
    }
}

/**
 renders a controller around a slug.
 It radiates outwards in expanding diamonds for how large the controller is.
 */
void SlugController::renderSlugMovement(){
    if(!inputBad()){
        /*
         sx and sy are the center x and y, used when rendering
         */
        int sx = slug->getX(),
        sy = slug->getY(),
        speed = slug->getMovesLeft();
        if(speed > 10)
            speed = 10;
        int speed2 = speed*2;
        int intsize = speed2+1;
        /*
         we used new, we must delete mem later
         */
        int ** moves = new int*[intsize];
        for (int x = 0; x < intsize; x++) {
            moves[x] = new int[intsize];
        }
        moves[speed][speed] = speed + 2;
        /*
         each iteration of the for is a new and larger diamond
         of rendered squares.
         */
        for(int runLen = 1; runLen <= speed; runLen++){
            int runLen1 = runLen - 1;
            //start at west tip of diamond
            int x = -runLen;
            int y = 0;
            //go on northwest edge to northern edge
            for(int i = 0; i < runLen1; i++){
                x++;
                y--;
                /*
                 Tests these
                 @X
                 X
                 */
                int sum = std::max(moves[speed+x+1][speed+y],moves[speed+x][speed+y+1]) - 1;
                moves[speed+x][speed+y] = -2;
                if(sum > 0){
                    bool squaregood = handleSquare(x + sx, y + sy, true);
                    if(squaregood)
                        moves[speed+x][speed+y] = sum;
                }
            }
            x++;
            y--;
            /*
             Tests northern tip
             @
             X
             */
            int sum = moves[speed+x][speed+y + 1] - 1;
            moves[speed+x][speed+y] = -2;
            if(sum > 0){
                bool squaregood = handleSquare(x + sx, y + sy, runLen != 1);
                if(squaregood)
                    moves[speed+x][speed+y] = sum;
            }
            //run along northeastern edge
            for(int i = 0; i < runLen1; i++){
                x++;
                y++;
                int sum = std::max(moves[speed+x-1][speed+y],moves[speed+x][speed+y+1]) - 1;
                moves[speed+x][speed+y] = -2;
                /*
                 Tests this
                 X@
                 X
                 */
                if(sum > 0){
                    bool squaregood = handleSquare(x + sx, y + sy, true);
                    if(squaregood)
                        moves[speed+x][speed+y] = sum;
                }
            }
            x++;
            y++;
            //test the east tip
            sum = moves[speed+x - 1][speed+y] - 1;
            moves[speed+x][speed+y] = -2;
            if(sum > 0){
                bool squaregood = handleSquare(x + sx, y + sy, runLen != 1);
                if(squaregood)
                    moves[speed+x][speed+y] = sum;
            }
            //run along southeastern edge
            for(int i = 0; i < runLen1; i++){
                x--;
                y++;
                /*
                 Test
                 X
                 X@
                 */
                int sum = std::max(moves[speed+x-1][speed+y],moves[speed+x][speed+y-1]) - 1;
                moves[speed+x][speed+y] = -2;
                if(sum > 0){
                    bool squaregood = handleSquare(x + sx, y + sy, true);
                    if(squaregood)
                        moves[speed+x][speed+y] = sum;
                }
            }
            x--;
            y++;
            sum = moves[speed+x][speed+y-1] - 1;
            //test southern tip
            moves[speed+x][speed+y] = -2;
            if(sum > 0){
                bool squaregood = handleSquare(x + sx, y + sy, runLen != 1);
                if(squaregood)
                    moves[speed+x][speed+y] = sum;
            }
            //test southwestern edge
            for(int i = 0; i < runLen1; i++){
                x--;
                y--;
                /*
                 Test
                 X
                 @X
                 */
                int sum = std::max(moves[speed+x+1][speed+y],moves[speed+x][speed+y-1]) - 1;
                moves[speed+x][speed+y] = -2;
                if(sum > 0){
                    bool squaregood = handleSquare(x + sx, y + sy, true);
                    if(squaregood)
                        moves[speed+x][speed+y] = sum;
                }
            }
            x--;
            y--;
            //test western tip
            sum = moves[speed+x+1][speed+y] - 1;
            moves[speed+x][speed+y] = -2;
            if(sum > 0){
                bool squaregood = handleSquare(x + sx, y + sy, runLen != 1);
                if(squaregood)
                    moves[speed+x][speed+y] = sum;
            }
        }
        
        for (int x = 0; x < intsize; x++) {
            delete [] moves[x];
        }
        delete [] moves;
        /*
         draw the arrows around the sprite directly
         */
        if(speed >= 1){
            int px = sx;
            int py = sy - 1;
            int isOccupied = map->isOccupied(px, py, slug);
            if(isOccupied == MAP_TILE_OCCUPIED_SELF || isOccupied == MAP_TILE_EMPTY){
                map->pointToDraw(px, py);
                arrowUp->draw(px, py);
            }
            px = sx + 1;
            py = sy;
            isOccupied = map->isOccupied(px, py, slug);
            if(isOccupied == MAP_TILE_OCCUPIED_SELF || isOccupied == MAP_TILE_EMPTY){
                map->pointToDraw(px, py);
                arrowRight->draw(px, py);
            }
            px = sx;
            py = sy + 1;
            isOccupied = map->isOccupied(px, py, slug);
            if(isOccupied == MAP_TILE_OCCUPIED_SELF || isOccupied == MAP_TILE_EMPTY){
                map->pointToDraw(px, py);
                arrowDown->draw(px, py);
            }
            px = sx - 1;
            py = sy;
            isOccupied = map->isOccupied(px, py, slug);
            if(isOccupied == MAP_TILE_OCCUPIED_SELF || isOccupied == MAP_TILE_EMPTY){
                map->pointToDraw(px, py);
                arrowLeft->draw(px, py);
            }
        }
    }
}

void SlugController::renderSlugAttack(){
    if(!inputBad()){
        /*
         sx and sy are the center x and y, used when rendering
         */
        int sx = slug->getX(),
        sy = slug->getY(),
        attackRange = currentAction->getRange();
        
        handleAttackSquare(sx, sy, true);
        
        /*
         each iteration of the for is a new and larger diamond
         of rendered squares.
         */
        for(int runLen = 1; runLen <= attackRange; runLen++){
            //start at west tip of diamond
            int x = -runLen;
            int y = 0;
            //go on northwest edge to northern edge
            for(int i = 0; i < runLen; i++){
                x++;
                y--;
                handleAttackSquare(x + sx, y + sy, true);
            }
            //run along northeastern edge
            for(int i = 0; i < runLen; i++){
                x++;
                y++;
                handleAttackSquare(x + sx, y + sy, true);
            }
            
            //run along southeastern edge
            for(int i = 0; i < runLen; i++){
                x--;
                y++;
                handleAttackSquare(x + sx, y + sy, true);
            }
            
            //test southwestern edge
            for(int i = 0; i < runLen; i++){
                x--;
                y--;
                handleAttackSquare(x + sx, y + sy, true);
            }
        }
    }
}

void SlugController::setSlug(Slug *slug){
    this->slug = slug;
    printf("Setting Slug!\n");
    if(slug != NULL){
        mode = slug->getControlMode();
        proto = slug->getSLUG();
        proto->actions->refocus();
        currentAction = slug->currentAction;
        printf("Setting Slug! %i\n",slug->team);
        if((slug->team & playerTeamMask)==0){
            printf("enemy\n");
            SlugSegment * closest = slug->manager->getClosestEnemy(slug);
            if(closest == NULL){
                printf("No Closest to search against!\n");
                return;
            }
            printf("closest found\n");
            
            Position * start = new Position(slug->getX(),slug->getY());
            Position * end = new Position(closest->x,closest->y);
            printf("(%i %i) -> (%i %i)\n",slug->getX(),slug->getY(),closest->x,closest->y);
            if(foundPath != NULL){
                int p = pathlen;
                while(p--)
                    delete foundPath[p];
                delete [] foundPath;
            }
            foundPath = pathfinder->search(map, slug, start, end, pathlen);
            pathindex = 0;
            printf("moving len: %i\n", pathlen);
            for (pathindex = 0; pathindex < (pathlen-1) && slug->getMovesLeft(); pathindex++) {
                
                Position * cur = foundPath[pathindex];
                Position * next = foundPath[pathindex + 1];
                printf("(%i,%i) to (%i,%i)\n",cur->x,cur->y,next->x,next->y);
                SlugSegment * atPos = map->getSlug(next->x,next->y);
                if(atPos!=NULL){
                    printf("ATTACK!!!\n");
                    slug->startAttack();
                    break;
                }else{
                    int dir = -1;
                    int dx = next->x-cur->x;
                    int dy = next->y-cur->y;
                    if(dx > 0)
                        dir = 1;
                    else if(dx < 0)
                        dir = 3;
                    else if(dy > 0)
                        dir = 2;
                    else if(dy < 0)
                        dir = 0;
                    if(dir != -1){
                        printf("moving dir: %i\n",dir);
                        slug->move(dir);
                    }else{
                        printf("ATTACK!!!\n");
                        slug->startAttack();
                        break;
                    }
                }
            }
            if(slug->getMovesLeft()){
                printf("ATTACK!!!\n");
                slug->startAttack();
            }
        }
    }
}

bool SlugController::inputBad(){
    return slug == NULL || !slug->manager->isOwnTurn() || slug->getIsDone();
}

bool SlugController::handlesMove(int x, int y){
    int sx = slug->getX();
    int sy = slug->getY();
    int dy = y - sy;
    int dx = x - sx;
    int dist = abs(dx) + abs(dy);
    bool willHandle = dist == 1;
    if(!willHandle)
        return false;
    int isOccupied = map->isOccupied(x, y, slug);
    willHandle = isOccupied == MAP_TILE_OCCUPIED_SELF || isOccupied == MAP_TILE_EMPTY;
    if(!willHandle)
        return false;
    if (dy == -1) {
        slug->move(0);
    }else if(dx == 1){
        slug->move(1);
    }else if(dy == 1){
        slug->move(2);
    }else if(dx == -1){
        slug->move(3);
    }
    if (slug->getIsDone()) {
        slug->manager->slugDone();
    }
    return true;
}

bool SlugController::handlesAttack(int x, int y){
    int sx = slug->getX();
    int sy = slug->getY();
    int dy = y - sy;
    int dx = x - sx;
    int dist = abs(dx) + abs(dy);
    bool willHandle = dist >= 0 && dist <= currentAction->getRange();
    if(!willHandle)
        return false;
    int occupancy = map->isOccupied(x, y, slug);
    if((occupancy & MAP_TILE_OUT_OF_BOUNDS) || (occupancy == MAP_TILE_OCCUPIED_SELF && !currentAction->canTargetSelf))
        return false;
    SlugSegment * atPos = map->getSlug(x, y);
    Slug * target = NULL;
    if(atPos != NULL){
        target = atPos->getOwner();
    }
    
    if(currentAction->commit(slug, target, x, y)){
        slug->invalidate();
        return true;
    }
    return false;
}

void SlugController::onChoice(int c){
    printf("choice %i\n",c);
    int ch = c - 2;
    if(ch < 0){
        if(ch == -1){
            slug->invalidate();
        }
    }else{
        proto->actions->refocus();
        while(ch > 0){
            ch--;
            proto->actions->next();
        }
        currentAction = proto->actions->next();
        slug->currentAction = currentAction;
        slug->controlMode = CONTROLLER_MODE_ATTACK;
        
    }
    hideMenu();
}

void SlugController::displayMenu(){
    if(actionMenu != NULL){
        interactionLayer->remove(actionMenu);
        renderLayer->remove(actionMenu);
        delete actionMenu;
        actionMenu=NULL;
    }
    actionMenu = new DropMenu(0,NULL, NULL, NULL, NULL);
    actionMenu->delegate = this;
    if(slug != NULL){
        const SLUG * data = slug->getSLUG();
        data->actions->refocus();
        int l = actionMenu->length = data->actions->getSize() + 2;
        char ** items = new char*[l];
        items[0] = copyStr("Close");
        items[1] = copyStr("No Action");
        for(int i = 0; i < l - 2; i++){
            items[i + 2] = copyStr(data->actions->next()->name);
        }
        char * colors = new char[l*4];
        char * fgcolors = new char[l*4];
        
        colors[0] = 255;
        colors[1] = 255;
        colors[2] = 255;
        colors[3] = 255;
        colors[4] = 255;
        colors[5] = 255;
        colors[6] = 255;
        colors[7] = 255;
        fgcolors[0] = 255;
        fgcolors[1] = 0;
        fgcolors[2] = 0;
        fgcolors[3] = 0;
        fgcolors[4] = 255;
        fgcolors[5] = 0;
        fgcolors[6] = 0;
        fgcolors[7] = 0;
        
        for(int i = 2; i < l; i++){
            int b = i*4;
            colors[b] = 255;
            colors[b + 1] = 255;
            colors[b + 2] = 0;
            colors[b + 3] = 0;
            
            fgcolors[b] = 255;
            fgcolors[b + 1] = 255;
            fgcolors[b + 2] = 255;
            fgcolors[b + 3] = 255;
        }
        
        int h = 25*RETINA_FACTOR, w = 15*RETINA_FACTOR;
        
        actionMenu->itemYOffset = w;
        actionMenu->menuYOffset = h;
        actionMenu->menuXOffset = w;
        actionMenu->itemWidth = screenBounds.w - w*2;
        actionMenu->itemHeight = h;
        actionMenu->items = items;
        actionMenu->colors = colors;
        actionMenu->fgcolors = fgcolors;
        actionMenu->needsUpdate = true;
        actionMenu->update();
    }
    oldSwiper = swiper;
    swiper = actionMenu;
    interactionLayer->add(actionMenu);
    renderLayer->add(actionMenu);
}

void SlugController::hideMenu(){
    if(actionMenu != NULL){
        interactionLayer->remove(actionMenu);
        renderLayer->remove(actionMenu);
        delete actionMenu;
        actionMenu=NULL;
    }
    swiper = oldSwiper;
}

bool SlugController::handlesInput(int x, int y){
    if(inputBad())
        return false;
    switch (mode) {
        case CONTROLLER_MODE_ATTACK:
            return handlesAttack(x, y);
            break;
        case CONTROLLER_MODE_MOVE:
            return handlesMove(x, y);
            break;
            
        default:
            return false;
            break;
    }
}