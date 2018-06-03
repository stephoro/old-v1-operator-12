//
//  TextureLoader.h
//  SLUG
//
//  Created by Steph on 3/4/16.
//
//

#ifndef __SLUG__TextureLoader__
#define __SLUG__TextureLoader__

#include <stdio.h>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
extern SDL_Renderer *globalRenderer;
SDL_Texture* loadTexture( std::string path );
SDL_Texture* slugTexture(int size, int expansionInset, int expansionSize, int r, int g, int b);

#endif /* defined(__SLUG__TextureLoader__) */
