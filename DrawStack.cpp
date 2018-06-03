//
//  DrawStack.cpp
//  SLUG
//
//  Created by Steph on 3/11/16.
//
//

#include "DrawStack.h"

DrawStack::DrawStack(){
    layers = new LinkedList<DrawLayer>();
};

DrawStack::~DrawStack(){
    layers->drain();
    delete layers;
}

void DrawStack::draw(){
    layers->refocus();
    int layersLeft = layers->getSize();
    DrawLayer * layer;
    while (layersLeft-- > 0) {
        layer = layers->next();
        if(layer->shouldRender())
            layer->draw();
        if(!layer->shouldRenderAfter())
            layersLeft = 0;
    }
}

void DrawStack::update(){
    layers->refocus();
    int layersLeft = layers->getSize();
    DrawLayer * layer;
    while (layersLeft-- > 0) {
        layer = layers->next();
        if(layer->shouldUpdate())
            layer->update();
        if(!layer->shouldUpdateAfter())
            layersLeft = 0;
    }
}


DrawLayer * DrawStack::expandLayers (bool visible, bool updates, bool renderAfter, bool updateAfter){

    DrawLayer * drawlayer = new DrawLayer(visible, updates, renderAfter, updateAfter);
    drawlayer->position = layers->add(drawlayer);
    return drawlayer;
}

bool DrawStack::onInput(int x, int y){
    layers->refocus();
    int layersLeft = layers->getSize();
    DrawLayer * layer;
    while (layersLeft-- > 0) {
        layer = layers->next();
        if(layer->shouldUpdate())
            layer->update();
        if(!layer->shouldUpdateAfter())
            layersLeft = 0;
    }
    return false;
}