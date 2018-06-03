//
//  InputStack.h
//  SLUG
//
//  Created by Steph on 3/13/16.
//
//

#ifndef __SLUG__InputStack__
#define __SLUG__InputStack__

#include <stdio.h>
#include "InputLayer.h"

class InputStack{
private:
    LinkedList<InputLayer> * layers;
public:
    InputStack();
    ~InputStack();
    /**
     Adds a new layer to the set of current layers.
     */
    InputLayer * expandLayers (bool interactAfter = true);
    
    /**
     pass an input event
     */
    bool onInput(int x, int y);
};

#endif /* defined(__SLUG__InputStack__) */
