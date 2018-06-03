//
//  LinkedList.cpp
//  SLUG
//
//  Created by Steph on 3/2/16.
//
//

#include "LinkedList.h"


template <typename Obj>


/**
 A simple node for a Linked List
 */
class Node{
public:
    /** The stored value */
    Node * &next;
    
    /** The next node in the list */
    Node * &prev;
    
    /** The previous node in the list */
    Obj * &obj;
    
    
};

LinkedList() {
    LinkedList::size = 0;
    head = NULL;
    focus = NULL;
}

/**
 Wipe all data held in the Linked List
 */
void LinkedList::release(){
    size = 0;
    head = NULL;
    focus = NULL;
}

/*
 Get the Linked list as an array
 @return {Array} The values stored from head to tail.
 LinkedList.prototype.toArray = function () {
 var array = new Array(this.size);
 var focus = this.head;
 for (var i = 0; i < this.size; i++) {
 array[i] = focus.obj;
 focus = focus.next;
 }
 return array;
 };
 */

/**
 Removes the object from the list. Shrinks size.
 @param node The node to remove.
 */
void remove (Node * &node) {
    Node * &prev = node->prev;
    Node * &next = node->next;
    prev->next = next;
    next->prev = prev;
    size--;
    if (node == focus)
        focus = prev;
    if (node == head)
        head = next;
};

/**
 Removes the first element from the list. Shrinks size.
 @return the first element in the list.
 */
Obj removeFirst () {
    Node node = head;
    Node * &prev = node.prev;
    Node * &next = node.next;
    prev->next = next;
    next->prev = prev;
    size--;
    if (node == focus)
        focus = prev;
    head = next;
    return node.obj;
};

/**
 Removes the last element from the list. Shrinks size.
 @return the first element in the list.
 */
Obj removeLast () {
    Node node = head;
    Node * &prev = node.prev;
    Node * &next = node.next;
    prev->next = next;
    next->prev = prev;
    size--;
    if (node == focus)
        focus = prev;
    if (node == head)
        head = next;
    return node.obj;
};


/**
 Gets the first element from the list.
 @return the first element in the list.
 */
Obj getFirst () {
    return head->obj;
};

/**
 Gets the last element from the list.
 @return the first element in the list.
 */
Obj getLast () {
    return head->prev->obj;
};

/**
 Increments the focus.
 @return the next stored value.
 */
Obj next () {
    if (size < 1)
        return NULL;
    focus = focus->next;
    return focus->obj;
};

/**
 Decrements the focus.
 @return the previous stored value.
 */
Obj prev () {
    if (size < 1)
        return NULL;
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
Node * &add (Obj obj) {
    Node * node = NULL;
    if (size == 0) {
        node = head = new Node(obj);
        head->next = head;
        head->prev = head;
        focus = head;
    } else {
        Node * &next = head;
        Node * &prev = head->prev;
        node = new Node(obj, next, prev);
        next->prev = node;
        prev->next = node;
    }
    size++;
    return node;
};

/**
 Adds an object to the linked list after the specified node.
 @param after The node after which the new node is to be placed.
 @param obj The value to be stored
 @return A reference to the object's node in the list.
 Use this for removal from the list.
 */
Node * &addAfter (Node * &after, Obj obj) {
    Node * &next = after->next;
    Node * &prev = after;
    Node * &node = new Node(obj, next, prev);
    next->prev = node;
    prev->next = node;
    size++;
    return node;
};

/**
 Adds an object to the linked list before the specified node.
 @param {LinkedList.Node} before The node before which the new node is to be placed.
 @param {Object} obj The value to be stored
 @return {LinkedList.Node} A reference to the object's node in the list.
 Use this for removal from the list.
 */
Node * &addBefore (Node * &before, Obj obj) {
    Node * &next = before;
    Node * &prev = before->prev;
    Node * &node = new Node(obj, next, prev);
    next->prev = node;
    prev->next = node;
    size++;
    return node;
};
};



