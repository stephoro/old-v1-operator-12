//
//  LinkedList.h
//  SLUG
//
//  Created by Steph on 3/2/16.
//
//

#ifndef __SLUG__LinkedList__
#define __SLUG__LinkedList__

#include <stdio.h>
//#include <string>

//using namespace std;
//
//  LinkedList.cpp
//  SLUG
//
//  Created by Steph on 3/2/16.
//
//





template <typename Obj>
/**
 This is a linked list meant to be iterated over.
 Fast: Removal, Addition, Iteration.
 Not supported: indexing.
 @author Steph
 */
class LinkedList {
public:
    /**
     A simple node for a Linked List
     */
    template <typename Data>
    class Node{
    public:
        /** The stored value */
        Node * next;
        
        /** The next node in the list */
        Node * prev;
        
        /** The previous node in the list */
        Data * obj;
        
        /**
         Construct a Node with next and prev links
         @param obj The stored value
         @param next The next node in the list
         @param prev The previous node in the list
         */
        Node(Data * obj, Node * next = NULL, Node * prev = NULL){
            this->next = next;
            this->prev = prev;
            this->obj = obj;
        }
        
    };
    
private:
    /** The size of this list. */
    int size;
    
    /** The first node of the list. */
    Node<Obj> * head;
    
    /** The currently observed node. */
    Node<Obj> * focus;
    
    void removeAllNodeRefs(){
        Node<Obj> * next = head;
        while (size > 0) {
            if(size > 1){
                next = next->next;
            }
            size--;
            delete head;
            head = next;
        }
        size = 0;
        head = NULL;
        focus = NULL;
    }
    void deleteEverything(){
        Node<Obj> * next = head;
        while (size > 0) {
            if(size > 1){
                next = next->next;
            }
            size--;

            delete head->obj;
            delete head;
            head = next;
        }
        size = 0;
        head = NULL;
        focus = NULL;
    }
public:
    LinkedList() {
        size = 0;
        head = NULL;
        focus = NULL;
    }
    ~LinkedList(){
        removeAllNodeRefs();
    }
    
    /**
     Wipe all data held in the Linked List
     */
    void release(){
        removeAllNodeRefs();
    }
    
    
    /**
     wipes all data and deletes objects.
     */
    void drain(){
        deleteEverything();
    }
  
    
    /**
     Get the size of the list
     */
    int getSize(){
        return size;
    }
    
    /**
     Removes the object from the list. Shrinks size.
     @param node The node to remove.
     */
    void remove (Node<Obj> * node) {
        Node<Obj> * prev = node->prev;
        Node<Obj> * next = node->next;
        prev->next = next;
        next->prev = prev;
        size--;
        if (node == focus)
            focus = prev;
        if (node == head)
            head = next;
        delete node;
    };
    
    /**
     Removes the first element from the list. Shrinks size.
     @return the first element in the list.
     */
    Obj * removeFirst () {
        Node <Obj> * node = head;
        Node<Obj> * prev = node->prev;
        Node<Obj> * next = node->next;
        prev->next = next;
        next->prev = prev;
        size--;
        if (node == focus)
            focus = prev;
        head = next;
        Obj * obj = node->obj;
        delete node;
        return obj;
    };
    
    /**
     Removes the last element from the list. Shrinks size.
     @return the first element in the list.
     */
    Obj * removeLast () {
        Node <Obj> * node = head->prev;
        Node<Obj> * prev = node->prev;
        Node<Obj> * next = node->next;
        prev->next = next;
        next->prev = prev;
        size--;
        if (node == focus)
            focus = prev;
        if (node == head)
            head = next;
        Obj * obj = node->obj;
        delete node;
        return obj;
    };
    
    
    /**
     Gets the first element from the list.
     @return the first element in the list.
     */
    Obj * getFirst () {
        return head->obj;
    };
    
    /**
     Gets the last element from the list.
     @return the first element in the list.
     */
    Obj * getLast () {
        return head->prev->obj;
    };
    
    /**
     Increments the focus.
     @return the next stored value.
     */
    Obj * next () {
        focus = focus->next;
        return focus->obj;
    };
    
    /**
     Decrements the focus.
     @return the previous stored value.
     */
    Obj * prev () {
        focus = focus->prev;
        return focus->obj;
    };
    
    /**
     resets the focus so that the next node will be the head
     */
    void refocus () {
        if (size >= 1)
            focus = head->prev;
    };
    
    /**
     Adds an object to the linked list at the last place.
     @param obj The value to be stored
     @return A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node<Obj> * add (Obj *obj) {
        Node<Obj> * node = NULL;
        if (size == 0) {
            node = head = new Node<Obj>(obj);
            head->next = head;
            head->prev = head;
            focus = head;
        } else {
            Node<Obj> * prev = head->prev;
            node = new Node<Obj>(obj, head, prev);
            head->prev = node;
            prev->next = node;
        }
        size++;
        return node;
    };
    
    
    /**
     Adds an object to the linked list at the first place.
     @param obj The value to be stored
     @return A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node<Obj> * addToFront (Obj *obj) {
        if (size == 0) {
            return add(obj);
        } else {
            Node<Obj> * node = NULL;
            Node<Obj> * prev = head->prev;
            node = new Node<Obj>(obj, head, prev);
            head->prev = node;
            prev->next = node;
            head = node;
            size++;
            return node;
        }
    };
    
    /**
     Adds an object to the linked list after the specified node.
     @param after The node after which the new node is to be placed.
     @param obj The value to be stored
     @return A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node<Obj> * addAfter (Node<Obj> * after, Obj *obj) {
        Node<Obj> * next = after->next;
        Node<Obj> * prev = after;
        Node<Obj> * node = new Node<Obj>(obj, next, prev);
        next->prev = node;
        prev->next = node;
        size++;
        return node;
    };
    
    /**
     Adds an object to the linked list before the specified node.
     @param before The node before which the new node is to be placed.
     @param obj The value to be stored
     @return A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node<Obj> * addBefore (Node<Obj> * before, Obj *obj) {
        Node<Obj> * next = before;
        Node<Obj> * prev = before->prev;
        Node<Obj> * node = new Node<Obj>(obj, next, prev);
        next->prev = node;
        prev->next = node;
        size++;
        return node;
    };
};

#endif /* defined(__SLUG__LinkedList__) */

