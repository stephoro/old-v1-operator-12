//
//  BinaryHeap.hpp
//  SLUG
//
//  Created by Steph on 7/14/16.
//
//

#ifndef BinaryHeap_hpp
#define BinaryHeap_hpp

#include <stdio.h>


template <typename Obj>
class BinaryHeap {
private:
    Obj ** nodes;
    int * values;
    int size;
    int original;
    int fill;
    void resize(){
        int old = size;
        size <<= 1;
        Obj ** oldArray = nodes;
        int * oldIndex = values;
        nodes = new Obj*[size];
        values = new int[size];
        while(old--){
            nodes[old] = oldArray[old];
            values[old] = oldIndex[old];
        }
    }
    
public:
    BinaryHeap(int size = 32){
        if(size <= 0)
            size = 2;
        original = size;
        fill = 0;
        this->size = size;
        nodes = new Obj*[original];
        values = new int[original];
        
    };
    
    ~BinaryHeap(){
        if(nodes != NULL)
            delete [] nodes;
        if(values != NULL)
            delete [] values;
    }
    
    
    void printIndexes(int root, int s){
        int b = s;
        while(b--)
            printf(" ");
        if(root >= fill || values == NULL){
            printf("(NULL)\n");
            return;
        }
        printf("%i\n",values[root]);
        printIndexes((root << 1) + 1, s + 2);
        printIndexes((root << 1) + 2, s + 2);
        
    }
    
    
    void add(Obj * toAdd, int value){
        if(nodes == NULL){
            nodes = new Obj*[original];
            values = new int[original];
            original = size;
        }
        if(fill == size)
            resize();
        nodes[fill] = toAdd;
        values[fill] = value;
        int child = fill;
        fill++;
        int parent = child >> 1;
        int pval = values[parent];
        Obj * temp;
        while(pval > value){
            values[parent] = value;
            values[child] = pval;
            
            temp = nodes[parent];
            nodes[parent] = nodes[child];
            nodes[child] = temp;
            
            child = parent;
            parent = child >> 1;
            pval = values[parent];
        }
            
        
    }
    //assumes the tree is proper size and that
    Obj * remove(){
        if(fill == 0)
            return NULL;
        Obj * ret = nodes[0];
        nodes[0] = nodes[--fill];
        Obj * temp;
        int pval = values[0] = values[fill];
        if(fill > 2){
            int parent = 0;
            int left = 1;
            int right = 2;
            int lval = values[1], rval = values[2];
            int min = left, mval = lval;
            if(rval < lval){
                min = right;
                mval = rval;
            }
            while (pval > mval) {
                
                values[parent] = mval;
                values[min] = pval;
                temp = nodes[parent];
                nodes[parent] = nodes[min];
                nodes[min] = temp;
                
                
                parent = min;
                left = (parent << 1) + 1;
                right = (parent << 1) + 2;
                if(right >= fill)
                    break;
                lval = values[left];
                rval = values[right];
                min = left;
                mval = lval;
                if(rval < lval){
                    min = right;
                    mval = rval;
                }
            }
            if(fill == right){
                mval = values[left];
                if(mval < pval){
                    values[parent] = mval;
                    values[left] = pval;
                    
                    temp = nodes[parent];
                    nodes[parent] = nodes[left];
                    nodes[left] = temp;
                }
                    
            }
        }else if(fill == 2){
            int mval = values[1];
            if(mval < pval){
                values[0] = mval;
                values[1] = pval;
                temp = nodes[0];
                nodes[0] = nodes[1];
                nodes[1] = temp;
            }
        }
        return ret;
    }
    
    void release(){
        if(nodes != NULL)
            delete [] nodes;
        if(values != NULL)
            delete [] values;
        nodes = NULL;
        values = NULL;
    }
    
    void drain(){
        if(values != NULL)
            delete [] values;
        values = NULL;
        for(int i = 0; i < size; i++)
            delete values[i];
        if(values != NULL)
            delete [] values;
        nodes = NULL;
    }
    
    int search(Obj * thing){
        int l = fill;
        while(l--){
            if(nodes[l] == thing)
                return l;
        }
    }
    
    int getFill(){
        return fill;
    }
    
    //meant only if new value is smaller
    void rescoreIndex(int index, int value){
        int parent = index >> 1;
        int pval = values[parent];
        Obj * temp;
        while(pval > value){
            values[parent] = value;
            values[index] = pval;
            
            temp = nodes[parent];
            nodes[parent] = nodes[index];
            nodes[index] = temp;
            
            index = parent;
            parent = index >> 1;
            pval = values[parent];
        }
    }
    
    
};


#endif /* BinaryHeap_hpp */
