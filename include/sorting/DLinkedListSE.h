/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
using node = typename DLinkedList<T>::Node; 

template<class T>
class DLinkedListSE: public DLinkedList<T>{
private: 
    int (*Icomparator)(T&,T&); 
public:
    
    DLinkedListSE(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*itemEQ)(T&, T&)=0 ) : 
            DLinkedList<T>(removeData, itemEQ){
        
    };
    
    DLinkedListSE(const DLinkedList<T>& list){
        this->copyFrom(list);
    }
    
    void sort(int (*comparator)(T&,T&)=0){
        //TODO: implement this function
        //     - You should implement the merge sort algorithm

        if(this->empty() || this->size() == 1) return ;

        //  Ensure that the List is disconnected !
        this->tail->prev->next = nullptr; 

        //  Run from here 
        this->head->next = mergeSort(this->head->next,comparator); 
        this->head->next->prev = this->head;  
        node<T>* travel = this->head; 
        while(travel->next != nullptr) 
        {
                travel = travel -> next; 
        }

        travel->next = this->tail;
        this->tail->prev = travel; 
    };
    
protected:

    node<T>* half(node<T>* slow)
    {
        node<T>* fast = slow; 

        while (fast && fast->next && fast->next->next)
        {
            fast = fast->next->next; 
            slow = slow->next; 
        }

        node<T> *temp = slow->next;
        slow->next = NULL;
        if (temp != NULL) {
            temp->prev = NULL;
        }

        return temp;
    }

    node<T>* merge(node<T>* left, node<T>* right, int (*comparator)(T&,T&)=0) 
    {
        if(!left) return right; 
        if(!right) return left; 

        //  If right > left 
        if(compare(left->data,right->data,comparator) == -1) 
        {
            left->next = merge(left->next,right,comparator);
            if(left->next)
            {
                left->next->prev = left; 
            }
            left->prev = nullptr; 
            return left; 
        }

        else 
        {
            right->next = merge(left,right->next,comparator); 
            if (right->next) {
                right->next->prev = right;
             }
            right->prev = nullptr;
            return right;
        }
    }

    node<T>* mergeSort(node<T>* start, int (*comparator)(T&,T&)=0) 
    {
        if(!start || !start->next) return start; 

        node<T>* mid = half(start);
        node<T>* left = mergeSort(start,comparator);
        node<T>* right = mergeSort(mid,comparator);  
        return merge(left,right,comparator); 
    }


    static int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)=0){
        if(comparator != 0) return comparator(lhs, rhs);
        else{
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */

