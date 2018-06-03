//
//  LoadFont.hpp
//  SLUG
//
//  Created by Steph on 7/11/16.
//
//

#ifndef LoadFont_hpp
#define LoadFont_hpp
#include "SDL.h"
#include <stdio.h>
#include <string>
#include "SDL_ttf.h"
extern SDL_Renderer * globalRenderer;
SDL_Texture* textureFromText(char * text, TTF_Font * font, SDL_Color fg, int &w, int &h);
SDL_Texture* textureFromUnicode(char * text, TTF_Font * font, SDL_Color fg, int &w, int &h);
SDL_Texture* textureFromGlyph(Uint16 glyph, TTF_Font * font, SDL_Color fg, int &w, int &h);
Uint16 * charTo16(const char * text);

#endif /* LoadFont_hpp */
