/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int seed; 
    /*TODO: add more member variables to support the iteration*/

    int num_of_batch; 
    int data_left; 
    xt::xarray<int>  index_array; 
    int size; 
    bool label_exit; 
    bool need; 

public:
    class Iterator; 
    DataLoader(Dataset<DType, LType>* ptr_dataset,
            int batch_size,
            bool shuffle=true,
            bool drop_last=false, 
            int seed = -1){
        /*TODO: Add your code to do the initialization */

        this->ptr_dataset = ptr_dataset; 
        this->batch_size = batch_size; 
        this->shuffle = shuffle; 
        this->drop_last = drop_last; 
        this->seed = seed; 

        //  Create batch USER
        this->size = ptr_dataset->len(); 
        this->num_of_batch = size / batch_size; 
        this->data_left = size%batch_size;
        this->label_exit = !ptr_dataset->get_label_shape().empty();

        //  Handle new_xt case 
        this->need = ((data_left != 0) && !drop_last); 
        // if(need) this->num_of_batch++; 

        //  Handle for shuffle cases here
        index_array = xt::arange(size);  

        if(shuffle) //  If the Shuffle is true, then the index_array change it value randomly !
        {
            if(seed >= 0) xt::random::seed(seed);
            xt::random::shuffle(index_array);
        }


    }
    int get_batch_size(){return batch_size;}
    int get_sample_count(){return ptr_dataset->len();}
    int get_total_batch(){return int(ptr_dataset->len()/batch_size);}

    virtual ~DataLoader()
    {
    }
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
    
    /*TODO: Add your code here to support iteration on batch*/

    class Iterator
    {
        private:

            // Batch<DType, LType>* batches;
            // int cursor;  
            
            //back up case 
            DataLoader<DType,LType> * it; 
            int cursor; 
            
        public:

        //  Iterator( Batch<DType, LType>* batches, int cursor) : batches(batches), cursor(cursor) {} 
        //  backup 

        Iterator(DataLoader<DType,LType>* it, int cursor) : it(it),cursor(cursor){}
        ~Iterator(){} 
        Iterator &operator=(const Iterator &iterator)
        {

            // this->batches = iterator.batches; 
            this->cursor = iterator.cursor;

            //backup 
            this->it = it;

            return *this;
            
        }

        Batch<DType,LType> operator*()
        {
            // return batches[cursor]; 

            /*=======================================
                        BACK-UP STAGE
            =========================================*/

            //  Get number of Index 
            int get_batch_size = it->batch_size; 
            
            if(cursor == it->num_of_batch - 1 && it->need) 
            {
                get_batch_size = it->data_left + it->batch_size;
            }
            
            //  Batch size create
            auto batch_size_data = it->ptr_dataset->get_data_shape(); 
            batch_size_data[0] = get_batch_size; 

            //  Check if Label is exit! 
            auto batch_size_label = it->ptr_dataset->get_label_shape();  
            batch_size_label[0] = get_batch_size; 

            //  Create Storage 
            xt::xarray<DType> batch_data(batch_size_data); 
            xt::xarray<LType> batch_label;
            if(it->label_exit) 
                batch_label = xt::xarray<LType>(batch_size_label);

            //  Concat Stage
            int get_first_index =  cursor*it->batch_size;
            for(int i = 0; i < get_batch_size; i++)
            { 
                //  get Real Index after the shuffle (or not)
                int real_index = it->index_array[get_first_index + i];

                //  get the Data
                DataLabel<DType,LType> get_value = it->ptr_dataset->getitem(real_index); 

                //  Assign value that we need !
                xt::view(batch_data,i,xt::all()) = get_value.getData(); 
                if(it->label_exit)
                    xt::view(batch_label,i,xt::all()) = get_value.getLabel();
            }

            //  Return the Batch 
            Batch<DType,LType> batch(batch_data,batch_label);
            return batch; 

        }
        bool operator!=(const Iterator &iterator)
        {
            return this->cursor != iterator.cursor; 
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            this->cursor++; 
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
    Iterator begin()
    {
        return Iterator(this,0);
    }

    Iterator end()
    { 
        return Iterator(this,this->num_of_batch);
    }

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

