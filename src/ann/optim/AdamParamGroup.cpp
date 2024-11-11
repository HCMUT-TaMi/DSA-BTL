/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   AdamParamGroup.cpp
 * Author: ltsach
 * 
 * Created on October 8, 2024, 1:43 PM
 */

#include "optim/AdamParamGroup.h"

AdamParamGroup::AdamParamGroup(double beta1, double beta2):
    m_beta1(beta1), m_beta2(beta2){
    //Create some maps:
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pFirstMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    m_pSecondMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    //
    m_step_idx = 1;
    m_beta1_t = m_beta1;
    m_beta2_t = m_beta2;
}

AdamParamGroup::AdamParamGroup(const AdamParamGroup& orig):
    m_beta1(orig.m_beta1), m_beta2(orig.m_beta2){
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pFirstMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    m_pSecondMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    //copy:
    *m_pParams = *orig.m_pParams;
    *m_pGrads = *orig.m_pGrads;
    *m_pFirstMomment = *orig.m_pFirstMomment;
    *m_pSecondMomment = *orig.m_pSecondMomment;
    //
    m_step_idx = 1;
    m_beta1_t = m_beta1;
    m_beta2_t = m_beta2;
}

AdamParamGroup::~AdamParamGroup() {
    if(m_pFirstMomment != nullptr) delete m_pFirstMomment;
    if(m_pSecondMomment != nullptr) delete m_pSecondMomment;
}

void AdamParamGroup::register_param(string param_name, 
        xt::xarray<double>* ptr_param,
        xt::xarray<double>* ptr_grad){
    //YOUR CODE IS HERE 
    this->m_pParams->put(param_name,ptr_param);
    this->m_pGrads->put(param_name,ptr_grad); 
    this->m_pFirstMomment->put(param_name,new double_tensor); 
    this->m_pSecondMomment->put(param_name,new double_tensor); 
}
void AdamParamGroup::register_sample_count(unsigned long long* pCounter){
    m_pCounter = pCounter;
}

void AdamParamGroup::zero_grad(){
    //YOUR CODE IS HERE
    DLinkedList<string> keys = this->m_pGrads->keys(); 
    for(auto key : keys)
    {
        xt::xarray<double>* P = this->m_pParams->get(key);
        xt::xarray<double>* grad = this->m_pGrads->get(key); 
        xt::xarray<double>* m1 = this->m_pFirstMomment->get(key); 
        xt::xarray<double>* m2 = this->m_pSecondMomment->get(key); 
        *grad = xt::zeros<double>(P->shape());
        *m1 = xt::zeros<double>(P->shape()); 
        *m2 = xt::zeros<double>(P->shape()); 
    }
    m_beta1_t = m_beta1; 
    m_beta2_t = m_beta2; 
    m_step_idx = 0; 
    *m_pCounter = 0;
}

void AdamParamGroup::step(double lr){
    //YOUR CODE IS HERE
    DLinkedList<string> keys = this->m_pGrads->keys(); 
    for(auto key : keys)
    {
        xt::xarray<double>& P = *this->m_pParams->get(key);
        xt::xarray<double>& grad = *this->m_pGrads->get(key); 
        xt::xarray<double>& m1 = *this->m_pFirstMomment->get(key); 
        xt::xarray<double>& m2 = *this->m_pSecondMomment->get(key); 
        
        m1 = m_beta1 * m1 + (1-m_beta1) * grad; 
        m2 = m_beta2 * m2 + (1-m_beta2) * grad * grad; 

        xt::xarray<double> m1_hat = m1 / (1-m_beta1_t+1e-7);
        xt::xarray<double> m2_hat = m2 / (1-m_beta2_t+1e-7); 

        xt::xarray<double> tmp = m1_hat / xt::sqrt(m2_hat + 1e-7); 
        P = P - tmp * lr; 
    }
    //UPDATE step_idx:
    m_step_idx += 1;
    m_beta1_t *= m_beta1;
    m_beta2_t *= m_beta2;
}
