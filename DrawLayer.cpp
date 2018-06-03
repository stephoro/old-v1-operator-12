//
//  DrawLayer.cpp
//  SLUG
//
//  Created by Steph on 3/3/16.
//
//

#include "DrawLayer.h"



DrawLayer::DrawLayer(bool visible, bool updates, bool renderAfter, bool updateAfter) {
    this->objects = new LinkedList<Drawable>();
    this->visible = visible;
    this->updates = updates;
    this->renderAfter = renderAfter;
    this->updateAfter = updateAfter;
    position = NULL;
}

DrawLayer::~DrawLayer(){
    delete objects;
}


void DrawLayer::add(Drawable *object){
    object->drawingLayer = this;
    object->drawingLayerRef = objects->add(object);
}

void DrawLayer::remove(Drawable *object){
    objects->remove(object->drawingLayerRef);
}

void DrawLayer::draw(){
    objects->refocus();
    int size = objects->getSize();
    Drawable * focus;
    while (size-- > 0) {
        focus = objects->next();
        focus->draw();
    }
}

void DrawLayer::update(){
    objects->refocus();
    int size = objects->getSize();
    Drawable * focus;
    while (size-- > 0) {
        focus = objects->next();
        focus->update();
    }
}

bool DrawLayer::shouldUpdateAfter(){
    return updateAfter;
}

bool DrawLayer::shouldRenderAfter(){
    return renderAfter;
}

bool DrawLayer::shouldRender(){
    return visible;
}

bool DrawLayer::shouldUpdate(){
    return updates;
}



