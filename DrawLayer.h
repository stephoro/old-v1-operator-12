//
//  DrawLayer.h
//  SLUG
//
//  Created by Steph on 3/11/16.
//
//

#ifndef __SLUG__DrawLayer__
#define __SLUG__DrawLayer__

#include <stdio.h>

#include "LinkedList.hpp"
#include "Drawable.h"
#include "SDL.h"

extern SDL_Renderer * globalRenderer;
/**
 A layer whose objects can be drawn.
 @author Steph
 */
class DrawLayer{
private:
    
    /** The objects to be drawn. */
    LinkedList<Drawable>  * objects;
    /** If the layer is to be rendered. */
    bool visible;
    /** If the layer should be updated. */
    bool updates;
    /** If the following layers should be rendered. */
    bool renderAfter;
    /** If the following layers should be updated. */
    bool updateAfter;
    
public:
    /** Reference to where the layer is stored within the DrawStack's linked list */
    LinkedList<DrawLayer>::Node<DrawLayer> * position;
    /**
     Construct a DrawLayer
     @param visible Should the layer be rendered?
     @param updates Should the draw layer update?
     @param renderAfter Should any subsequent layer render?
     @param updateAfter Should any subsequent layer update?
     */
    DrawLayer(bool visible = true, bool updates = true, bool renderAfter = true, bool updateAfter = true);
    ~DrawLayer();
    
    /**
     Add a Drawable to the DrawLayer
     @param The object to add to the DrawLayer
     */
    void add(Drawable *object);
    /**
     Remove a Drawable from the DrawLayer
     @param The object to be removed from the DrawLayer
     */
    void remove(Drawable *object);
    
    /** render this layer */
    void draw();
    
    /** update this layer */
    void update();
    
    /** should layers after this one update */
    bool shouldUpdateAfter();
    
    /** should layers after this one render */
    bool shouldRenderAfter();
    
    /** should this layer render */
    bool shouldRender();
    
    /** should this layer update */
    bool shouldUpdate();

    
};

#endif /* defined(__SLUG__DrawLayer__) */
