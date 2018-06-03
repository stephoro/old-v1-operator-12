//
//  Drawable.h
//  SLUG
//
//  Created by Steph on 3/3/16.
//
//

#ifndef __SLUG__Drawable__
#define __SLUG__Drawable__

#include "LinkedList.hpp"
#include <stdio.h>

class DrawLayer;
class Drawable{
public:
    DrawLayer * drawingLayer;
    LinkedList<Drawable>::Node<Drawable> * drawingLayerRef;
    
    virtual void draw() = 0;
    
    virtual void update() = 0;
    
};
#endif /* defined(__SLUG__Drawable__) */
