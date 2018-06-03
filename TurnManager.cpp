//
//  TurnManager.cpp
//  SLUG
//
//  Created by Steph on 3/15/16.
//
//

#include "TurnManager.h"
#include "Slug.h"
#include "Map.h"
#include "SlugController.h"
extern SlugController * slugController;

TurnManager::TurnManager(Uint32 team){
    this->team = team;
    segmentMap = new QuadTree<SlugSegment>(0,0,map->width,map->height);
    if(turnManagerList == NULL){
        turnManagerList = new LinkedList<TurnManager>();
    }
    managedSlugs = new LinkedList<Slug>();
    numberOfSlugsTillTurnOver = 0;
    nodeRef = turnManagerList->add(this);
    isMyTurn = false;
}

TurnManager::~TurnManager(){
    delete segmentMap;
    if(turnManagerList != NULL){
        delete turnManagerList;
        turnManagerList = NULL;
    }
    managedSlugs->drain();
    delete managedSlugs;
}

bool TurnManager::isOwnTurn(){
    return isMyTurn;
}

void TurnManager::addSlug(Slug *slug){
    slug->team |= team;
    slug->nodeRef = managedSlugs->add(slug);
    slug->manager = this;
    numberOfSlugsTillTurnOver++;
}

void TurnManager::removeSlug(Slug *slug){
    managedSlugs->remove(slug->nodeRef);
    if(!slug->getIsDone()){
        numberOfSlugsTillTurnOver--;
    }
}

Slug * TurnManager::getClosestEnemy(){
    int managersLeft = turnManagerList->getSize() - 1;
    managersLeft--;
    Slug * closest;
    int dist = 0;
    while(managersLeft--){
        TurnManager * list = turnManagerList->next();
        if((list->team & team) == 0){
            if(dist == 0){
                //TODO
            }
        }
    }
    turnManagerList->next();
    return NULL;
}

bool TurnManager::isOwnedSlug(Slug * slug){
    return slug->manager == this;
}

void TurnManager::newTurn(){
    isMyTurn = true;
    numberOfSlugsTillTurnOver = managedSlugs->getSize();
    int slugsToWake = numberOfSlugsTillTurnOver;
    managedSlugs->refocus();
    while (slugsToWake > 0) {
        slugsToWake--;
        managedSlugs->next()->newTurn();
    }
    Slug* next = managedSlugs->next();
    slugController->setSlug(next);
    map->focusSlug = next;
}

void TurnManager::slugDone(){
    numberOfSlugsTillTurnOver--;
    if (numberOfSlugsTillTurnOver == 0) {
        isMyTurn = false;
        map->nextTurn();
        turnManagerList->next()->newTurn();
    }else{
        Slug* next = managedSlugs->next();
        while (next->controlMode == CONTROLLER_MODE_INVALID) {
            next = managedSlugs->next();
        }
        slugController->setSlug(next);
        map->focusSlug = next;
    }
}