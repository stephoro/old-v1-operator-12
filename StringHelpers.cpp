//
//  StringHelpers.cpp
//  SLUG
//
//  Created by Steph on 3/19/16.
//
//

#include "StringHelpers.h"

char * copyStr(std::string * str){
    int len = (int)str->length();
    char * ret = new char[len + 1];
    for (int i = len - 1; i >= 0; i--) {
        ret[i] = str->at(i);
    }
    ret[len] = '\0';
    return ret;
}

char * copyStr(const char * str){
    int len = (int)std::strlen(str);
    //printf("len of '%s' is %i\n",str,len);
    char * ret = new char[len + 1];
    for (int i = len - 1; i >= 0; i--) {
        ret[i] = str[i];
    }
    ret[len] = '\0';
    //printf("len of ret '%s' is %i\n",ret,(int)strlen(ret));
    return ret;
}


int parseInt(std::string * str){
    int sum = 0;
    int pow = 1;
    bool negate = false;
    for (int i = (int)str->length() - 1; i >= 0; i--) {
        char at = str->at(i);
        if (at == '-') {
            negate = true;
        }else{
            sum += (at-'0')*pow;
            pow*=10;
        }
    }
    if (negate) {
        return -sum;
    }
    return sum;
}