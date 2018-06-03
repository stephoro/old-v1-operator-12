//
//  TextureLoader.cpp
//  SLUG
//
//  Created by Steph on 3/4/16.
//
//

#include "TextureLoader.h"


SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( globalRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return newTexture;
}

SDL_Texture* slugTexture(int size, int expansionInset, int expansionSize, int r, int g, int b){
    int totalSize = size + expansionInset;
    int inset = (totalSize+expansionInset-expansionSize)/2-expansionInset;
    SDL_Texture* newTexture = SDL_CreateTexture(globalRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, totalSize, totalSize);
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
    SDL_Texture* savedTarget = SDL_GetRenderTarget(globalRenderer);
    SDL_SetRenderTarget(globalRenderer, newTexture);
    SDL_SetRenderDrawColor(globalRenderer, 0,0,0,0);
    SDL_RenderClear(globalRenderer);
    //SDL_SetRenderDrawBlendMode(globalRenderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(globalRenderer, r, g, b, 255);
    SDL_Rect rect;
    rect.w = expansionInset;
    rect.h = expansionSize;
    rect.x = totalSize-expansionInset;
    rect.y = inset;
    SDL_RenderFillRect(globalRenderer, &rect);
    rect.w = expansionSize;
    rect.h = expansionInset;
    rect.x = inset;
    rect.y = totalSize-expansionInset;
    SDL_RenderFillRect(globalRenderer, &rect);

    rect.w = size;
    rect.h = size;
    rect.x = 0;
    rect.y = 0;
    SDL_RenderFillRect(globalRenderer, &rect);

    SDL_SetRenderTarget(globalRenderer, savedTarget);

    return newTexture;
}


