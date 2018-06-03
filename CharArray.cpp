//
//  CharArray.cpp
//  SLUG
//
//  Created by Steph on 6/28/16.
//
//

#include "CharArray.hpp"

CharArray::CharArray(char * buffer, int readHead){
    this->readHead = readHead;
    this->buffer = buffer;
    ownsBuffer = true;
}

CharArray::~CharArray(){
    if(ownsBuffer)
        delete [] buffer;
}

char * CharArray::getBuffer(){
    return buffer;
}

char * CharArray::getString(){
    int at = readHead;
    while (buffer[at] != '\0') {
        at++;
    }
    at++;
    char * str = new char[at-readHead];
    int i = 0;
    
    while(readHead < at){
        str[i++] = buffer[readHead++];
    }
    return str;
}
void CharArray::rollBack(){
    readHead = 0;
}
void CharArray::rollBackBy(int amount){
    readHead -= amount;
    if(readHead < 0)
        readHead = 0;
}
Uint32 CharArray::get32bits(){
    Uint32 ret = ((buffer[readHead] & 0xFF) << 24) |
    ((buffer[readHead + 1] & 0xFF) << 16) |
    ((buffer[readHead + 2] & 0xFF) << 8) |
    (buffer[readHead + 3] & 0xFF);
    readHead += 4;
    return ret;
}

Uint32 CharArray::get24bits(){
    Uint32 ret = ((buffer[readHead] & 0xFF) << 16) |
    ((buffer[readHead + 1] & 0xFF) << 8) |
    (buffer[readHead + 2] & 0xFF);
    readHead += 3;
    return ret;
}
Uint16 CharArray::get16bits(){
    Uint16 ret = ((buffer[readHead] & 0xFF) << 8) |
    (buffer[readHead + 1] & 0xFF);
    readHead += 2;
    return ret;
}
Uint8 CharArray::get8bits(){
    return (buffer[readHead++] & 0xFF);
}