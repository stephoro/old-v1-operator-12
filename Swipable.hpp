//
//  Swipable.hpp
//  SLUG
//
//  Created by Steph on 7/10/16.
//
//

#ifndef Swipable_hpp
#define Swipable_hpp

#include <stdio.h>

class Swipable {
    
public:
    virtual void addOffsetX(int delta) = 0;
    virtual void addOffsetY(int delta) = 0;
};


#endif /* Swipable_hpp */
