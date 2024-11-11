/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    // //  User_defied!
    // Batch(){};

    Batch(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    virtual ~Batch(){}
    xt::xarray<DType>& getData(){return data; }
    xt::xarray<LType>& getLabel(){return label; }
};


template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};
    
    virtual int len()=0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;
    
};

//////////////////////////////////////////////////////////////////////
template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType>{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
    
public:
    /* TensorDataset: 
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
    */

    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label){
        /* TODO: your code is here for the initialization
         */
        xt::svector<unsigned long> tmp_data_shape = xt::svector(data.shape()); 
        xt::svector<unsigned long> tmp_label_shape = xt::svector(label.shape());
        bool isData = (!data.shape().empty());
        bool Case1 = (tmp_data_shape[0] == tmp_label_shape[0]);
        bool Case2 =  (label.shape().empty());
        if(isData && Case1)
        {
            this->data = data;  
            this->label = label; 
            this->data_shape = xt::svector(data.shape()); 
            this->label_shape = xt::svector(label.shape()); 
        }
        if(isData && Case2)
        {
            this->data = data; 
            this->data_shape = xt::svector(data.shape()); 
            this->label_shape = xt::svector(label.shape()); 
        }
    }
    /* len():
     *  return the size of dimension 0
    */
    int len(){
        /* TODO: your code is here to return the dataset's length
         */
        return data_shape[0]; 

        // return 0; //remove it when complete
    }
    
    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index){
        /* TODO: your code is here
         */ 

        //  Ensure of valid Index! 
        if (index < 0 || index > len())  
        {
            throw std::out_of_range("The index you select is out of range"); 
        }

        xt::xarray<DType> get_value_reshape = xt::view(data,index,xt::all()); 
        xt::xarray<LType> get_label_reshape;
        if(!this->label.shape().empty())
             get_label_reshape = xt::view(label,index,xt::all()); 

        //  Convert the Data into 2D data !!! -> back to 1D data by flatten it
        // get_value_reshape = xt::reshape_view(get_value_reshape,{1,data_shape[1]}); 
        // get_label_reshape = xt::reshape_view(get_label_reshape,{1,label_shape[1]});
        DataLabel<DType,LType> result(get_value_reshape,get_label_reshape);  
        return result; 
    }
    
    xt::svector<unsigned long> get_data_shape(){
        /* TODO: your code is here to return data_shape
         */
        return this->data_shape; 
    }
    xt::svector<unsigned long> get_label_shape(){
        /* TODO: your code is here to return label_shape
         */
        return this->label_shape; 
    }
};

#endif /* DATASET_H */

