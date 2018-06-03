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
    int subs;
    int meIndex;
    int x1, y1, x2, y2;
    QuadTree ** children;
    QuadTree * parent;
    Obj * child;
public:
    QuadTree(int x, int y, int w, int h, QuadTree * parent = NULL, int index = 0){
        this->parent = parent;
        meIndex = index;
        children = NULL;
        child = NULL;
        subs=0;
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
        subs++;
        int nx = x1;
        int ny = y1;
        int w2 = (x2-x1);
        int h2 = (y2-y1);
        if((index&0x1) && (w2&0x1)) // if index is right and w2 odd
            w2++;
        if((index&0x2) && (h2&0x1)) // if index is bottom and h2 odd
            h2++;
        w2/=2;
        h2/=2;
        if(x > cx)
            nx += w2;
        if(y > cy)
            ny += w2;
        printf("New Child at %i (%i %i %i %i)\n", index,nx,ny,w2,h2);
        
        children[index]=new QuadTree<Obj>(nx,ny,w2,h2, this, index);
    }
    
    /*
     0|1
     ---
     2|3
     */
    bool insert(Obj * obj, int x, int y){
        //printf("inserting (%i %i) in (%i %i %i %i)\n",x,y,x1,y1,x2,y2);
        if(children == NULL){
            if(child != NULL){
                //printf("pregnant! dividing\n");
                children = new QuadTree*[4];
                children[0]=children[1]=children[2]=children[3]=NULL;
                int index = getIndex(ox, oy);
                if(children[index] == NULL){
                    initChild(index, ox, oy);
                }
                Obj * oldChild = child;
                child = NULL;
                children[index]->insert(oldChild, ox, oy);
            }else{
                //printf("inserted!\n");
                child = obj;
                ox=x;
                oy=y;
                return true;
            }
        }
        int index = getIndex(x, y);
        if(children[index] == NULL){
            initChild(index, x, y);
        }
        return children[index]->insert(obj, x, y);
    }
    
    void trim(int index){
        if(index != -1){
            children[index] = NULL;
            subs--;
            if(subs == 0){
                delete [] children;
                children = NULL;
                index = -1;
            }
        }
        if(index == -1 && parent != NULL){
            parent->trim(meIndex);
            //printf("trimmed this\n");
            delete this;
        }
    }
    
    bool remove(Obj * obj, int x, int y){
        //printf("removing (%i, %i)\n",x,y);
        QuadTree * parent = this;
        int index;
        QuadTree ** searching = children;
        QuadTree * oldParent;
        while (searching != NULL) {
            //printf("seeking (%i %i) in (%i %i %i %i)\n",x,y,parent->x1,parent->y1,parent->x2,parent->y2);
            oldParent = parent;
            index = parent->getIndex(x,y);
            parent = searching[index];
            if(parent == NULL){
                parent = oldParent;
                //printf("FAILURE!\n");
                break;
            }
            searching = parent->children;
        }
        //printf("seeking (%i %i) in (%i %i %i %i)\n",x,y,parent->x1,parent->y1,parent->x2,parent->y2);
        if(parent->child == obj){
            parent->child = NULL;
            parent->trim(-1);
            //printf("success!\n");
            return true;
        }else if(parent->child == NULL){
            //printf("child null\n");
        }
        //printf("failure?\n");
        return false;
    }
    
    Obj * closestTo(int x, int y){
        int d = 0x0FFFFFFF;
        return closestTo(x, y, d);
    }
    
    Obj * closestTo(int x, int y, int & dist, Obj * best = NULL){
        int bt = y < cy, rt = x > cx;
        
        //exclude if dist is less than best here
        if (x < x1 - dist || x > x2 + dist || y < y1 - dist || y > y2 + dist) {
            //printf("reject %d %d %d %d\n",x < x1 - dist , x > x2 + dist , y < y1 - dist , y > y2 + dist);
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
        if(children == NULL)
            return best;
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        ch = (!bt)*2 + rt;
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        ch = bt*2 + (!rt);
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        ch = (!bt)*2 + (!rt);
        if(children[ch] != NULL) best = children[ch]->closestTo(x,y,dist,best);
        return best;
    }
    
    
};

#endif /* QuadTree_hpp */
