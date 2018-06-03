//
//  Interactable.h
//  SLUG
//
//  Created by Steph on 3/13/16.
//
//

#ifndef __SLUG__Interactable__
#define __SLUG__Interactable__

#include <stdio.h>
#include "LinkedList.hpp"

class InputLayer;
class Interactable{
public:
    InputLayer * interactionLayer;
    LinkedList<Interactable>::Node<Interactable> * interactionLayerRef;
    
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual bool onInput(int x, int y) = 0;
};

#endif /* defined(__SLUG__Interactable__) */
