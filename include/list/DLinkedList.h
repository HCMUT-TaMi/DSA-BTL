/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }

    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
/////////////////////////////////////////////////////////////////////
//  Define BWD Iterator
    class BWDIterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;
    public:
        BWDIterator(DLinkedList<T> *pList = 0, bool end = true)
        {
            if (end)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->head;
                else
                    pNode = 0;

            }

            this->pList = pList;
        }

        BWDIterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }

        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pPrev = pNode->next; // MUST next, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pPrev;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix -- overload
        BWDIterator &operator--()
        {
            pNode = pNode->prev;
            return *this;
        }
        // Postfix ++ overload
        BWDIterator operator--(int)
        {
            BWDIterator iterator = *this;
            --*this;
            return iterator;
        }
    };


};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;




//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO, ASSIGN THE VALUE
    this->deleteUserData = deleteUserData; 
    this->itemEqual = itemEqual;

    // Generate Empty Node
    this->head = new DLinkedList<T>::Node(); 
    this->tail = new DLinkedList<T>::Node(); 

    // Avoid the ambigous
    this->tail->prev = head;
    this->head->next = tail; 

    count = 0; 
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO ASSIGN THE VALUE
    this->head = new DLinkedList<T>::Node(); 
    this->tail = new DLinkedList<T>::Node(); 
    // Avoid the ambigous
    this->tail->prev = head;
    this->head->next = tail; 
    //  Copy the val 
    this->copyFrom(list); 
}


template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    //  TODO
    //  Clear the data before assign the value 
    if(!this->head)
    {
        // Generate Empty Node
        this->head = new DLinkedList<T>::Node(); 
        this->tail = new DLinkedList<T>::Node(); 

        // Avoid the ambigous
        this->tail->prev = head;
        this->head->next = tail; 

        count = 0; 
    }

    if(this != &list && list.head)
    {
        this->clear(); 
        this->copyFrom(list); 
    }

    return *this; 
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    this->clear();
    delete head; 
    delete tail; 
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    DLinkedList<T>::Node* new_Node = new DLinkedList<T>::Node(e,nullptr,nullptr); 
    
    // get the prev_Node of Tail
    DLinkedList<T>::Node* prev_tail = tail->prev; 

    // assign new_Node with prev_Node 
    new_Node->prev = prev_tail; 

    // getting prev_Node to point to the next new_Node
    prev_tail->next = new_Node; 

    // ensure that the last Node is there with new_Node and new_Node->next
    tail->prev = new_Node; 
    new_Node->next = tail; 

    // count!! 
    count++; 
}
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO

    /*
        ADD BACKWARD HEAD->END, travel = HEAD -> move to END, then it create HEAD->value->END
    */

    //  Check for valid index if not bai bai
    if(index > count || index < 0) {throw std::out_of_range("xai index khac ba oi!");}

    DLinkedList<T>::Node* new_Node = new DLinkedList<T>::Node(e,nullptr,nullptr); 

    // iteration through the array 
    DLinkedList<T>::Node* travel = this->getPreviousNodeOf(index);

    // Got the address of the Node we want to travel to !
    DLinkedList<T>::Node* at = travel;
    DLinkedList<T>::Node* at_prev = at->prev;  

    //  Assign new_Node prev and next and at_prev->next 
    new_Node->next = at; 
    new_Node->prev = at_prev; 
    at_prev->next = new_Node; 

    //  Change at->previous pointer 
    at->prev = new_Node;

    //  Count! 
    count++; 
}
template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>:: getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    // TODO
    /*
        BEGIN: 0->1->2->...->N-1->END(or N position!) 
        ->  Travel like normal 
    */

    //  Second Half case 
    if(index > count/2) 
    {
        DLinkedList<T>::Node* travel = this->tail; 

        //loops to find index 
        for(int i = this->count; i > index; i--) 
        {
            travel = travel->prev; 
        }
        
        return travel;
    }

    DLinkedList<T>::Node* travel = this->head->next; 

    //loops to find index 
    for(int i = 0; i < index; i++) 
        travel = travel->next; 
    
    return travel;   
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if(index < 0 || index >= count) throw std::out_of_range("out of range!");

    // Iteration for finding the position !
    DLinkedList<T>::Node* pNode = getPreviousNodeOf(index); 

    // Remove it out of List 
    pNode->prev->next = pNode->next;
    pNode->next->prev = pNode->prev;
    T store = pNode->data; 

    //Remove pNode
    delete pNode; 
    this->count--;

    return store; 
}


template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return count == 0; 
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count; 
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    if(empty()) return; 

    // Loops through all the lists, there is no it++, because it.remove will
    // get the node to move to next node 
    Node *travel = head->next; 

    if(deleteUserData != nullptr)
        deleteUserData(this); 

    while(travel != tail)
    { 

        //store the next pos 
        Node* travel_next = travel->next; 
        
        //delete mission 
        delete travel; 
        travel = travel_next; 
    }

    // Ensure that the list is still exists 
    this->head->next = this->tail; 
    this->tail->prev = this->head; 

    // Ensure that count is reset 
    this->count = 0; 
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    Node* travel = this->getPreviousNodeOf(index); 
    return travel->data;   
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    DLinkedList<T>::Iterator it = this->begin(); 

    // Loops to find index
    int i = 0;  
    while (it != this->end())  // Ensuring it doesn't go beyond the list
    {
        // Check if user-defined function of compare exists!
        if (itemEqual == nullptr) 
        {
            if (*it == item) 
            {
                return i;  
            } 
        }
        else 
        {
            if (itemEqual(*it, item)) 
            {
                return i; 
            }
        }

        // Increment iterator and index only after comparison
        ++it;
        ++i;
    }

    // Cannot find the value
    return -1; 

}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO O(N)
    
    //  default case
    if(empty()) return false;

    Node* travel = this->head->next; 

    //  travel until find-out the destinations
    while(travel != this->tail)
    {
        if(itemEqual == 0)
        {
            if(travel->data == item) break; 
        }

        else
        {
            if(itemEqual(travel->data,item)) break; 
        }

        //  Ensuring the case
        travel = travel->next; 
    }

    if(travel == this->tail) return false; 

    //  preparation for deletions
    travel->prev->next = travel->next; 
    travel->next->prev = travel->prev; 

    //  delete state
    if(removeItemData) removeItemData(travel->data); //do not pass item -> different address
    delete travel;

    //  size ensuring 
    this->count--;

    return true; 
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    int res = this->indexOf(item); 
    return !(res == -1); 
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    std::ostringstream ss;
    ss << "["; 

    //  intial setup
    DLinkedList<T>::Iterator it = this->begin(); 

    if(it != end())
    {
        if(item2str == 0) 
        {
            ss << *it; 
        }

        else 
        {
            ss << item2str(*it); 
        }
    

        //  loop setup
        for(it = ++begin(); it != end(); it++) 
        {
            if(item2str == 0)
            { 
                ss << ", "; 
                ss << *it; 
            }

            else 
            {
                ss << ", " ;
                ss << item2str(*it); 
            }
        } 
    }

    ss << "]";

    return ss.str(); 
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    // TODO
 
    if(list.head)
    {
        this->count = 0; 
        this->deleteUserData = list.deleteUserData; 
        this->itemEqual = list.itemEqual;
        Node* tmp_head = list.head->next; 
        // Node* tmp_tail = list.tail; 

        while(tmp_head != list.tail)
        { 
            //  add will add size + 1 
            this->add(tmp_head->data);
            tmp_head = tmp_head->next; 
        }
    
    }

}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO

    this->clear(); //still not understand
}

#endif /* DLINKEDLIST_H */
