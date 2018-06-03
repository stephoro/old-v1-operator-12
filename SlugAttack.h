//
//  SlugAttack.h
//  SLUG
//
//  Created by Steph on 3/18/16.
//
//

#ifndef __SLUG__SlugAttack__
#define __SLUG__SlugAttack__

#include <stdio.h>
#include "SDL.h"
#include "CharArray.hpp"
#include "LinkedList.hpp"

#define ACTION_NUM_TYPES 7
#define ACTION_ATTACK 0x1
#define ACTION_HEAL 0x2
#define ACTION_SPEED 0x4
#define ACTION_CRIPPLE 0x8
#define ACTION_MULTI 0x30
#define ACTION_SELF 0x50
#define ACTION_SPAWN 0x80
#define ACTION_ISMULTI 0x10

class SlugAction;

struct SLUG {
    char * name;
    char * imgPath;
    Uint32 color;
    Uint8 xIndex;
    Uint8 yIndex;
    Uint8 maxSize;
    Uint8 price;
    Uint8 maxMoves;
    LinkedList<SlugAction> * actions;
    Uint32 rowNumber;
    int numberAvailable;
};

class Slug;
class SlugAction{
private:
    Uint8 range;
    int type;
    
public:
    SlugAction(int range, int type);
    virtual ~SlugAction();
    virtual bool commit(Slug * sender, Slug * reciever, int x, int y) = 0;
    virtual SlugAction * copy() = 0;
    void setRange(Uint8 r);
    int getRange();
    void setType(int t);
    int getType();
    bool canTargetSelf;
    char * name;
};

class SlugAttack : public SlugAction{
private:
    Uint8 damage;
public:
    SlugAttack();
    SlugAction * copy();
    int getDamage();
    void setDamage(Uint8 damage);
    bool commit(Slug * sender, Slug * reciever, int x, int y);
};

class SlugHeal : public SlugAction{
private:
    Uint8 healing;
    Uint8 bools;
public:
    SlugHeal();
    SlugAction * copy();
    int getPointsHealed();
    void setPointsHealed(Uint8 p);
    bool commit(Slug * sender, Slug * reciever, int x, int y);
    void setBools(Uint8 bools);
};

class SlugSpeedChange : public SlugAction{
private:
    int amount;
public:
    SlugSpeedChange();
    SlugAction * copy();
    int getDelta();
    void setDelta(int d);
    bool commit(Slug * sender, Slug * reciever, int x, int y);
};

class SlugCripple : public SlugAction{
private:
    Uint8 bools;
public:
    SlugCripple();
    SlugAction * copy();
    void setBools(Uint8 bools);
    bool commit(Slug * sender, Slug * reciever, int x, int y);
};

class SlugActionMulti : public SlugAction{
public:
    SlugActionMulti(int r, int t);
    SlugAction * parent;
    int actionsLeft;
    virtual void addAction(SlugAction * action) = 0;
};

class SlugMultiaction : public SlugActionMulti{
private:
    LinkedList<SlugAction> * actions;
public:
    SlugMultiaction();
    ~SlugMultiaction();
    void addAction(SlugAction * action);
    SlugAction * copy();
    bool commit(Slug * sender, Slug * reciever, int x, int y);
};

class SlugAffectSelf : public SlugActionMulti{
private:
    LinkedList<SlugAction> * actions;
public:
    SlugAffectSelf();
    ~SlugAffectSelf();
    void addAction(SlugAction * action);
    SlugAction * copy();
    bool commit(Slug * sender, Slug * reciever, int x, int y);
};


class SlugSpawnOther : public SlugAction{
private:
    char * filename;
    char * slugname;
public:
    SlugSpawnOther();
    ~SlugSpawnOther();
    void setFilename(char * file);
    void setSlugname(char * file);
    SlugAction * copy();
    bool commit(Slug * sender, Slug * reciever, int x, int y);
};

#endif /* defined(__SLUG__SlugAttack__) */
