/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"

template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    xMap<T,int>inDegree;
    xMap<T,int>outDegree; 
    xMap<T,bool>visit; 
    int (*hash_code)(T&, int);
    
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0): graph(graph), hash_code(hash_code), inDegree(hash_code), outDegree(hash_code), visit(hash_code) {
        //TODO
        DLinkedList<T> verLst = graph->vertices(); 
    }   
    DLinkedList<T> sort(int mode=0, bool sorted=true){
        //TODO
        if(mode == DFS) return dfsSort(sorted);
        else return bfsSort(sorted);

    }
    DLinkedList<T> bfsSort(bool sorted=true){ 
        //TODO
        DLinkedList<T> ans; 
        this->inDegree = this->vertex2inDegree(this->hash_code); 
        DLinkedList<T> getZero = this->listOfZeroInDegrees(); 
        if(sorted)
        {
            DLinkedListSE<T> order(getZero);
            order.sort(); 
            getZero = order; 
        } 

        Queue<T> q;
        for(T& a : getZero)
        {
             q.push(a); 
        }


        while(!q.empty())
        {
            T travel = q.pop(); 
            ans.add(travel); 
            DLinkedList<T> getTo = graph->getOutwardEdges(travel); 
            DLinkedList<T> inZero; 

            for(T& a: getTo)
            {
                this->inDegree.get(a)--;
                if(this->inDegree.get(a) == 0) 
                {
                    inZero.add(a); 
                }
            }
            if(sorted) 
            {
                DLinkedListSE<T> order(inZero);
                order.sort(); 
                inZero = order; 
            }

            for(T& edge : inZero) 
            {
                q.push(edge); 
            }
        }
        return ans; 
    }

    void dfsWalk(
        T vertice, 
        DLinkedList<T>&ans,
        bool sorted = true)
    {
        Stack<T> s; 
        s.push(vertice); 
        this->visit.get(vertice) = true; 


        while(!s.empty())
        {
            T walk = s.peek();

            if(this->outDegree.get(walk) == 0) 
            {
                ans.add(0,walk);
                DLinkedList<T> getInward = graph->getInwardEdges(walk);
                for(T& ver : getInward) 
                {
                    this->outDegree.get(ver)--; 
                }
                s.pop(); 
             }

            else
            {
                DLinkedList<T> walkOutward = this->graph->getOutwardEdges(walk); 

                if(sorted)
                {
                    DLinkedListSE<T> s(walkOutward); 
                    s.sort(); 
                    walkOutward = s; 
                }     

                for(T out_v : walkOutward)
                {
                    if(this->visit.get(out_v)) continue;
                    s.push(out_v);
                    this->visit.get(out_v) = true; 

                }

            }
        }
    }


    DLinkedList<T>dfsSort(bool sorted = true)
    {
        this->outDegree = vertex2outDegree(this->hash_code);
        this->inDegree = vertex2inDegree(this->hash_code); 
        this->visit = visitMap(this->hash_code);

        DLinkedList<T> listZeros = listOfZeroInDegrees();
        DLinkedList<T> finalAns; 
        Stack<T> s; 
        
        if(sorted) 
        {
            DLinkedListSE<T> sorted(listZeros); 
            sorted.sort(); 
            listZeros = sorted; 
        }

        for(auto listZero : listZeros)
        {
            s.push(listZero);
            visit.get(listZero) = true; 
        }        

        while(!s.empty())
        {
            int travl = s.pop(); 
            dfsWalk(travl,finalAns,sorted); 
        }
        
        return finalAns; 

    }

protected:

    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int))
    {
        xMap<T,int> ans(hash);
        typename AbstractGraph<T>::Iterator it = this->graph->begin();
        while(it != this->graph->end()){
            T vertex = *it;
            int inDegree = this->graph->inDegree(vertex);
            ans.put(vertex, inDegree);
            
            it++;
        }

        return ans; 
    };
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int))
    {
        xMap<T,int> ans(hash);
        typename AbstractGraph<T>::Iterator it = this->graph->begin();
        while(it != this->graph->end()){
            T vertex = *it;
            int inDegree = this->graph->outDegree(vertex);
            ans.put(vertex, inDegree);
            
            it++;
        }

        return ans; 
    }
    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> ans; 
        typename AbstractGraph<T>::Iterator it = this->graph->begin();
        while(it != this->graph->end()){
            T vertex = *it;
            if(this->inDegree.get(vertex) == 0) ans.add(vertex); 
            it++; 
        }
        return ans; 
    }

    xMap<T,bool> visitMap(int (*hash)(T&, int))
    {
        xMap<T,bool> isVisit(hash); 
        typename AbstractGraph<T>::Iterator it = this->graph->begin();
        while(it != this->graph->end()){
            T vertex = *it;
            isVisit.put(vertex, false);
            it++;
        }

        return isVisit; 
    }

}; //TopoSorter


template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


#endif /* TOPOSORTER_H */

