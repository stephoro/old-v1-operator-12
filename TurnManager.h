//
//  TurnManager.h
//  SLUG
//
//  Created by Steph on 3/15/16.
//
//

#ifndef __SLUG__TurnManager__
#define __SLUG__TurnManager__

#include <stdio.h>
#include "LinkedList.hpp"

class Slug;
class TurnManager;
class Map;

extern Map * map;
extern LinkedList<TurnManager> * turnManagerList;

class TurnManager{
private:
    LinkedList<Slug> * managedSlugs;
    bool isMyTurn;
public:
    int numberOfSlugsTillTurnOver;
    LinkedList<TurnManager>::Node<TurnManager> * nodeRef;
    bool isOwnedSlug(Slug * slug);
    bool isOwnTurn();
    void newTurn();
    void addSlug(Slug * slug);
    void removeSlug(Slug * slug);
    void slugDone();
    TurnManager();
};

#endif /* defined(__SLUG__TurnManager__) */
