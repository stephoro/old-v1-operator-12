//
//  InputLayer.cpp
//  SLUG
//
//  Created by Steph on 3/13/16.
//
//

#include "InputLayer.h"




InputLayer::InputLayer(bool interactAfter) {
    objects = new LinkedList<Interactable>();
    this->interactAfter = interactAfter;
    position = NULL;
}

InputLayer::~InputLayer(){
    delete objects;
}

void InputLayer::add(Interactable *object){
    object->interactionLayer = this;
    object->interactionLayerRef = objects->addToFront(object);
}

void InputLayer::remove(Interactable *object){
    objects->remove(object->interactionLayerRef);
}


bool InputLayer::onInput(int x, int y){
    objects->refocus();
    int size = objects->getSize();
    Interactable * focus;
    while (size-- > 0) {
        focus = objects->next();
        int ox = focus->getX();
        int oy = focus->getY();
        int width = focus->getWidth();
        int height = focus->getHeight();
        if(x >= ox && x <= ox + width && y >= oy && y <= oy + height && focus->onInput(x, y)){
            return true;
        }
    }
    return !interactAfter;
}

bool InputLayer::shouldInteractAfter(){
    return interactAfter;
}

