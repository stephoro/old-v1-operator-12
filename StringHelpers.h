//
//  StringHelpers.h
//  SLUG
//
//  Created by Steph on 3/19/16.
//
//

#ifndef __SLUG__StringHelpers__
#define __SLUG__StringHelpers__

#include <stdio.h>
#include <string>

char * copyStr(std::string * str);
char * copyStr(const char * str);
int parseInt(std::string * str);

#endif /* defined(__SLUG__StringHelpers__) */
