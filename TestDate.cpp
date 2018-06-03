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
    class Node{
    public:
        
        
        /**
         Construct a Node with next and prev links
         @param obj The stored value
         @param next The next node in the list
         @param prev The previous node in the list
         */
        Node(Obj * &obj, Node * &next = NULL, Node * &prev = NULL);
    };
private:
    /** The size of this list. */
    int size;
    
    /** The first node of the list. */
    Node * head;
    
    /** The currently observed node. */
    Node * focus;
public:
    LinkedList();
    
    /**
     Wipe all data held in the Linked List
     */
    void release();
    
    /**
     Removes the object from the list. Shrinks size.
     @param node The node to remove.
     */
    void remove (Node * &node);
    
    /**
     Removes the first element from the list. Shrinks size.
     @return the first element in the list.
     */
    Obj removeFirst ();
    
    /**
     Removes the last element from the list. Shrinks size.
     @return the first element in the list.
     */
    Obj removeLast ();
    
    /**
     Gets the first element from the list.
     @return the first element in the list.
     */
    Obj getFirst ();
    
    /**
     Gets the last element from the list.
     @return the first element in the list.
     */
    Obj getLast ();
    
    /**
     Increments the focus.
     @return the next stored value.
     */
    Obj next ();
    
    /**
     Decrements the focus.
     @return the previous stored value.
     */
    Obj prev ();
    
    /**
     resets the focus so that the next node will be the head
     */
    void refocus ();
    
    /**
     Adds an object to the linked list at the last place.
     @param obj The value to be stored
     @return A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node * &add (Obj obj);
    
    /**
     Adds an object to the linked list after the specified node.
     @param after The node after which the new node is to be placed.
     @param obj The value to be stored
     @return A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node * &addAfter (Node * &after, Obj obj);
    
    /**
     Adds an object to the linked list before the specified node.
     @param {LinkedList.Node} before The node before which the new node is to be placed.
     @param {Object} obj The value to be stored
     @return {LinkedList.Node} A reference to the object's node in the list.
     Use this for removal from the list.
     */
    Node * &addBefore (Node * &before, Obj obj);
};
