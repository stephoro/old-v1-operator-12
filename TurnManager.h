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
#include "SDL.h"
#include "QuadTree.hpp"
#include "SlugSegment.h"
class Slug;
class TurnManager;
class Map;

extern Map * map;
extern LinkedList<TurnManager> * turnManagerList;

class TurnManager{
private:
    bool isMyTurn;
protected:
    Uint32 team;
    LinkedList<Slug> * managedSlugs;
public:
    QuadTree<SlugSegment> * segmentMap;
    SlugSegment * getClosestEnemy(Slug * closeTo);
    int numberOfSlugsTillTurnOver;
    LinkedList<TurnManager>::Node<TurnManager> * nodeRef;
    bool isOwnedSlug(Slug * slug);
    bool isOwnTurn();
    void newTurn();
    void addSlug(Slug * slug);
    void removeSlug(Slug * slug);
    void slugDone();
    TurnManager(Uint32 team);
    ~TurnManager();
};

#endif /* defined(__SLUG__TurnManager__) */
