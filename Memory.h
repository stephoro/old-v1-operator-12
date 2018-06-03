//
//  Memory.h
//  SLUG
//
//  Created by Steph on 3/20/16.
//
//

#ifndef __SLUG__Memory__
#define __SLUG__Memory__

#include <stdio.h>
#include <string>
#include "StringHelpers.h"
#include "FileEditor.h"
#include "SpriteSheet.h"
#include "Slug.h"
#include "SlugAttack.h"
#include "SlugData.h"
#include "CharArray.hpp"
#include "SpriteSheet.h"
#include "Hashmap.hpp"
class SlugData;
extern SpriteSheet * slugHeadSheet;

int ** loadMap(int &width, int &height);
void loadSlugs();

struct MemoryStruct {
    SLUG * slug;
    SlugAction * action;
    bool isMultiAction;
    Uint32 rowNumber;
    LinkedList<SLUG> * slugs;
};

class SlugFile {
private:
    MemoryStruct * memory;
    char * raw_data;
    CharArray * proto;
    bool isLoaded;
    SpriteSheet * slugHeadSheet;
    int numberOfSlugs;
    SpriteSheet * sheet;
public:
    HashMap<SlugData> * archetypesHash;
    char * filename;
    SlugData ** archetypes;
    SlugFile(char * filename);
    ~SlugFile();
    void freeSLUG(SLUG * toFree);
    void freeMemoryStruct(MemoryStruct * toFree);
    bool getIsLoaded();
};





#endif /* defined(__SLUG__Memory__) */
