//
//  CharArray.hpp
//  SLUG
//
//  Created by Steph on 6/28/16.
//
//

#ifndef CharArray_hpp
#define CharArray_hpp
#define getNthBool(b,n) (b&(1<<(7-n)))

#include <stdio.h>
#include "SDL.h"
class CharArray {
private:
    int readHead;
    char * buffer;
    bool ownsBuffer;
public:
    CharArray(char * buffer, int readHead = 0);
    ~CharArray();
    char * getBuffer();
    char * getString();
    void rollBack();
    void rollBackBy(int amount);
    Uint32 get32bits();
    Uint32 get24bits();
    Uint16 get16bits();
    Uint8 get8bits();
};


#endif /* CharArray_hpp */
