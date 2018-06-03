//
//  Slug.cpp
//  SLUG
//
//  Created by Steph on 3/12/16.
//
//

#include "Slug.h"
#include "TurnManager.h"
#include "Sprite.h"
#include "SlugController.h"

Slug::Slug(int size, int inset, int x, int y, SlugData * data){
    myData = data->copy();
    this->segments = new LinkedList<SlugSegment>();
    SLUG * proto = myData->prototype;
    setMovementSpeed(proto->maxMoves);
    
    proto->actions->refocus();
    currentAction = proto->actions->next();
    //this->attackStr =  myData->getAction(0)->getDamage();
    movesLeft = movementSpeed;
    this->x = x;
    this->y = y;
    isDone = false;
    movementDone = false;
    this->size = size;
    this->inset = inset;
    head = new SlugSegment(this, 0, size, inset, x, y, true);
    head->reference = this->segments->add(head);
    map->setSlug(head, x, y);
    currentSize = 1;
    setMaxSize(proto->maxSize);
    headSprite = myData->sprite;
    sprite = myData->texture;
    controlMode = CONTROLLER_MODE_INVALID;
    isCrippled = false;
}

const SLUG * Slug::getSLUG(){
    return myData->prototype;
}

void Slug::startAttack(){
    controlMode = CONTROLLER_MODE_ATTACK;
}

int Slug::getControlMode(){
    return controlMode;
}

bool Slug::getIsDone(){
    return isDone;
}

void Slug::invalidate(){
    if(!isDone)
        manager->slugDone();
    controlMode = CONTROLLER_MODE_INVALID;
    isDone = true;
}

bool Slug::takeDamage(int amount){
    while (amount-- > 0 && currentSize > 0) {
        removeChunk();
    }
    if (currentSize == 0) {
        manager->removeSlug(this);
    }
    return currentSize == 0;
}

Slug::~Slug(){
    delete myData;
    SlugSegment * first;
    while (segments->getSize()>0) {
        first = segments->removeFirst();
        map->setSlug(NULL, first->x, first->y);
    }
    delete segments;
}

void Slug::removeChunk(){
    SlugSegment * first = segments->getFirst();
    int x = first->x;
    int y = first->y;
    map->setSlug(NULL, x, y);
    segments->removeFirst();
    int pt = y - 1;
    if(map->pointValid(x, pt)){
        SlugSegment * segment = map->getSlug(x, pt);
        if(segment != NULL)
            segment->removeConnection(SLUG_SOUTH_CONNECTION);
    }
    pt = x - 1;
    if(map->pointValid(pt, y)){
        SlugSegment * segment = map->getSlug(pt, y);
        if(segment != NULL)
            segment->removeConnection(SLUG_EAST_CONNECTION);
    }
    currentSize--;
}

void Slug::newTurn(){
    isDone = false;
    movementDone = false;
    movesLeft = movementSpeed;
    controlMode = CONTROLLER_MODE_MOVE;
    if(movesLeft == 0)
        startAttack();
}

int Slug::getMaxSize(){
    return maxSize;
}

void Slug::setMaxSize(int s){
    if(s > 255)
        s = 255;
    if(s > 0)
        maxSize = s;
}

void Slug::grow(int number, bool addSegments, bool overflow, bool increaseMax){
    if(increaseMax && !overflow){
        setMaxSize(maxSize + number);
    }
    
    if(!overflow && currentSize + number > maxSize)
        number -= (maxSize-currentSize);
    if(number <= 0)
        return;
    if(!addSegments){
        if(overflow && !increaseMax)
            setMaxSize(currentSize + number);
        return;
    }
    
    LinkedList<SlugSegment> * temp = new LinkedList<SlugSegment>();
    segments->refocus();
    int n = segments->getSize();
    while(n-- && number){
        SlugSegment * near = segments->next();
        int x = near->x;
        int y = near->y;
        int k = -1;
        int dx, dy;
        while(k++ < 3 && number){
            switch (k) {
                case 0:
                    dx = 0;
                    dy = -1;
                    break;
                case 1:
                    dx = 1;
                    dy = 0;
                    break;
                case 2:
                    dx = 0;
                    dy = 1;
                    break;
                default:
                    dx = -1;
                    dy = 0;
                    break;
            }
            int ax = x + dx, ay = y + dy;
            if(!(map->isOccupied(ax, ay, NULL) & MAP_TILE_OUT_OF_BOUNDS)){
                SlugSegment * atPos = map->getSlug(ax, ay);
                bool isNull = atPos == NULL;
                if(isNull){
                    atPos = new SlugSegment(this, 0, size, inset, ax, ay, false);
                    map->setSlug(atPos, ax, ay);
                    temp->add(atPos);
                    addConnections(ax, ay, atPos);
                    number--;
                }
            }
        }
    }
    number = temp->getSize();
    temp->refocus();
    while(number--){
        SlugSegment * next =temp->next();
        next->reference = segments->addToFront(next);
        currentSize++;
    }
    if(currentSize > maxSize)
        setMaxSize(currentSize);
    delete temp;
}

void Slug::addConnections(int toX, int toY, SlugSegment * inquisitor){
    int pt = toY - 1;
    if(map->pointValid(toX, pt)){
        SlugSegment * segment = map->getSlug(toX, pt);
        if(segment != NULL && segment->getOwner() == this){
            segment->addConnection(SLUG_SOUTH_CONNECTION);
        }
        
    }
    pt = toX + 1;
    if(map->pointValid(pt, toY)){
        SlugSegment * segment = map->getSlug(pt, toY);
        if(segment != NULL && segment->getOwner() == this){
            inquisitor->addConnection(SLUG_EAST_CONNECTION);
        }
    }
    pt = toY + 1;
    if(map->pointValid(toX, pt)){
        SlugSegment * segment = map->getSlug(toX, pt);
        if(segment != NULL && segment->getOwner() == this){
            inquisitor->addConnection(SLUG_SOUTH_CONNECTION);
        }
        
    }
    pt = toX - 1;
    if(map->pointValid(pt, toY)){
        SlugSegment * segment = map->getSlug(pt, toY);
        if(segment != NULL && segment->getOwner() == this){
            segment->addConnection(SLUG_EAST_CONNECTION);
        }
    }
}

void Slug::move(int dir){
    if(isCrippled)
        setMaxSize(currentSize);
    if(movesLeft > 0){
        int dx, dy;
        switch (dir) {
            case 0:
                dx = 0;
                dy = -1;
                break;
            case 1:
                dx = 1;
                dy = 0;
                break;
            case 2:
                dx = 0;
                dy = 1;
                break;
            default:
                dx = -1;
                dy = 0;
                break;
        }
        
        int toX = x + dx,
        toY = y + dy;
        if(map->pointValid(toX, toY)){
            SlugSegment * atPos = map->getSlug(toX, toY);
            bool isNull = atPos == NULL;
            bool isSelf = !isNull && atPos->getOwner() == this;
            if(!isSelf){
                atPos = new SlugSegment(this, 0, size, inset, toX, toY, false);
                map->setSlug(atPos, toX, toY);
                atPos->reference = segments->add(atPos);
                currentSize++;
            }else{
                segments->remove(atPos->reference);
                atPos->reference = segments->add(atPos);
            }
            
            atPos->isHead = true;
            head->isHead = false;
            
            head = atPos;
            
            addConnections(toX, toY, atPos);
            
            if(currentSize > maxSize){
                removeChunk();
            }
            
            x = toX;
            y = toY;
            
        }
        movesLeft--;
        if(movesLeft == 0){
            movementDone = true;
            controlMode = CONTROLLER_MODE_ATTACK;
        }
    }
}

int Slug::getX(){
    return x;
}

int Slug::getY(){
    return y;
}

int Slug::getMovementSpeed(){
    return movementSpeed;
}

int Slug::getMovesLeft(){
    return movesLeft;
}

void Slug::setMovementSpeed(int moveSpeed){
    if(moveSpeed > 255)
        moveSpeed = 255;
    if(moveSpeed < 0)
        moveSpeed = 0;
    movementSpeed = moveSpeed;
}



