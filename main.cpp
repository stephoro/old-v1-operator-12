/*
 *  rectangles.c
 *  written by Holmes Futrell
 *  use however you want
 */


/**
 issues, fix events not being caugt because the app is asleep.
 make a redraw in square for animation purposes that don't kill the cpu
 enable low cpu mode in which we don't do a lot of the animations
 that appear in rects.
 undo button & history
 file systems hash
 saving / loading from documents
 */

#include "SDL.h"
#include <time.h>
#include "LinkedList.hpp"
#include "Test_LinkedList.h"
#include "SDL_image.h"
#include "TextureLoader.h"
#include "SpriteSheet.h"
#include "Sprite.h"
#include "Slug.h"
#include "Map.h"
#include "Test_Map.h"
#include "DrawStack.h"
#include "SlugController.h"
#include "DrawLayer.h"
#include "InputLayer.h"
#include "InputStack.h"
#include "SDL_touch.h"
#include "TurnManager.h"
#include "SlugData.h"
#include "SLUG_Button.h"
#include "Memory.h"
#include "gameplay.h"
#include "SDL_ttf.h"
#include "Hashmap.hpp"
#include "BinaryHeap.hpp"
#include "AStar.hpp"
HashMap<SlugFile> * SlugFileSystem;
TTF_Font * menuFont;
Swipable * swiper;
DrawStack * mainStack;
InputStack * interactionStack;
SDL_Rect screenBounds;
SDL_Rect screenPoints;
SDL_Renderer *globalRenderer;
SlugData ** archetypes;
TurnManager * playerTurnManager;

SDL_Texture * slug;
SlugController * slugController;
LinkedList<TurnManager> * turnManagerList = NULL;
Sprite * doneCheckmark;
SpriteSheet * slugHeadSheet = NULL;
int needsDisplay;

void handleInput(int x, int y);

Map* map;

bool isRetina = true;
float RETINA_FACTOR = 1;

int randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
                   SDL_Rect *clip = NULL)
{
    SDL_RenderCopy(ren, tex, clip, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
                   SDL_Rect *clip = NULL)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != NULL){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    dst.x <<= 1;
    dst.y <<= 1;
    dst.w <<= 1;
    dst.h <<= 1;
    renderTexture(tex, ren, dst, clip);
}




int testALL(){
    Test_LinkedList();
    Test_Map();
        return 0;
}





/**
 convert a touch event to static coordinates
 */
void handleTouch(float x, float y){
    int ix = x * screenBounds.w,
    iy = y * screenBounds.h;
    handleInput(ix, iy);
}

void handleInput(int x, int y){
    interactionStack->onInput(x, y);
}

void InitPlayer(){
    
}

void btnAttack(){
    if(map->focusSlug != NULL){// && map->focusSlug->manager == playerTurnManager){
        //map->focusSlug->startAttack();
        slugController->displayMenu();

    }
}

int
main(int argc, char *argv[])
{
    AStarSearch * star = new AStarSearch();
    delete star;
    return 0;
    
    //binary heap test
    /*BinaryHeap<int> * test = new BinaryHeap<int>();
    test->printIndexes(0, 0);
    test->add(NULL, 9);
    test->printIndexes(0, 0);
    test->add(NULL, 4);
    test->printIndexes(0, 0);
    test->add(NULL, -1);
    test->printIndexes(0, 0);
    test->add(NULL, -12);
    test->printIndexes(0, 0);
    test->add(NULL, 2);
    test->printIndexes(0, 0);
    test->add(NULL, 5);
    test->printIndexes(0, 0);
    
    test->remove();
    test->printIndexes(0, 0);
    test->remove();
    test->printIndexes(0, 0);
    test->remove();
    test->printIndexes(0, 0);
    test->remove();
    test->printIndexes(0, 0);
    test->remove();
    test->printIndexes(0, 0);
    test->remove();
    test->printIndexes(0, 0);
    test->remove();
    test->printIndexes(0, 0);

    delete test;
    return 0;*/
    
    
    
    needsDisplay = 1;
    testALL();
    
    SDL_Window *window;

    
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        return 1;
    }
    
    /* initialize TTF */
    if( TTF_Init() == -1 ){
        printf("Could not initialize SDL\n");
    return 1;
    }

    /* seed random number generator */
    srand((uint)time(NULL));
    
    /* get window bounds from sys */
    if (SDL_GetDisplayBounds(0, &screenBounds) != 0) {
        SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
        return 1;
    }
    
    
    Uint32 initopts = SDL_WINDOW_OPENGL;
    if(isRetina)
        initopts |= SDL_WINDOW_ALLOW_HIGHDPI;
    
    
    /* create window and renderer */
    window =
    SDL_CreateWindow(NULL, 0, 0, screenBounds.w, screenBounds.h,
                     initopts);
    
    
    
    
    if (!window) {
        printf("Could not initialize Window\n");
        return 1;
    }
    
    globalRenderer = SDL_CreateRenderer(window, -1, 0);
    if (!globalRenderer) {
        printf("Could not create renderer\n");
        return 1;
    }
    
    
    
    if(isRetina){
        int a = 0, b = 0;
        isRetina = false;
        SDL_GetRendererOutputSize(globalRenderer, &a, &b);
        if(a){
            RETINA_FACTOR = a/screenBounds.w;
            isRetina = true;
        }
    }
    screenPoints.x = 0;
    screenPoints.y = 0;
    screenPoints.w = screenBounds.w;
    screenPoints.h = screenBounds.h;
    screenBounds.w *= RETINA_FACTOR;
    screenBounds.h *= RETINA_FACTOR;
    
    
    
    gameplayMain();
    
    /* shutdown SDL */
    SDL_Quit();
    
    return 0;
}




