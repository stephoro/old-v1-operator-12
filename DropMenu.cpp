//
//  DropMenu.cpp
//  SLUG
//
//  Created by Steph on 7/10/16.
//
//

#include "DropMenu.hpp"
#include "LoadFont.hpp"


DropMenu::DropMenu(int length, char ** items, char * colors, char * fgcolors, DropMenuDelegate * delegate){
    this->length = length;
    this->items = items;
    this->colors = colors;
    this->fgcolors = fgcolors;
    this->delegate = delegate;
    deltaY = 0;
    texts = NULL;
    needsUpdate = false;
    textRects = NULL;
}

DropMenu::~DropMenu(){
    if(items != NULL){
        for(int i = 0; i < length; i++)
            delete [] items[i];
        delete [] items;
    }
    if(colors != NULL)
        delete [] colors;
    if(fgcolors != NULL)
        delete [] fgcolors;
    if(texts != NULL){
        for(int i = 0; i < length; i++)
            SDL_DestroyTexture(texts[i]);
        delete [] texts;
    }
    if(textRects != NULL){
        for(int i = 0; i < length; i++)
            delete textRects[i];
        delete [] textRects;
    }
}


void DropMenu::draw(){
    SDL_SetRenderDrawBlendMode(globalRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 128);
    SDL_RenderFillRect(globalRenderer, &screenBounds);
    SDL_Rect rect = SDL_Rect();
    rect.x = menuXOffset;
    rect.y = 0;
    rect.w = itemWidth;
    rect.h = itemHeight;
    
    for(int c = 0; c < length; c++){
        int b = c*4;
        SDL_SetRenderDrawColor(globalRenderer, colors[b + 1], colors[b + 2], colors[b + 3], colors[b]);
        rect.y = menuYOffset + c * (itemYOffset + itemHeight) - deltaY;//FUNSIES, move '- deltaY' inside parens
        SDL_RenderFillRect(globalRenderer, &rect);
        SDL_Rect * source =textRects[c];
        SDL_Rect * drawRect = new SDL_Rect();
        drawRect->x = menuXOffset + (itemWidth - source->w)/2;
        drawRect->y = rect.y + (itemHeight - source->h)/2;
        drawRect->w = source->w;
        drawRect->h = source->h;
        SDL_RenderCopy(globalRenderer, texts[c], source, drawRect);
    }
}

void DropMenu::update(){
    if(needsUpdate){
        printf("updating\n");
        needsUpdate = false;
        if(texts != NULL){
            for(int i = 0; i < length; i++)
                SDL_DestroyTexture(texts[i]);
            delete [] texts;
        }
        if(textRects != NULL){
            for(int i = 0; i < length; i++)
                delete textRects[i];
            delete [] textRects;
        }
        texts = new SDL_Texture*[length];
        textRects = new SDL_Rect*[length];
        for(int i = 0; i < length; i++){
            int b = i*4;
            SDL_Color fg = SDL_Color();
            fg.a = fgcolors[b];
            fg.r = fgcolors[b+1];
            fg.g = fgcolors[b+2];
            fg.b = fgcolors[b+3];
            int w,h;
            textRects[i] = new SDL_Rect();
            textRects[i]->x = 0;
            textRects[i]->y = 0;
            printf("%i: %s\n",i,items[i]);
            texts[i] = textureFromUnicode(items[i], menuFont, fg, w, h);
            textRects[i]->w=w;
            textRects[i]->h=h;
        }
    }
}

void DropMenu::addOffsetX(int delta){
    
}
void DropMenu::addOffsetY(int delta){
    delta/=2;
    deltaY -= delta;
    if(deltaY < 0){
        deltaY = 0;
    }else if(deltaY > (length-1) * (itemYOffset + itemHeight)){
        deltaY = (length-1) * (itemYOffset + itemHeight);
    }
}


int DropMenu::getX(){
    return screenBounds.x;
}

int DropMenu::getY(){
    return screenBounds.y;
}

int DropMenu::getWidth(){
    return screenBounds.w;
}

int DropMenu::getHeight(){
    return screenBounds.h;
}

bool DropMenu::onInput(int x, int y){
    x -= menuXOffset;
    y -= menuYOffset - deltaY;
    if(x > 0 && x < itemWidth && y > 0){
        int modY = y % (itemHeight + itemYOffset);
        if(modY <= itemHeight){
            int divY = y / (itemHeight + itemYOffset);
            printf("in choice %i\n",divY);
            if(delegate != NULL)
                delegate->onChoice(divY);
        }
    }
    return true;
}
