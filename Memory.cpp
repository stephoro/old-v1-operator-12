//
//  Memory.cpp
//  SLUG
//
//  Created by Steph on 3/20/16.
//
//
#define END_OF_FILE '\0'

#include "Memory.h"




extern float RETINA_FACTOR;






/*
 issue, what if your parent that resumes control already is disactivated.
 */
void SlugMemAddActive(MemoryStruct * actives, SlugAction *activeAction, bool actuallyAdd = true){
    if(!actuallyAdd)
        printf("false addition\n");
    if(actives->isMultiAction){
        SlugActionMulti * owner = (SlugActionMulti *)actives->action;
        printf("Owner: %s actions: %i\n",owner->name, owner->actionsLeft);
        if(actuallyAdd)
            owner->addAction(activeAction);
        owner->actionsLeft--;
        bool isMulti = true;
        while(isMulti && owner->actionsLeft <= 0){
            printf("releasing control %s\n",owner->name);
            if(owner->parent == NULL){
                isMulti = actives->isMultiAction = false;
            }else{
                printf("getting type from %s\n",owner->name);
                int t = owner->parent->getType();
                if(!(t & ACTION_ISMULTI)){
                    isMulti = actives->isMultiAction = false;
                    actives->action = owner->parent;
                    printf("resuming control %i %d\n",owner->parent->getType(), owner == owner->parent);
                }else{
                    owner = (SlugActionMulti *)owner->parent;
                    printf("new (%s) ownership %i\n",owner->name, owner->actionsLeft);
                    actives->action = owner;
                }
            }
        }
        if(owner->actionsLeft <= 0){
            actives->isMultiAction = false;
        }
    }else if(actuallyAdd){
        actives->action = activeAction;
        actives->slug->actions->add(activeAction);
    }
}


void ReadComment(CharArray * proto, char * name, MemoryStruct * actives){
    printf("comment %s\n",name);
    delete [] name;
}

void SlugInit(CharArray * proto, char * name, MemoryStruct * actives){
    SLUG * activeSlug = new SLUG();
    actives->slug = activeSlug;
    actives->slugs->add(activeSlug);
    activeSlug->actions = new LinkedList<SlugAction>();
    activeSlug->imgPath = proto->getString();
    activeSlug->name = name;
    activeSlug->color = proto->get32bits();
    activeSlug->xIndex = proto->get8bits();
    activeSlug->yIndex = proto->get8bits();
    activeSlug->maxSize = proto->get8bits();
    activeSlug->maxMoves = proto->get8bits();
    activeSlug->rowNumber = actives->rowNumber;
    activeSlug->numberAvailable = 1;
    printf("path %s\n",activeSlug->imgPath);
    printf("color %i\n",activeSlug->color);
    printf("x %i\n",activeSlug->xIndex);
    printf("y %i\n",activeSlug->yIndex);
    printf("health %i\n",activeSlug->maxSize);
    printf("speed %i\n",activeSlug->maxMoves);
    
}

void SlugCost(CharArray * proto, char * name, MemoryStruct * actives){
    SLUG * activeSlug = actives->slug;
    delete [] name;
    activeSlug->price = proto->get32bits();
    printf("price %i\n",activeSlug->price);
}

void SlugActionRange(CharArray * proto, char * name, MemoryStruct * actives){
    delete [] name;
    if(actives->action == NULL){
        printf("ERR NO ACTION");
        proto->get8bits();
        return;
    }
    SlugAction * activeAction = actives->action;
    activeAction->setRange(proto->get8bits());
    printf("Action %s range: %i\n",activeAction->name,activeAction->getRange());
    SlugMemAddActive(actives, NULL, false);
}

void SlugActionTargeting(CharArray * proto, char * name, MemoryStruct * actives){
    delete [] name;
   if(actives->action == NULL){
        printf("ERR NO ACTION");
        proto->get8bits();
        return;
    }
    SlugAction * activeAction = actives->action;
    activeAction->canTargetSelf = getNthBool(proto->get8bits(), 0);
    printf("Can target self: %d\n",activeAction->canTargetSelf);
}

void SlugSetAmount(CharArray * proto, char * name, MemoryStruct * actives){
    delete [] name;
    if(actives->slug == NULL){
        printf("ERR NO Slug");
        proto->get32bits();
        return;
    }
    SLUG * activeSlug = actives->slug;
    activeSlug->numberAvailable = proto->get32bits();
    printf("Slugs Avail: %i\n",activeSlug->numberAvailable);
}


void SlugMultilineAction(CharArray * proto, char * name, MemoryStruct * actives){
    SlugMultiaction * activeAction = new SlugMultiaction();
    activeAction->parent = actives->action;
    SlugMemAddActive(actives, activeAction);
    actives->action = activeAction;
    activeAction->name = name;
    
    
    actives->isMultiAction = true;
    activeAction->actionsLeft = proto->get8bits();
    
    printf("NEW Multi Action, Name: %s\n",name);
    printf("number consumed %i\n",activeAction->actionsLeft);
}

void SlugActionHealth(CharArray * proto, char * name, MemoryStruct * actives){
    SlugHeal * activeAction = new SlugHeal();
    SlugMemAddActive(actives, activeAction);
    activeAction->name = name;
    
    activeAction->setPointsHealed(proto->get8bits());
    activeAction->setBools(proto->get8bits());
    printf("NEW Action, Name: %s\n",name);
    printf("Action healing: %i\n",activeAction->getPointsHealed());
}

void SlugActionDamage(CharArray * proto, char * name, MemoryStruct * actives){
    SlugAttack * activeAction = new SlugAttack();
    SlugMemAddActive(actives, activeAction);
    activeAction->name = name;
    
    activeAction->setDamage(proto->get8bits());
    
    printf("NEW Action, Name: %s\n",name);
    printf("Action damage: %i\n",activeAction->getDamage());
}

void SlugActionSpawn(CharArray * proto, char * name, MemoryStruct * actives){
    SlugSpawnOther * activeAction = new SlugSpawnOther();
    SlugMemAddActive(actives, activeAction);
    activeAction->name = name;
    printf("NEW Action, Name: %s\n",name);    
    activeAction->setFilename(proto->getString());
    activeAction->setSlugname(proto->getString());
    
}

void SlugActionSpeed(CharArray * proto, char * name, MemoryStruct * actives){
    SlugSpeedChange * activeAction = new SlugSpeedChange();
    SlugMemAddActive(actives, activeAction);
    activeAction->name = name;
    
    activeAction->setDelta(proto->get32bits());
    
    printf("NEW Action, Name: %s\n",name);
    printf("Action speed: %i\n",activeAction->getDelta());
}

void SlugActionCripple(CharArray * proto, char * name, MemoryStruct * actives){
    SlugCripple * activeAction = new SlugCripple();
    SlugMemAddActive(actives, activeAction);
    activeAction->name = name;
    
    activeAction->setBools(proto->get8bits());
    
    printf("NEW Action, Name: %s\n",name);
}

void SlugActionAffectSelf(CharArray * proto, char * name, MemoryStruct * actives){
    SlugAffectSelf * activeAction = new SlugAffectSelf();
    activeAction->parent = actives->action;
    SlugMemAddActive(actives, activeAction);
    actives->action = activeAction;
    activeAction->name = name;
    
    
    actives->isMultiAction = true;
    activeAction->actionsLeft = proto->get8bits();
    
    printf("NEW Self Action, Name: %s\n",name);
    printf("number consumed %i\n",activeAction->actionsLeft);
}

typedef void (*LoadingFunction) (CharArray * proto, char * name, MemoryStruct * actives);

SlugFile::SlugFile(char * filename){
    archetypesHash = new HashMap<SlugData>();
    this->filename = filename;
    raw_data = file_read(filename);//
    
    isLoaded = false;
    
    if(raw_data == NULL){
        printf("failed to load %s\n",filename);
        printf("err %s",SDL_GetError());
        delete [] filename;
        return;
    }
    memory = new MemoryStruct();
    memory->isMultiAction = false;
    memory->action = NULL;
    memory->slug = NULL;
    memory->rowNumber = 0;
    proto = new CharArray(raw_data);
    memory->slugs = new LinkedList<SLUG>();
    
    printf("loading %s\n",filename);
    
    printf("working\n");
    
    int numOfFuncs = 13;
    LoadingFunction functions[] =
    {
        ReadComment,
        SlugInit,
        SlugCost,
        SlugMultilineAction,
        SlugActionRange,
        SlugActionDamage,
        SlugActionHealth,
        SlugActionSpeed,
        SlugActionCripple,
        SlugActionAffectSelf,
        SlugActionTargeting,
        SlugActionSpawn,
        SlugSetAmount
    };
    
    int length = proto->get32bits();
    int type;
    char * name;
    
    for(int i = 0; i < length; i++){
        type = proto->get32bits();
        name = proto->getString();
        printf("type: %i multi? %d\n",type,memory->isMultiAction);
        if(type < 0 || type >= numOfFuncs)
            type = 0;
        memory->rowNumber++;
        functions[type](proto, name, memory);
    }
    numberOfSlugs = memory->slugs->getSize();
    archetypes = new SlugData*[numberOfSlugs];
    memory->slugs->refocus();
    printf("num of slugs %i\n",numberOfSlugs);
    if(numberOfSlugs){
    for(int s = 0; s < numberOfSlugs; s++){
        SlugData * data = new SlugData();
        data->setProto(memory->slugs->next());
        archetypes[s] = data;
        archetypesHash->put(data->prototype->name, data);
    }
    
    sheet = new SpriteSheet(archetypes[0]->prototype->imgPath, 28*2, 28*2, 28, 28);
    
    for(int s = 0; s < numberOfSlugs; s++){
        archetypes[s]->setSheet(sheet);
    }
    
    isLoaded = true;
    }
    else{
        isLoaded = false;
    }
}

SlugFile::~SlugFile(){
    delete archetypesHash;
    freeMemoryStruct(memory);
    if(sheet != NULL)
        delete sheet;
    delete proto;
    delete [] filename;
    for (int s = 0; s < numberOfSlugs; s++) {
        delete archetypes[s];
    }
    delete [] archetypes;
}

void SlugFile::freeSLUG(SLUG *toFree){
    toFree->actions->drain();
    delete toFree->actions;
    delete [] toFree->imgPath;
    delete toFree;
    delete [] toFree->name;
}

bool SlugFile::getIsLoaded(){
    return isLoaded;
}

void SlugFile::freeMemoryStruct(MemoryStruct *toFree){
    int s = toFree->slugs->getSize();
    toFree->slugs->refocus();
    while(s--){
        SLUG * slug = toFree->slugs->next();
        freeSLUG(slug);
    }
    delete toFree->slugs;
    delete toFree;
}

void loadSlugs(){
}/*
  char * chars = file_read("images/slugs.dat");
  std::string source = std::string(chars);
  long length = source.length();
  long position = 0;
  bool seeking = false;
  char seek = '\n';
  char searchTerm = ':';
  char newLine = '\n';
  char space = ' ';
  char tab = '\t';
  char quote = '"';
  char escape = '\\';
  long lastFound = 0;
  bool isQuoted = false;
  bool wasEscaped = false;
  int slugNumber;
  int abilityNumber;
  SlugData * data = NULL;
  SlugAttack * attack = NULL;
  LinkedList<std::string> splits;
  string currentWord = "";
  int archetypeLength;
  
  while (position < length) {
  if (wasEscaped) {
  wasEscaped = false;
  position++;
  continue;
  }
  char at = source.at(position);
  if (seeking) {
  if (at == seek) {
  seeking = false;
  continue;
  }else{
  position++;
  continue;
  }
  }
  
  if (at == escape) {
  position++;
  wasEscaped = true;
  currentWord += source.substr(lastFound, position - lastFound);
  lastFound = position + 1;
  continue;
  }
  if (at == quote) {
  if (isQuoted) {
  if(position - lastFound > 0)
  currentWord += source.substr(lastFound, position - lastFound);
  lastFound = position + 1;
  if(currentWord.length() > 0){
  string * toAdd = new string(currentWord);
  splits.add(toAdd);
  currentWord = "";
  }
  }else{
  lastFound = position + 1;
  }
  isQuoted = !isQuoted;
  position++;
  continue;
  }
  if (isQuoted) {
  position++;
  continue;
  }
  if (at == searchTerm || at == newLine || at == tab || at == space) {
  if(position - lastFound > 0)
  currentWord += source.substr(lastFound, position - lastFound);
  lastFound = position + 1;
  if(currentWord.length() > 0){
  string * toAdd = new string(currentWord);
  splits.add(toAdd);
  currentWord = "";
  }
  if (at == newLine) {
  string * key = splits.removeFirst();
  if (key->compare("name") == 0) {
  string * name = splits.removeFirst();
  data->name = copyStr(name);
  delete name;
  }else if (key->compare("title") == 0) {
  string * name = splits.removeFirst();
  attack->name = copyStr(name);
  delete name;
  }else if (key->compare("sprite") == 0) {
  string * number = splits.removeFirst();
  data->sprite = slugHeadSheet->getSprite(parseInt(number));
  delete number;
  }else if (key->compare("spritesheet") == 0) {
  string * name = splits.removeFirst();
  string * w = splits.removeFirst();
  string * h = splits.removeFirst();
  string * dw = splits.removeFirst();
  string * dh = splits.removeFirst();
  slugHeadSheet = new SpriteSheet(*name, parseInt(w),  parseInt(h),  parseInt(dw),  parseInt(dh));
  delete name;
  delete w;
  delete h;
  delete dw;
  delete dh;
  }else if (key->compare("size") == 0) {
  string * number = splits.removeFirst();
  data->size = parseInt(number);
  delete number;
  }else if (key->compare("color") == 0) {
  string * r = splits.removeFirst();
  string * g = splits.removeFirst();
  string * b = splits.removeFirst();
  data->makeColor(parseInt(r), parseInt(g), parseInt(b));
  delete r;
  delete g;
  delete b;
  }else if (key->compare("move") == 0) {
  string * number = splits.removeFirst();
  data->move = parseInt(number);
  delete number;
  }else if (key->compare("range") == 0) {
  string * number = splits.removeFirst();
  attack->setRange(parseInt(number));
  delete number;
  }else if (key->compare("hit") == 0) {
  string * number = splits.removeFirst();
  attack->setStrength(parseInt(number));
  delete number;
  }else if (key->compare("Slug") == 0) {
  if(data != NULL){
  if(attack != NULL){
  data->setAttack(abilityNumber, attack);
  }
  archetypes[slugNumber] = data;
  }
  string * number = splits.removeFirst();
  slugNumber = parseInt(number);
  data = new SlugData();
  delete number;
  }else if (key->compare("attack") == 0) {
  if(attack != NULL){
  data->setAttack(abilityNumber, attack);
  }
  string * number = splits.removeFirst();
  abilityNumber = parseInt(number);
  attack = new SlugAttack();
  delete number;
  }else if (key->compare("Slugs") == 0) {
  string * number = splits.removeFirst();
  archetypeLength = parseInt(number);
  archetypes = new SlugData*[archetypeLength];
  delete number;
  }else if (key->compare("abilities") == 0) {
  string * number = splits.removeFirst();
  data->initAttacks(parseInt(number));
  delete number;
  }else if (key->compare("END") == 0) {
  printf("EOF\n");
  delete key;
  break;
  }
  delete key;
  while (splits.getSize() > 0) {
  string * nth = splits.removeLast();
  printf("%s\n",nth->c_str());
  delete nth;
  }
  }
  }
  position++;
  }
  if(data != NULL){
  if(attack != NULL){
  data->setAttack(abilityNumber, attack);
  }
  archetypes[slugNumber] = data;
  }
  
  for (int i = 0; i < archetypeLength; i++) {
  printf("Slug:%s\n",archetypes[i]->name);
  printf("move:%i\n",archetypes[i]->move);
  printf("size:%i\n",archetypes[i]->size);
  printf("attck:%s\n",archetypes[i]->getAttack(0)->name);
  printf("attack range:%i\n",archetypes[i]->getAttack(0)->getRange());
  printf("attack str:%i\n",archetypes[i]->getAttack(0)->getStrength());
  }
  
  }*/

int ** loadMap(int &width, int &height){
    char * raw_data = file_read("images/map01.dat");
    
    if (raw_data == NULL) {
        return NULL;
    }
    
    CharArray * proto = new CharArray(raw_data);
    int ** mapTiles;
    
    width = proto->get16bits();
    height = proto->get16bits();
    proto->get16bits();//render width
    proto->get16bits();//render height
    
    mapTiles = new int*[width];
    for(int x = 0; x < width; x++){
        int * col = mapTiles[x] = new int[height];
        for(int y = 0; y < height; y++){
            col[y] = proto->get8bits();
        }
    }
    
    
    return mapTiles;
}