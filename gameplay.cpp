//
//  gameplay.cpp
//  SLUG
//
//  Created by Steph on 3/20/16.
//
//

#include "gameplay.h"
#include "Map.h"
#include "SDL.h"
#include "DrawStack.h"
#include "InputStack.h"
#include "SlugData.h"
#include "TurnManager.h"
#include "LinkedList.hpp"
#include "Memory.h"
#include "Slug.h"
#include "SlugController.h"
#include "SLUG_Button.h"
#include "StringHelpers.h"
#include "DropMenu.hpp"
#include "SDL_ttf.h"
#include "Hashmap.hpp"

extern HashMap<SlugFile> * SlugFileSystem;
extern float RETINA_FACTOR;

extern Swipable * swiper;
extern TTF_Font * menuFont;
extern Map * map;
extern DrawStack * mainStack;
extern InputStack * interactionStack;
extern SDL_Rect screenBounds;
extern SDL_Renderer *globalRenderer;
extern SlugData ** archetypes;
extern TurnManager * playerTurnManager;

extern void handleTouch(float x, float y);


extern void btnAttack();

extern SDL_Texture * slug;
extern SlugController * slugController;
extern LinkedList<TurnManager> * turnManagerList;
extern Sprite * doneCheckmark;
extern SpriteSheet * slugHeadSheet;

extern void handleInput(int x, int y);

extern bool isRetina;
extern int needsDisplay;

void
render(SDL_Renderer *renderer)
{
    
    /* Clear the screen */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    mainStack->draw();
    /* update screen */
    SDL_RenderPresent(renderer);
}


int gameplayMain(){
    menuFont = TTF_OpenFont("images/mozart.ttf", 25*RETINA_FACTOR); //this opens a font style and sets a size

    int done;
    SDL_Event event;
    SlugFileSystem = new HashMap<SlugFile>();
    
    int w, h;
    int ** mapdata = loadMap(w,h);
    
    map = new Map(w, h, screenBounds.w, screenBounds.h, 32*RETINA_FACTOR, 2*RETINA_FACTOR, mapdata);
    
    
    
    mainStack = new DrawStack();
    interactionStack = new InputStack();
    InputLayer * interactionLayer = interactionStack->expandLayers();
    DrawLayer * mapLayer = mainStack->expandLayers();
    
    playerTurnManager = new TurnManager(0x1);
    TurnManager * enemyTurnManager = new TurnManager(0x2);
    
    
    mapLayer->add(map);
    interactionLayer->add(map);
    //spriteLayer->add(testSprite1);
    //spriteLayer->add(testSprite2);
    //slug = slugTexture(28, 4, 14, 255, 0, 0);
    
    
    
    slugController = new SlugController();
    slugController->playerTeamMask = 0x1;
    mapLayer->add(slugController);
    map->controller = slugController;
    SlugFile * loadedSlugs = new SlugFile(copyStr("images/test.dat"));
    if(!loadedSlugs->getIsLoaded()){
        printf("memory leak");
        return 0;
    }
    SlugFileSystem->put(loadedSlugs->filename, loadedSlugs);

    slugHeadSheet = new SpriteSheet("images/headsprites.png",28*2,28*2,28,28);
    doneCheckmark = slugHeadSheet->getSprite(0, 28*2, 20, 20, 10*RETINA_FACTOR, 10*RETINA_FACTOR);
    
    Slug* s = new Slug(28, 4, 0, 0, loadedSlugs->archetypes[0], playerTurnManager);
    s = new Slug(28, 4, 5, 6, loadedSlugs->archetypes[1], enemyTurnManager);


    playerTurnManager->newTurn();

    /*
    //loadSlugs2();
    
    doneCheckmark = slugHeadSheet->getSprite(0, 28, 10, 10);
    //slugsprites->
    playerTurnManager->addSlug(new Slug(28, 4, 0, 0, archetypes[3]));
    playerTurnManager->addSlug(new Slug(28, 4, 0, 2, archetypes[4]));
    playerTurnManager->addSlug(new Slug(28, 4, 0, 4, archetypes[5]));
    playerTurnManager->addSlug(new Slug(28, 4, 0, 6, archetypes[6]));
    playerTurnManager->addSlug(new Slug(28, 4, 0, 8, archetypes[7]));
    enemyTurnManager->addSlug(new Slug(28, 4, 5, 7, archetypes[0]));
    enemyTurnManager->addSlug(new Slug(28, 4, 5, 3, archetypes[1]));
    enemyTurnManager->addSlug(new Slug(28, 4, 5, 5, archetypes[2]));
    
    playerTurnManager->newTurn();
    */
    
    
    char * colors = new char[8];
    colors[0] = 255;
    colors[4] = 255;
    
    colors[1] = 255;
    colors[2] = 0;
    colors[3] = 0;

    colors[5] = 255;
    colors[6] = 255;
    colors[7] = 255;
    
    

    SLUG_Button * button2 = new SLUG_Button(screenBounds.w/2-246/4*RETINA_FACTOR, 20*RETINA_FACTOR, 246/2*RETINA_FACTOR, 38/2*RETINA_FACTOR, copyStr("Actions"), colors, &btnAttack);
    interactionLayer->add(button2);
    mapLayer->add(button2);
    
    
    //render(globalRenderer);
    //246 38
    
    
    swiper = map;
    /*
     
    */
    
    
    needsDisplay = 1;
    int x = 0;
    int y = 0;
    // Enter render loop, waiting for user to quit
    done = 0;
    Uint16 renderLoop = 0;
    long fingerId = -1;
    bool sliding = false;
    bool isHandlingTick = true;
    while (!done) {
        
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                done = 1;
            }
        }
        if(event.type == SDL_FINGERDOWN){
            SDL_TouchFingerEvent touchEvent = event.tfinger;
            fingerId = touchEvent.fingerId;
            x = touchEvent.x*screenBounds.w;
            y = touchEvent.y*screenBounds.h;
            sliding = false;
        }else if(event.type == SDL_FINGERMOTION){
            
            SDL_TouchFingerEvent touchEvent = event.tfinger;
            if (touchEvent.fingerId == fingerId) {
                int nx = touchEvent.x*screenBounds.w;
                int ny = touchEvent.y*screenBounds.h;
                if(sliding || abs(nx - x) + abs(ny - y) > 10){
                    if(nx != x || ny !=y){
                        needsDisplay = 1;
                        isHandlingTick = true;
                    }
                    swiper->addOffsetX(nx - x);
                    swiper->addOffsetY(ny - y);
                    x = nx;
                    y = ny;
                    sliding = true;
                }
            }else if(touchEvent.fingerId != -1){
                touchEvent.fingerId = fingerId;
                x = touchEvent.x*screenBounds.w;
                y = touchEvent.y*screenBounds.h;
                sliding = true;
            }
        }else if (event.type == SDL_FINGERUP){
            needsDisplay = 1;
            if(sliding){
                sliding = false;
                fingerId = -1;
            }else{
                SDL_TouchFingerEvent touchEvent = event.tfinger;
                if (fingerId != -1) {
                    isHandlingTick = true;
                    fingerId = -1;
                    handleTouch(touchEvent.x,touchEvent.y);
                }
            }
        }
        
        if(needsDisplay && isHandlingTick){
            needsDisplay--;
            if(needsDisplay <= 0){
                isHandlingTick = false;
                needsDisplay = 0;
            }
            printf("rendering %i\n",renderLoop++);
            render(globalRenderer);
        }else{
            //SDL_RenderPresent(globalRenderer);

        }
        if(sliding){
            SDL_Delay(20);
        }else{
            SDL_Delay(16);
        }
        
    }
    //delete testMenu;
    SlugFileSystem->drain();
    delete playerTurnManager; // auto deletes turn manager list
    delete enemyTurnManager;
    delete SlugFileSystem;
    delete mainStack;
    delete button2;
    mainStack = NULL;
    delete interactionStack;
    interactionStack = NULL;
    SDL_free(menuFont);
    return done;
}