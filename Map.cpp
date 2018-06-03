//
//  Map.cpp
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#include "Map.h"
#include "SlugController.h"

extern float RETINA_FACTOR;

int Map::getX(){
    return drawRect.x;
}

int Map::getY(){
    return drawRect.y;
}

int Map::getWidth(){
    return drawRect.w;
}

int Map::getHeight(){
    return drawRect.h;
}

void Map::addOffsetX(int delta){
    offsetX += delta;

    if(offsetX > screenBounds.w - tileSize*2)
        offsetX = screenBounds.w - tileSize*2;
    if(-offsetX > (width - 2) * (tileSize))
        offsetX = -(width - 2) * (tileSize);

}

void Map::addOffsetY(int delta){
    offsetY += delta;
    if(offsetY > screenBounds.h - tileSize*2)
        offsetY = screenBounds.h - tileSize*2;
    if(-offsetY > (height-2) * (tileSize))
        offsetY = -(height-2) * (tileSize);
    
    
}

Map::~Map(){
    delete background;
    delete backgroundSheet;
    for(int x = 0; x < width; x++){
        delete [] objects[x];
    }
    delete [] objects;
}

Map::Map(int tileWidth, int tileHeight, int width, int height, int tileSize, int inset, int ** mapSquares){
    focusSlug = NULL;
    int w = width;
    int h = height;
    drawRect.x = 0;
    drawRect.y = 0;
    drawRect.w = width;
    drawRect.h = height;
    width = tileWidth;
    height = tileHeight;
    offsetY = (h - height*tileSize)/2;
    offsetX = (w - width*tileSize)/2;
    
    
    this->mapSquares = mapSquares;
    
    
    
    this->inset = inset;
    this->tileSize = tileSize;
    this->width = width;
    this->height = height;
    this->size = width*height;
    
    objects = new SlugArray[width];
    for(int x = 0; x < width; x++){
        SlugSegment ** row = objects[x] = new SlugSegment*[height];
        for(int y = 0; y < height; y++){
            row[y] = NULL;
        }
    }
    
    
    printf("drawrect: %i %i\n",drawRect.w,drawRect.h);
    backgroundSheet = new SpriteSheet("images/Matrix.png", drawRect.w, drawRect.h, w, h);
    background = backgroundSheet->getSprite(0, 0, drawRect.w, drawRect.h, drawRect.w, drawRect.h);
    printf("tilesize %i\n",tileSize);
    rect.w = tileSize - inset*2;
    rect.h = tileSize - inset*2;
    
    
}

bool Map::pointValid(int x, int y){
    return x >= 0 && y >= 0 && x < width && y < height;
}

SlugSegment * Map::getSlug(int x, int y){
    if(!pointValid(x, y))
        return NULL;
    return objects[x][y];
}

void Map::setSlug(SlugSegment *slug, int x, int y){
    if(pointValid(x, y)){
        if(objects[x][y] != NULL){
            delete objects[x][y];
        }
        objects[x][y] = slug;
    }
}

void Map::nextTurn(){
    focusSlug = NULL;
    controller->setSlug(NULL);
}

void Map::draw(){
    
    int basex = inset + offsetX;
    int basey = inset + offsetY;
    
    int insetx = basex, insety = basey, tileSize2 = tileSize;
    

    SDL_SetRenderDrawBlendMode(globalRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 128);
    
    //SDL_RenderCopy(globalRenderer, background, NULL, &drawRect);
    background->draw(0, 0);
    
    int stx = -basex/tileSize2;
    int sty = -basey/tileSize2;
    int edx = stx + screenBounds.w/tileSize2 + 1;
    int edy = sty + screenBounds.h/tileSize2 + 1;
    if(edx > width) edx = width;
    if(edy > height) edy = height;
    if(stx < 0) stx = 0;
    if(sty < 0) sty = 0;
    for(int x = stx; x < edx; x++){
        
        for(int y = sty; y < edy; y++){
            rect.x = x*tileSize2 + insetx;
            rect.y = y*tileSize2 + insety;
            if(rect.x < -rect.w || rect.y < -rect.h || rect.x > screenBounds.w || rect.y > screenBounds.h){
            }else{
                if(objects[x][y] != NULL){
                    objects[x][y]->draw(basex + x*tileSize2, basey + y*tileSize2);
                }else if(mapSquares[x][y]){
                    
                    SDL_RenderFillRect(globalRenderer, &rect);
                }
            }
        }
    }
}

void Map::pointToDraw(int &x, int &y){
    x = drawRect.x/2 + x*tileSize + offsetX;
    y = drawRect.y/2 + y*tileSize + offsetY;
}

void Map::update(){
    //Not called ever
}

int Map::isOccupied(int x, int y, Slug *inquisitor){
    bool isValid = pointValid(x, y);
    if(!isValid)
        return MAP_TILE_OUT_OF_BOUNDS;
    if(!mapSquares[x][y]){
        return MAP_TILE_NON_EXISTENT;
    }
    SlugSegment * atPoint = objects[x][y];
    if (atPoint == NULL) {
        return MAP_TILE_EMPTY; //no one is here
    }else if(atPoint->getOwner() == inquisitor){
        return MAP_TILE_OCCUPIED_SELF; // we are here
    }
    return MAP_TILE_OCCUPIED_OTHER; // somebody else is here
}

bool Map::onInput(int x, int y){
    int tSize = tileSize;
    
    if(x - drawRect.x - offsetX < 0 || y - drawRect.y - offsetY < 0)
        return false;
    int myX = (x - drawRect.x - offsetX) / tSize;
    int myY = (y - drawRect.y - offsetY) / tSize;
    if(!pointValid(myX, myY))
        return false;

    if(focusSlug == NULL){
        SlugSegment * segment = objects[myX][myY];
        if(segment != NULL){
            focusSlug = segment->getOwner();
            controller->setSlug(focusSlug);
        }
    }else{
        bool handled = controller->handlesInput(myX, myY);
        if(!handled){
            SlugSegment * segment = objects[myX][myY];
            if(segment != NULL){
                focusSlug = segment->getOwner();
                controller->setSlug(focusSlug);
            }
        }
    }
    
    
    
    return true;
}

