/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   CrossEntropy.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction): ILossLayer(reduction){
}

CrossEntropy::CrossEntropy(const CrossEntropy& orig):
ILossLayer(orig){
}

CrossEntropy::~CrossEntropy() {
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t){
    //  YOUR CODE IS HERE
    this->m_aCached_Ypred = X;
    this->m_aYtarget = t;

    return cross_entropy(m_aCached_Ypred,m_aYtarget,true);
}
xt::xarray<double> CrossEntropy::backward() {
    //YOUR CODE IS HERE
    //  return the deltaY 
    xt::xarray<double> deltaY; 
    deltaY = -1 * (this->m_aYtarget / (this->m_aCached_Ypred + 1e-7));

    return deltaY/deltaY.shape()[0]; 
}