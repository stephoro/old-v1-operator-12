//
//  LoadFont.cpp
//  SLUG
//
//  Created by Steph on 7/11/16.
//
//

#include "LoadFont.hpp"

Uint16 * charTo16(const char * text){
    int i = 0;
    
    Uint16 * retText = new Uint16[ 1];
    //printf("code %i: ",size);
    while (text[i]) {
        //retText[j++] = ((text[i]&0xFF)<<8) + (text[i+1]&0xFF);
        printf("%x | ",text[i]);
        
        i++;
        //size --;
    }
    //retText[j++] = 0;
    printf("\n");
    return retText;
}

SDL_Texture* textureFromText(char * text, TTF_Font * font, SDL_Color fg, int &w, int &h){
    //Get rid of preexisting texture
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Render text to surface
    SDL_Surface* loadedSurface = TTF_RenderUTF8_Blended(font, text, fg);
    
    if( loadedSurface == NULL )
    {
        SDL_Log( "Unable to render font %s! SDL Error: %s\n", text, SDL_GetError() );
    }
    else
    {
        
        w = loadedSurface->w;
        h = loadedSurface->h;
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( globalRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", text, SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    return newTexture;
}

SDL_Texture* textureFromGlyph(Uint16 glyph, TTF_Font * font, SDL_Color fg, int &w, int &h){
    //Get rid of preexisting texture
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Render text to surface
    SDL_Surface* loadedSurface = TTF_RenderGlyph_Blended(font, glyph, fg);
    
    if( loadedSurface == NULL )
    {
        SDL_Log( "Unable to render font %i! SDL Error: %s\n", glyph, SDL_GetError() );
    }
    else
    {
        
        w = loadedSurface->w;
        h = loadedSurface->h;
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( globalRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %i! SDL Error: %s\n", glyph, SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    return newTexture;
}

SDL_Texture* textureFromUnicode(char * text, TTF_Font * font, SDL_Color fg, int &w, int &h){
    //Get rid of preexisting texture
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Render text to surface
    SDL_Surface* loadedSurface = TTF_RenderUTF8_Blended(font, ( text ), fg);
    
    if( loadedSurface == NULL )
    {
        SDL_Log( "Unable to render font! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        
        w = loadedSurface->w;
        h = loadedSurface->h;
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( globalRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from! SDL Error: %s\n", SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    return newTexture;
}