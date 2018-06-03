
//
//  Hashmap.cpp
//  SLUG
//
//  Created by Steph on 7/12/16.
//
//

#include "Hashmap.hpp"

unsigned long stringHash(const char * str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash;
}