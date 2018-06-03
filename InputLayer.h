//
//  InputLayer.h
//  SLUG
//
//  Created by Steph on 3/13/16.
//
//

#ifndef __SLUG__InputLayer__
#define __SLUG__InputLayer__

#include <stdio.h>

#include "LinkedList.hpp"
#include "Interactable.h"
#include "SDL.h"

extern SDL_Renderer * globalRenderer;
/**
 A layer whose objects can be drawn.
 @author Steph
 */
class InputLayer{
private:
    
    /** The objects to be drawn. */
    LinkedList<Interactable>  * objects;

    /** If the following lower layers should be interacted with. */
    bool interactAfter;
    
public:
    /** Reference to where the layer is stored within the InputStack's linked list */
    LinkedList<InputLayer>::Node<InputLayer> * position;
    /**
     Construct an InputLayer
     @param updateAfter Should any subsequent layer accept input?
     */
    InputLayer(bool interactAfter = true);
    ~InputLayer();
    /**
     Add a Drawable to the DrawLayer
     @param The object to add to the DrawLayer
     */
    void add(Interactable *object);
    /**
     Remove a Drawable from the DrawLayer
     @param The object to be removed from the DrawLayer
     */
    void remove(Interactable *object);
    
    /** render this layer */
    bool onInput(int x, int y);
    
    /** should this layer render */
    bool shouldInteractAfter();
    
    
    
};

#endif /* defined(__SLUG__InputLayer__) */
