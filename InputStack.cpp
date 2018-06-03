//
//  InputStack.cpp
//  SLUG
//
//  Created by Steph on 3/13/16.
//
//

#include "InputStack.h"


InputStack::InputStack(){
    layers = new LinkedList<InputLayer>();
}

InputStack::~InputStack(){
    layers->drain();
    delete layers;
}

bool InputStack::onInput(int x, int y){
    layers->refocus();
    int layersLeft = layers->getSize();
    InputLayer * layer;
    while (layersLeft-- > 0) {
        layer = layers->next();
        if(layer->onInput(x, y))
            return true;
        if(!layer->shouldInteractAfter())
            return true;
    }
    return false;
}


InputLayer * InputStack::expandLayers (bool interactAfter){
    
    InputLayer * inputlayer = new InputLayer(interactAfter);
    inputlayer->position = layers->addToFront(inputlayer);
    return inputlayer;
}
