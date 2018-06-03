//
//  QuadTree.hpp
//  SLUG
//
//  Created by Steph on 7/16/16.
//
//

#ifndef QuadTree_hpp
#define QuadTree_hpp

#include <stdio.h>

template <typename Obj>
class QuadTree{
private:
    int cx, cy;//center
    int ox, oy;//object
    int x1, y1, x2, y2;
    QuadTree ** children;
    Obj * child;
public:
    QuadTree(int x, int y, int w, int h, QuadTree * parent = NULL){
        children = NULL;
        child = NULL;
        this->x1=x;
        this->y1=y;
        this->x2=x+w;
        this->y2=y+h;
        cx = x+w/2;
        cy = y+h/2;
    }
    
    ~QuadTree(){
        if(children != NULL){
            for(int i = 0; i < 4; i++)
                delete children[i];
            delete [] children;
        }
    }
    
    
    inline int getIndex(const int x, const int y){
        int index = 0;
        if(x > cx)
            index++;
        if(y > cy)
            index += 2;
        return index;
    }
    
    inline void initChild(const int index, const int x, const int y){
        int nx = x1;
        int ny = y1;
        int w2 = (x2-x1)/2;
        int h2 = (y2-y1)/2;
        if(x > cx)
            nx += w2;
        if(y > cy)
            ny += w2;
        children[index]=new QuadTree(nx,ny,w2,h2, this);
    }
    
    /*
     0|1
     ---
     2|3
     */
    bool insert(Obj * obj, int x, int y){
        if(children == NULL){
            if(child != NULL){
                children = new QuadTree[4];
                children[0]=children[1]=children[2]=children[3]=NULL;
                int index = getIndex(x, y);
                if(children[index] == NULL){
                    initChild(index, x, y);
                }
                children[index]->insert(child, x, y);
                child = NULL;
            }else{
                child = obj;
                ox=x;
                oy=y;
                return true;
            }
        }
        int index = getIndex(ox, oy);
        if(children[index] == NULL){
            initChild(index, ox, oy);
        }
        return children[index]->insert(obj, ox, oy);
    }
    
    bool remove(Obj * obj, int x, int y){
        QuadTree * parent = this;
        int index;
        QuadTree ** searching = children;
        QuadTree * oldParent;
        while (searching != NULL) {
            oldParent = parent;
            index = parent->getIndex(x,y);
            parent = searching[index];
            if(parent == NULL){
                parent = oldParent;
                break;
            }
            searching = parent->children;
        }
        if(parent->child == obj){
            parent->child = NULL;
            return true;
        }
        return false;
    }
    
    Obj * closestTo(int x, int y){
        return closestTo(x, y, 0x7FFFFFFF);
    }
    
    Obj * closestTo(int x, int y, int & dist, Obj * best = NULL){
        int bt = y < cy, rt = x > cx;
        
        //exclude if dist is less than best here
        if (x < x1 - dist || x > x2 + dist || y < y1 - dist || y > y2 + dist) {
            return best;
        }
        
        if(child != NULL){
            int d = ox - x;
            int v = oy - y;
            if(d < 0)d=-d;
            if(v < 0)v=-v;
            d=d+v;
            if(d < dist){
                dist = d;
                best = child;
            }
        }
        
        int ch = bt*2 + rt;
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        ch = (!bt)*2 + rt;
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        ch = bt*2 + (!rt);
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        ch = (!bt)*2 + (!rt);
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        return NULL;
    }
    
    
};

#endif /* QuadTree_hpp */
