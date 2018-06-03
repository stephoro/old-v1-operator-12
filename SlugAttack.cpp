//
//  SlugAttack.cpp
//  SLUG
//
//  Created by Steph on 3/18/16.
//
//

#include "SlugAttack.h"
#include "StringHelpers.h"
#include "Slug.h"
#include "Hashmap.hpp"
#include "Memory.h"
#include "TurnManager.h"
extern HashMap<SlugFile> * SlugFileSystem;

#pragma mark Parent

SlugAction::SlugAction(int range, int type){
    name = NULL;
    canTargetSelf = false;
    setRange(range);
    setType(type);
}

void SlugAction::setRange(Uint8 r){
    if(r < 0)
        r = 0;
    range = r;
}
int SlugAction::getRange(){
    return range;
}

void SlugAction::setType(int t){
    if(t < 0)
        t = 0;
    type = t;
}
int SlugAction::getType(){
    return type;
}

SlugAction::~SlugAction(){
    if(name != NULL){
        printf("deleting action %s",name);
        delete name;
    }
}

#pragma mark Attack

bool SlugAttack::commit(Slug *sender, Slug *reciever, int x, int y){
    if(reciever == NULL)
        return true;
    reciever->takeDamage(damage);
    return true;
}

SlugAttack::SlugAttack() : SlugAction(1,ACTION_ATTACK){
    damage = 1;
}

SlugAction * SlugAttack::copy(){
    SlugAttack * newAction = new SlugAttack();
    newAction->setRange(getRange());
    newAction->setDamage(damage);
    newAction->name = copyStr(name);
    return newAction;
}

int SlugAttack::getDamage(){
    return damage;
}

void SlugAttack::setDamage(Uint8 damage){
    if (damage < 0)
        damage = 0;
    this->damage = damage;
}

#pragma mark Heal

bool SlugHeal::commit(Slug *sender, Slug *reciever, int x, int y){
    if(reciever != NULL)
        reciever->grow(healing,getNthBool(bools, 0),getNthBool(bools, 1),getNthBool(bools, 2));
    return true;
}

SlugHeal::SlugHeal() : SlugAction(1,ACTION_HEAL){
    healing = 1;
    bools = 0x80;
}

SlugAction * SlugHeal::copy(){
    SlugHeal * newAction = new SlugHeal();
    newAction->setRange(getRange());
    newAction->setBools(bools);
    newAction->setPointsHealed(healing);
    newAction->name = copyStr(name);
    return newAction;
}

int SlugHeal::getPointsHealed(){
    return healing;
}

void SlugHeal::setPointsHealed(Uint8 p){
    if (p < 0)
        p = 0;
    this->healing = p;
}

void SlugHeal::setBools(Uint8 bools){
    this->bools = bools;
    printf("bools set %d %d %d %d\n",bools,getNthBool(bools, 0),getNthBool(bools, 1),getNthBool(bools, 2));
}

#pragma mark Speed

bool SlugSpeedChange::commit(Slug *sender, Slug *reciever, int x, int y){
    if(reciever != NULL)
        reciever->setMovementSpeed(reciever->getMovementSpeed()+amount);
    return true;
}

SlugSpeedChange::SlugSpeedChange() : SlugAction(1,ACTION_SPEED){
    amount = 1;
}

SlugAction * SlugSpeedChange::copy(){
    SlugSpeedChange * newAction = new SlugSpeedChange();
    newAction->setRange(getRange());
    newAction->setDelta(amount);
    newAction->name = copyStr(name);
    return newAction;
}

int SlugSpeedChange::getDelta(){
    return amount;
}

void SlugSpeedChange::setDelta(int d){
    amount = d;
}

#pragma mark Cripple

bool SlugCripple::commit(Slug *sender, Slug *reciever, int x, int y){
    if(reciever != NULL){
        if(getNthBool(bools, 0))
            reciever->setMaxSize(reciever->currentSize);
        if(getNthBool(bools, 1))
            reciever->isCrippled = true;
    }
    return true;
}

SlugCripple::SlugCripple() : SlugAction(1,ACTION_CRIPPLE){
    bools = 0x80;
}

SlugAction * SlugCripple::copy(){
    SlugCripple * newAction = new SlugCripple();
    newAction->setRange(getRange());
    newAction->setBools(bools);
    newAction->name = copyStr(name);
    return newAction;
}

void SlugCripple::setBools(Uint8 bools){
    this->bools = bools;
    printf("bools set %d %d %d\n",bools,getNthBool(bools, 0),getNthBool(bools, 1));
}

#pragma mark Multithingy

SlugActionMulti::SlugActionMulti(int r, int t) : SlugAction(r,t){
    parent = NULL;
}

#pragma mark Multiattack

bool SlugMultiaction::commit(Slug *sender, Slug *reciever, int x, int y){
    int s = actions->getSize();
    actions->refocus();
    while(s--){
        actions->next()->commit(sender, reciever, x, y);
    }
    return true;
}

SlugMultiaction::SlugMultiaction() : SlugActionMulti(1,ACTION_MULTI){
    actions = new LinkedList<SlugAction>();
    parent = NULL;
}

SlugMultiaction::~SlugMultiaction(){
    actions->drain();
    delete actions;
}

void SlugMultiaction::addAction(SlugAction *action){
    this->actions->add(action);
}

SlugAction * SlugMultiaction::copy(){
    SlugMultiaction * newAction = new SlugMultiaction();
    newAction->setRange(getRange());
    newAction->name = copyStr(name);
    int s = actions->getSize();
    actions->refocus();
    while(s--){
        newAction->addAction(actions->next()->copy());
    }
    return newAction;
}

#pragma mark SelfAction

bool SlugAffectSelf::commit(Slug *sender, Slug *reciever, int x, int y){
    int s = actions->getSize();
    actions->refocus();
    while(s--){
        actions->next()->commit(sender, sender, x, y);
    }
    return true;
}

SlugAffectSelf::SlugAffectSelf() : SlugActionMulti(1,ACTION_SELF){
    actions = new LinkedList<SlugAction>();
    parent = NULL;
}

SlugAffectSelf::~SlugAffectSelf(){
    actions->drain();
    delete actions;
}

void SlugAffectSelf::addAction(SlugAction *action){
    this->actions->add(action);
}

SlugAction * SlugAffectSelf::copy(){
    SlugAffectSelf * newAction = new SlugAffectSelf();
    newAction->setRange(getRange());
    newAction->name = copyStr(name);
    int s = actions->getSize();
    actions->refocus();
    while(s--){
        newAction->addAction(actions->next()->copy());
    }
    return newAction;
}

#pragma mark Spawn

bool SlugSpawnOther::commit(Slug *sender, Slug *reciever, int x, int y){
    if(map->isOccupied(x, y, NULL) != MAP_TILE_EMPTY)
        return true;
    SlugFile * file = SlugFileSystem->get(filename);
    printf("filename: %s",filename);
    
    if(file != NULL){
        SlugData * data = file->archetypesHash->get(slugname);
        if(data != NULL){
            Slug * slug = new Slug(28, 4, x, y, data);
            sender->manager->addSlug(slug);
            slug->isDone = true;
            sender->manager->numberOfSlugsTillTurnOver--;
        }else{
            printf("data null\n");
        }
    }else{
        printf("file null\n");
    }
    return true;
}

SlugSpawnOther::SlugSpawnOther() : SlugAction(1,ACTION_SPAWN){
    filename = NULL;
    slugname = NULL;
}

SlugSpawnOther::~SlugSpawnOther(){
    if(filename != NULL)
        delete [] filename;
}

void SlugSpawnOther::setSlugname(char *file){
    printf("slugname: %s\n",file);
    slugname = file;
}

void SlugSpawnOther::setFilename(char *file){
    printf("filename: %s\n",file);
    filename = file;
}

SlugAction * SlugSpawnOther::copy(){
    SlugSpawnOther * newAction = new SlugSpawnOther();
    newAction->setRange(getRange());
    newAction->name = copyStr(name);
    newAction->setSlugname(copyStr(filename));
    newAction->setFilename(copyStr(filename));
    return newAction;
}


