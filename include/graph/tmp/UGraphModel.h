/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* u = this->getVertexNode(from); 
        typename AbstractGraph<T>::VertexNode* v = this->getVertexNode(to); 
        if(!u) throw VertexNotFoundException(this->vertex2str(from)); 
        if(!v) throw VertexNotFoundException(this->vertex2str(to)); 
        u->connect(v,weight); 
        v->connect(u,weight); 
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* u = this->getVertexNode(from); 
        typename AbstractGraph<T>::VertexNode* v = this->getVertexNode(to); 
        if(!u) throw VertexNotFoundException(this->vertex2str(from));
        if(!v) throw VertexNotFoundException(this->vertex2str(to));  
        typename AbstractGraph<T>::Edge e(u,v);

        if(!u->getEdge(v)) throw EdgeNotFoundException(this->edge2Str(e)); 
        u->removeTo(v);
        v->removeTo(u); 
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *rem = this->getVertexNode(vertex); 
        if(!rem) throw VertexNotFoundException(this->vertex2str(vertex)); 

        typename DLinkedList<typename AbstractGraph<T>::VertexNode*>::Iterator it = this->nodeList.begin();
        while(it != this->nodeList.end()){

            typename AbstractGraph<T>::VertexNode *Node = *it;

            //  Ensure rem ===> Node removed 
            rem->removeTo(Node);

            if(this->vertexEQ(Node->getVertex(),vertex)) 
            {   
                it.remove();
            }

            //  Ensure Node ====> rem removed
            else Node->removeTo(rem);

            it++;
        }

        delete rem; 

    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T>* newModel = new DGraphModel(vertexEQ,vertex2str); 

        for(int i = 0; i < nvertices; i++)
            newModel->add(vertices[i]); 

        for(int i = 0; i < nedges; i++)
        {
            newModel->connect(edges[i].from,edges[i].to,edges[i].weight); 
        } 

        return newModel; 

    }
};

#endif /* UGRAPHMODEL_H */
