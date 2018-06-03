//
//  Map.h
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#ifndef __SLUG__Map__
#define __SLUG__Map__

#include <stdio.h>
#include <vector>
#include "SDL.h"
#include "SlugSegment.h"
#include "SpriteSheet.h"
#include "Drawable.h"
#include "Swipable.hpp"
#include "Interactable.h"
#include "QuadTree.hpp"
extern bool isRetina;
extern SDL_Renderer * globalRenderer;
extern SDL_Rect screenBounds;


extern int needsDisplay;

class SlugController;

const int MAP_TILE_NON_EXISTENT = 0x2;
const int MAP_TILE_UNKNOWN = 0x0;
const int MAP_TILE_EMPTY = 0x4;
const int MAP_TILE_OUT_OF_BOUNDS = 0x3;
const int MAP_TILE_OCCUPIED_SELF = 0x8;
const int MAP_TILE_OCCUPIED_OTHER = 0x10;

class Map : public Drawable, public Interactable, public Swipable{
private:
    SpriteSheet * backgroundSheet;
    /** the objects (slug segments) at each point of the map */
    SlugSegment *** objects;
    /** where the map displays on the screen, also its hitbox */
    SDL_Rect rect;
    SDL_Rect drawRect;
    int ** mapSquares;
    int offsetX, offsetY;
public:
    int width, height, size, tileSize, inset;
    void addOffsetX(int delta);
    void addOffsetY(int delta);
    /** static background */
    Sprite * background;
    /** constructor 
     @param width The width of the screen the map is to be displayed to
     @param height The height of the screen the map is to be displayed to
     @param tileSize The square size of the tiles to be displayed
     @param inset How much should the tiles be erroded?
     */
    Map(int tileWidth, int tileHeight, int width, int height, int tileSize, int inset, int ** mapSquares);
    /** destructor */
    ~Map();
    /** 
     get the segment at a position, NULL if or not found 
     @param x The x coord. in map tile coordiantes.
     @param y The y coord. in map tile coordiantes.
     */
    SlugSegment * getSlug(int x, int y);
    /** 
     one controller which switches around to allow the user to control
     slugs and to illustrate enemy slug movement
     */
    SlugController * controller;
    Slug * focusSlug;
    /** 
     set the segment at a position, NULL if deleting 
     @param x The x coord. in map tile coordiantes.
     @param y The y coord. in map tile coordiantes.
     */
    void setSlug(SlugSegment * slug, int x, int y);
    /** 
     is a point (in the map's tile coordinate system) within bounds
     @param x The x coord. in map tile coordiantes.
     @param y The y coord. in map tile coordiantes.
     */
    bool pointValid(int x, int y);
    /** draw the map */
    void draw();
    /** update the map */
    void update();
    /** 
     map coordinates to drawing coordinates 
     @param x The x coord. in map tile coordiantes.
     @param y The y coord. in map tile coordiantes.
     @return x The x coord. in pixel coordiantes.
     @return y The y coord. in pixel tile coordiantes.
     */
    void pointToDraw(int &x, int &y);
    /** 
     Check what's happening at an x or y coordinate.
     @param x The x coord. in map tile coordiantes.
     @param y The y coord. in map tile coordiantes.
     @return does a space have a slug on it, exist, or is it occupied by the inquisitor? 
     */
    int isOccupied(int x, int y, Slug * inquisitor);
    /** used for input collision box */
    int getX();
    /** used for input collision box */
    int getY();
    /** used for input collision box */
    int getWidth();
    /** used for input collision box */
    int getHeight();
    /** 
     on input collision, what should we do (converts into map
     tile coords. and passes to recipient).
     @param x The x coord. in pixel coordiantes.
     @param y The y coord. in pixel tile coordiantes.
     @return wheter we have captured the touch or not
     */
    bool onInput(int x, int y);
    
    void nextTurn();
};

#endif /* defined(__SLUG__Map__) */
