//
//  Slug.h
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#ifndef __SLUG__Slug__
#define __SLUG__Slug__

#include <stdio.h>
#include "LinkedList.hpp"
#include "TextureLoader.h"
#include "Map.h"
#include "SlugData.h"
class Sprite;
class Slug;
class SlugSegment;
class TurnManager;


extern Map * map;
extern Sprite * doneCheckmark;


class Slug{
private:
    int x, y, size, inset;
    int movementSpeed;
    SlugData * myData;
    int movesLeft;
    SlugSegment * head;
    LinkedList<SlugSegment> * segments;
    void removeChunk();
    bool movementDone;
    void addConnections(int toX, int toY, SlugSegment * inquisitor);
    int maxSize;
public:
    bool isDone;
    int getMaxSize();
    void setMaxSize(int s);
    SlugAction * currentAction;
    int currentSize;
    bool isCrippled;
    int controlMode;
    Sprite * headSprite;
    const SLUG * getSLUG();
    void invalidate();
    int getControlMode();
    void grow(int number, bool addSegments, bool overflow, bool increaseMax);
    void startAttack();
    bool takeDamage(int amount);
    bool getIsDone();
    TurnManager * manager;
    LinkedList<Slug>::Node<Slug> * nodeRef;
    SDL_Texture * sprite;
    Slug(int size, int inset, int x, int y, SlugData * data);
    ~Slug();
    void move(int direction);
    int getX();
    int getY();
    int getMovementSpeed();
    int getMovesLeft();
    void newTurn();
    void setMovementSpeed(int moveSpeed);
};

#endif /* defined(__SLUG__Slug__) */
