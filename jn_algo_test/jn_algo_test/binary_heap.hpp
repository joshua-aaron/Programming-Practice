//
//  binary_heap.h
//  jn_algo_test
//
//  Created by Joshua Navon on 7/22/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#ifndef heap_hpp
#define heap_hpp

namespace clrs_impl_jn {

    template <typename T, typename FwdIt>
    class Min_bin_heap {
    public:
        Min_bin_heap();
        explicit Min_bin_heap(T);
        ~Min_bin_heap();
        Min_bin_heap(const Min_bin_heap& heap);
        Min_bin_heap& operator=(const Min_bin_heap&);
        Min_bin_heap(Min_bin_heap&& heap);
        Min_bin_heap& operator=(Min_bin_heap&&);
        std::size_t parent(size_t);
        std::size_t left(size_t);
        std::size_t right(size_t);
        bool operator==(Min_bin_heap&);
        bool operator!=(Min_bin_heap&);
        void insert(T);
        void find(T);
        T extract_min();
    private:
    	std::size_t size;
        T* data;
    };

    template <typename T, typename FwdIt>
    class Max_bin_heap {
    public:
        Max_bin_heap();
        explicit Max_bin_heap(T);
        Max_bin_heap(FwdIt,FwdIt);
        ~Max_bin_heap();
        Max_bin_heap(const Max_bin_heap&);
        Max_bin_heap& operator=(const Max_bin_heap&);
        Max_bin_heap(Max_bin_heap&&);
        Max_bin_heap& operator=(Max_bin_heap&&);
        std::size_t parent(size_t);
        std::size_t left(size_t);
        std::size_t right(size_t);
        void max_heapify(T*,std::size_t);
        T& operator[](size_t);
        const T& operator[](size_t) const;
        void insert(T);
        void increase_key(size_t, T);
        bool find(T) const;
        T max() const;
        T extract_max();
        std::size_t get_size() const;
        void to_sorted_array(T*);
    private:
    	std::size_t size;
    	T* data;
    };

    // Make an empty heap;
    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>::Max_bin_heap() : size{0}, data{nullptr} {}

    // Initialize the heap with a singe key
    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>::Max_bin_heap(T key) : size{1}, data{new T[1]} {
        *this->data = key;
    }

    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>::Max_bin_heap(FwdIt begin, FwdIt end) {
        auto const size = std::distance(begin, end);
        this->size = size;
        this->data = new T[size];
        size_t idx = 0;
        for (FwdIt iter = begin; iter != end; iter++)
            data[idx++] = *iter;
        for (int ii = size / 2; size >= 0; ii--)
            max_heapify(data, ii);
    }

    // Calls delete_node, which works recursively, and then deletes the tree's root node
    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>::~Max_bin_heap() {
        delete[] this->data;
    }

    // Deep copy
    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>::Max_bin_heap(const Max_bin_heap& old_heap) {
        if (old_heap.data == nullptr) {
            this->size = 0;
            this->data = nullptr;
        }
        else {
            this->size = old_heap.size;
            for (int ii = 0; ii < size; ii++)
                this->data[ii] = old_heap.data[ii];
        }
    }

    // Delete whatever the this object has in it and perform a deep copy
    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>& Max_bin_heap<T,FwdIt>::operator=(const Max_bin_heap& old_heap) {
        if (this->data != nullptr)
            delete[] this->data;
        if (old_heap.data == nullptr) {
            this->size = 0;
            this->data = nullptr;
        }
        else {
            size = old_heap.size;
            for (int ii = 0; ii < size; ii++)
                this->data[ii] = old_heap.data[ii];
        }
    }

    template<typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>::Max_bin_heap(Max_bin_heap&& old_heap) : size{old_heap.size}, data{old_heap.data} {
        old_heap.size = 0;
        old_heap.data = nullptr;
    }

    template <typename T, typename FwdIt>
    Max_bin_heap<T,FwdIt>& Max_bin_heap<T,FwdIt>::operator=(Max_bin_heap&& old_heap) {
        if (this->data != nullptr)
            delete[] this->data;
        this->size = old_heap.size;
        this->data = old_heap.data;
        old_heap.size = 0;
        old_heap.data = nullptr;
    }

    template <typename T, typename FwdIt>
    std::size_t Max_bin_heap<T,FwdIt>::parent(size_t idx) {
        return idx / 2; // Implicit floor
    }

    template <typename T, typename FwdIt>
    std::size_t Max_bin_heap<T,FwdIt>::left(size_t idx) {
            return (idx % 2 == 1) ? idx / 2 : (idx / 2) - 1;
    }

    template <typename T, typename FwdIt>
    std::size_t Max_bin_heap<T,FwdIt>::right(size_t idx) {
            return (2 * idx) + 2;
    }

    template <typename T, typename FwdIt>
    void Max_bin_heap<T,FwdIt>::max_heapify(T* data, std::size_t idx) {
        std::size_t left_idx = left(idx);
        std::size_t right_idx = right(idx);
        std::size_t largest = 0;
        T tmp;
        
        if (left_idx < this->size && data[left_idx] > data[idx])
            largest = left_idx;
        else
            largest = idx;
        
        if (right_idx < this->size && data[right_idx] > data[largest])
            largest = right_idx;
        
        if (largest != idx) {
            tmp = data[idx];
            data[idx] = data[largest];
            data[largest] = tmp;
            max_heapify(data, largest);
        }
    }

    template <typename T, typename FwdIt>
    T& Max_bin_heap<T, FwdIt>::operator[](size_t idx) {
        return this->data[idx];
    }

    template <typename T, typename FwdIt>
    const T& Max_bin_heap<T, FwdIt>::operator[](size_t idx) const {
        return this->data[idx];
    }

    template <typename T, typename FwdIt>
    std::size_t Max_bin_heap<T, FwdIt>::get_size() const {
        return this->size;
    }

    template <typename T, typename FwdIt>
    void Max_bin_heap<T, FwdIt>::to_sorted_array(T* destination) {
        std::copy(this->data,this->data + this->size, destination);
        for (int ii = this->size / 2; ii >= 0; ii--)
            max_heapify(destination, ii);
    }

    template <typename T, typename FwdIt>
    void Max_bin_heap<T, FwdIt>::insert(T key) {
        this->size += 1;
        increase_key(this->size, key);
    }

    template <typename T, typename FwdIt>
    void Max_bin_heap<T, FwdIt>::increase_key(size_t idx, T key) {
        T tmp;
        this->data[idx] = key;
        while (idx > 0 && this->data[parent(idx)] < this->data[idx]) {
            tmp = this->data[idx];
            this->data[idx] = this->data[parent(idx)];
            this->data[parent(idx)] = tmp;
            idx = parent(idx);
        }
    }

    template <typename T, typename FwdIt>
    T Max_bin_heap<T, FwdIt>::max() const {
        if (this->size == 0)
            throw new std::runtime_error("Size of heap is 0");
        return this->data[0];
    }
    
    template <typename T, typename FwdIt>
    T Max_bin_heap<T, FwdIt>::extract_max() {
        T max;
        if (this->size == 0)
            throw new std::runtime_error("Size of the heap is 0");
            
        max = this->data[0];
        this->data[0] = this->data[this->size - 1];
        this->size--;
        max_heapify(this->data, 1);
        return max;
    }
}



#endif /* heap_h */
