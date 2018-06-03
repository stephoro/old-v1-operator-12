//
//  SLUG_Button.cpp
//  SLUG
//
//  Created by Steph on 3/20/16.
//
//

#include "SLUG_Button.h"
#include "LoadFont.hpp"
extern float RETINA_FACTOR;
extern TTF_Font * menuFont;
SLUG_Button::SLUG_Button(int x, int y, int w, int h, char * name, char * colors, void (*targetFun)()){
    myFunc = targetFun;
    
    this->name = name;
    this->colors = colors;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    rect = new SDL_Rect();
    textureFillRect = new SDL_Rect();
    textureSourceRect = new SDL_Rect();
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    SDL_Color fg = SDL_Color();
    fg.r = colors[5];
    fg.g = colors[6];
    fg.b = colors[7];
    fg.a = colors[4];
    texture = textureFromUnicode(name, menuFont, fg, tw, th);
    textureFillRect->x = x + (w-tw)/2;
    textureFillRect->y = y + (h-th)/2;
    textureFillRect->w = tw;
    textureFillRect->h = th;
    textureSourceRect->x=0;
    textureSourceRect->y=0;
    textureSourceRect->w = tw;
    textureSourceRect->h = th;
    
}

SLUG_Button::~SLUG_Button(){
    delete rect;
    delete textureFillRect;
    delete textureSourceRect;
    delete [] name;
    delete [] colors;
    SDL_DestroyTexture(texture);
}

void SLUG_Button::draw(){
    SDL_SetRenderDrawColor(globalRenderer, colors[1], colors[2], colors[3], colors[0]);
    SDL_RenderFillRect(globalRenderer, rect);
    SDL_RenderCopy(globalRenderer, texture, textureSourceRect, textureFillRect);
}

void SLUG_Button::update(){
    
}

bool SLUG_Button::onInput(int x, int y){
    myFunc();
    return true;
}

int SLUG_Button::getHeight(){
    return h;
}
int SLUG_Button::getWidth(){
    return w;
}
int SLUG_Button::getX(){
    return x;
}
int SLUG_Button::getY(){
    return y;
}