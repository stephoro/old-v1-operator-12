//
//  DrawStack.h
//  SLUG
//
//  Created by Steph on 3/11/16.
//
//

#ifndef __SLUG__DrawStack__
#define __SLUG__DrawStack__

#include <stdio.h>
#include "LinkedList.hpp"
#include "DrawLayer.h"


class DrawStack{
private:
    LinkedList<DrawLayer> * layers;
public:
    DrawStack();
    ~DrawStack();
    /**
     Adds a new layer to the set of current layers.
     */
    DrawLayer * expandLayers (bool visible = true, bool updates = true, bool renderAfter = true, bool updateAfter = true);
    
    
    /**
     Draw content by iteration of layers.
     */
    void draw();
    
    /**
     Update content by iteration of layers.
     */
    void update();
    
    /**
     pass an input event
     */
    bool onInput(int x, int y);
};

#endif /* defined(__SLUG__DrawStack__) */
