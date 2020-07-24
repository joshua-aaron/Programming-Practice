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

    template <typename T, typename A = std::allocator<T>>
    class Heap_base {
    public:
        virtual ~Heap_base();
        std::size_t get_size() const;
        std::size_t capacity() const;
        void print_data() const;
    protected:
        Heap_base();
        Heap_base(T);
        Heap_base(T*, std::size_t);
        Heap_base(const Heap_base&);
        Heap_base& operator=(const Heap_base&);
        Heap_base(Heap_base&&);
        Heap_base& operator=(Heap_base&&);
        T& operator[](size_t);
        const T& operator[](size_t) const;
        void reserve(std::size_t);
        void resize(std::size_t, T val = T());
        std::size_t parent(size_t);
        std::size_t left(size_t);
        std::size_t right(size_t);
        std::size_t size_;
        T* data_;
        std::size_t space_;
        A alloc_;
    };

    template <typename T, typename A = std::allocator<T>>
    class Min_bin_heap {
    public:
        Min_bin_heap();
        explicit Min_bin_heap(T);
        Min_bin_heap(T*,size_t);
        ~Min_bin_heap();
        Min_bin_heap(const Min_bin_heap&);
        Min_bin_heap& operator=(const Min_bin_heap&);
        Min_bin_heap(Min_bin_heap&&);
        Min_bin_heap& operator=(Min_bin_heap&&);
        T& operator[](size_t);
        const T& operator[](size_t) const;
        void insert(T);
        T min() const;
        T extract_min();
        void to_sorted_array(T*);
    private:
        void min_heapify(T*,std::size_t);
        void decrease_key(size_t, T);
    };

    template <typename T, typename A = std::allocator<T>>
    class Max_bin_heap : public Heap_base<T,A> {
    public:
        Max_bin_heap();
        explicit Max_bin_heap(T);
        Max_bin_heap(T*,size_t);
        ~Max_bin_heap();
        Max_bin_heap(const Max_bin_heap&);
        Max_bin_heap& operator=(const Max_bin_heap&);
        Max_bin_heap(Max_bin_heap&&);
        Max_bin_heap& operator=(Max_bin_heap&&);
        T& operator[](size_t);
        const T& operator[](size_t) const;
        void insert(T);
        bool find(T) const;
        T max() const;
        T extract_max();
        void to_sorted_array(T*);
    private:
        void max_heapify(T*,std::size_t);
        void increase_key(size_t, T);
    };

    
    /// Heap_base class implementation
    template <typename T, typename A>
    Heap_base<T,A>::Heap_base() : size_{0}, data_{nullptr}, space_{0} {}

    template <typename T, typename A>
    Heap_base<T,A>::Heap_base(T key) : size_{1}, data_{new T[1]}, space_{1} {
        data_[0] = key;
    }

    template <typename T, typename A>
    Heap_base<T,A>::Heap_base(T* data, std::size_t size) : size_{size}, data_{new T[size]}, space_{size} {
        for (int ii = 0; ii < size_; ii++)
            data_[ii] = *(data + ii);
    }

    template <typename T, typename A>
    Heap_base<T,A>::~Heap_base() {
        for (int ii = 0; ii < size_; ii++)
            alloc_.destroy(&data_[ii]);
        alloc_.deallocate(data_, space_);
    }

    // Deep copy
    template <typename T, typename A>
    Heap_base<T,A>::Heap_base(const Heap_base& old_heap) {
        if (old_heap.data_ == nullptr) {
            size_ = 0;
            data_ = nullptr;
        }
        else {
            size_ = old_heap.size_;
            space_ = old_heap.space_;
            data_ = alloc_.allocate(space_);
            for (int ii = 0; ii < size_; ii++)
                alloc_.construct(&data_[ii], old_heap.data_[ii]);
        }
    }

    // Delete whatever the this object has in it and perform a deep copy
    template <typename T, typename A>
    Heap_base<T,A>& Heap_base<T,A>::operator=(const Heap_base& old_heap) {
        if (data_ != nullptr) {
            for (int ii = 0; ii < size_; ii++)
                alloc_.destroy(&data_[ii]);
            alloc_.deallocate(data_, space_);
        }
        if (old_heap.data_ == nullptr) {
            size_ = 0;
            space_ = 0;
            data_ = nullptr;
        }
        else {
            size_ = old_heap.size_;
            space_ = old_heap.space_;
            data_ = alloc_.allocate(space_);
            for (int ii = 0; ii < size_; ii++)
                alloc_.construct(&data_[ii], old_heap.data_[ii]);
        }
        return *this;
    }

    template<typename T, typename A>
    Heap_base<T,A>::Heap_base(Heap_base&& old_heap) : size_{old_heap.size_}, data_{old_heap.data_} {
        old_heap.size_ = 0;
        old_heap.data_ = nullptr;
    }

    template <typename T, typename A>
    Heap_base<T,A>& Heap_base<T,A>::operator=(Heap_base&& old_heap) {
        if (data_ != nullptr) {
            for (int ii = 0; ii < size_; ii++)
                alloc_.destroy(&data_[ii]);
            alloc_.deallocate(data_, space_);
        }
        size_ = old_heap.size_;
        data_ = old_heap.data_;
        old_heap.size_ = 0;
        old_heap.data_ = nullptr;
        return *this;
    }

    template <typename T, typename A>
    std::size_t Heap_base<T,A>::parent(size_t idx) {
        return idx / 2; // Implicit floor
    }

    template <typename T, typename A>
    std::size_t Heap_base<T,A>::left(size_t idx) {
            return (idx * 2) + 1;
    }

    template <typename T, typename A>
    std::size_t Heap_base<T,A>::right(size_t idx) {
            return (idx * 2) + 2;
    }

    template <typename T, typename A>
        T& Heap_base<T,A>::operator[](size_t idx) {
            return *(data_ + idx);
        }

        template <typename T, typename A>
        const T& Heap_base<T,A>::operator[](size_t idx) const {
            return *(data_ + idx);
        }

        template <typename T, typename A>
        std::size_t Heap_base<T,A>::get_size() const {
            return size_;
        }
        
        template <typename T, typename A>
        std::size_t Heap_base<T,A>::capacity() const {
            return space_;
        }
        
        template <typename T, typename A>
        void Heap_base<T,A>::reserve(std::size_t newalloc) {
            if (newalloc <= space_)
                return;
            T* p =  alloc_.allocate(newalloc);
            for (int ii = 0; ii < size_; ii++)
                alloc_.construct(&p[ii], data_[ii]);
            for (int ii = 0; ii < size_; ii++)
                alloc_.destroy(&data_[ii]);
            alloc_.deallocate(data_, space_);
            data_ = p;
            space_ = newalloc;
        }

        template <typename T, typename A>
    void Heap_base<T,A>::resize(std::size_t newsize, T val) {
            reserve(newsize);
            for (int ii = size_; ii < newsize; ii++)
                alloc_.construct(&data_[ii], val);
            for (int ii = newsize; ii < size_; ii++)
                alloc_.destroy(&data_[ii]);
            size_ = newsize;
        }

    template <typename T, typename A>
    void Heap_base<T,A>::print_data() const {
        std::cout << "this->data_ is: " << std::endl;
        for (int ii = 0; ii < size_; ii++)
            std::cout << data_[ii] << " ";
        std::cout << std::endl << std::endl;
    }

    /// Max_bin_heap implementation
    // Make an empty heap;
    template <typename T, typename A>
        Max_bin_heap<T,A>::Max_bin_heap() : Heap_base<T,A>() {}

    // Initialize the heap with a singe key
    template <typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(T key) : Heap_base<T,A>::Heap_base(key) {}

    template <typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(T* data, size_t size) :
        Heap_base<T,A>::Heap_base(data, size) {
        for (int ii = this->size_ / 2 - 1; ii >= 0; ii--)
            max_heapify(this->data_, ii);
    }

    // Just utilitize the base class destructor
    template <typename T, typename A>
    Max_bin_heap<T,A>::~Max_bin_heap() {}

    // Deep copy
    template <typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(const Max_bin_heap& old_heap) :
        Heap_base<T,A>(old_heap) {}

    // Delete whatever the this object has in it and perform a deep copy
    template <typename T, typename A>
    Max_bin_heap<T,A>& Max_bin_heap<T,A>::operator=(const Max_bin_heap& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    template<typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(Max_bin_heap&& old_heap) :
        Heap_base<T,A>::Heap(old_heap) {}

    template <typename T, typename A>
    Max_bin_heap<T,A>& Max_bin_heap<T,A>::operator=(Max_bin_heap&& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    template <typename T, typename A>
    T& Max_bin_heap<T,A>::operator[](std::size_t idx) {
        return Heap_base<T,A>::operator[](idx);
    }

    template <typename T, typename A>
    const T& Max_bin_heap<T,A>::operator[](std::size_t idx) const {
        return Heap_base<T,A>::operator[](idx);
    }

    template <typename T, typename A>
    void Max_bin_heap<T,A>::max_heapify(T* data, std::size_t idx) {
        std::size_t left_idx = Heap_base<T,A>::left(idx);
        std::size_t right_idx = Heap_base<T,A>::right(idx);
        std::size_t largest = 0;
        T tmp;
    
        if (left_idx < this->size_ && data[left_idx] > data[idx])
            largest = left_idx;
        else
            largest = idx;
        
        if (right_idx < this->size_ && data[right_idx] > data[largest])
            largest = right_idx;
        
        if (largest != idx) {
            tmp = data[idx];
            data[idx] = data[largest];
            data[largest] = tmp;
            max_heapify(data, largest);
        }
    }

    template <typename T, typename A>
    void Max_bin_heap<T,A>::to_sorted_array(T* destination) {
        std::copy(this->data_,this->data_ + this->size_, destination);
        T tmp;
        size_t orig_size = this->size_;
        for (int ii = this->size_ - 1; ii > 0; ii--) {
            tmp = *(destination);
            *(destination) = *(destination + ii);
            *(destination + ii) = tmp;
            this->size_--;
            max_heapify(destination, 0);
        }
        this->size_ = orig_size;
    }

    template <typename T, typename A>
    void Max_bin_heap<T,A>::insert(T key) {
        if (this->space_ == 0)
            Heap_base<T,A>::reserve(8);
        else if (this->size_ == this->space_)
            Heap_base<T,A>::reserve(2 * this->size_);
        this->size_++;
        increase_key(this->size_ - 1, key);
    }

    template <typename T, typename A>
    void Max_bin_heap<T,A>::increase_key(size_t idx, T key) {
        T tmp;
        this->data_[idx] = key;
        while (idx > 0 && this->data_[Heap_base<T,A>::parent(idx)] < this->data_[idx]) {
            tmp = this->data_[idx];
            this->data_[idx] = this->data_[Heap_base<T,A>::parent(idx)];
            this->data_[Heap_base<T,A>::parent(idx)] = tmp;
            idx = Heap_base<T,A>::parent(idx);
        }
    }

    template <typename T, typename A>
    T Max_bin_heap<T,A>::max() const {
        if (this->size_ == 0)
            throw new std::runtime_error("Size of heap is 0");
        return this->data_[0];
    }
    
    template <typename T, typename A>
    T Max_bin_heap<T,A>::extract_max() {
        T max;
        
        // Can't extract anything from a size 0 heap
        // Throw an exception and let the caller deal with it
        if (this->size_ == 0)
            throw new std::runtime_error("Size of the heap is 0");
            
        max = this->data_[0];
        // Copy the last val to the max position, resize the array and heapify
        this->data_[0] = this->data_[this->size_ - 1];
        Heap_base<T,A>::resize(this->size_ - 1);
        max_heapify(this->data_, 0);
        return max;
    }


    /// Min_binary_heap implementations
    template <typename T, typename A>
        Min_bin_heap<T,A>::Min_bin_heap() : Heap_base<T,A>() {}

    // Initialize the heap with a singe key
    template <typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(T key) : Heap_base<T,A>::Heap_base(key) {}

    template <typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(T* data, size_t size) :
        Heap_base<T,A>::Heap_base(data, size) {
        for (int ii = this->size_ / 2 - 1; ii >= 0; ii--)
            min_heapify(this->data_, ii);
    }

    // Just utilitize the base class destructor
    template <typename T, typename A>
    Min_bin_heap<T,A>::~Min_bin_heap() {}

    // Deep copy
    template <typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(const Min_bin_heap& old_heap) :
        Heap_base<T,A>(old_heap) {}

    // Delete whatever the this object has in it and perform a deep copy
    template <typename T, typename A>
    Min_bin_heap<T,A>& Min_bin_heap<T,A>::operator=(const Min_bin_heap& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    template<typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(Min_bin_heap&& old_heap) :
        Heap_base<T,A>::Heap(old_heap) {}

    template <typename T, typename A>
    Min_bin_heap<T,A>& Min_bin_heap<T,A>::operator=(Min_bin_heap&& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    template <typename T, typename A>
    T& Min_bin_heap<T,A>::operator[](std::size_t idx) {
        return Heap_base<T,A>::operator[](idx);
    }

    template <typename T, typename A>
    const T& Min_bin_heap<T,A>::operator[](std::size_t idx) const {
        return Heap_base<T,A>::operator[](idx);
    }

    template <typename T, typename A>
    void Min_bin_heap<T,A>::min_heapify(T* data, std::size_t idx) {
        std::size_t left_idx = Heap_base<T,A>::left(idx);
        std::size_t right_idx = Heap_base<T,A>::right(idx);
        std::size_t smallest = 0;
        T tmp;
    
        if (left_idx < this->size_ && data[left_idx] < data[idx])
            smallest = left_idx;
        else
            smallest = idx;
        
        if (right_idx < this->size_ && data[right_idx] < data[smallest])
            smallest = right_idx;
        
        if (smallest != idx) {
            tmp = data[idx];
            data[idx] = data[smallest];
            data[smallest] = tmp;
            min_heapify(data, smallest);
        }
    }

    template <typename T, typename A>
    void Min_bin_heap<T,A>::to_sorted_array(T* destination) {
        std::copy(this->data_,this->data_ + this->size_, destination);
        T tmp;
        size_t orig_size = this->size_;
        for (int ii = this->size_ - 1; ii > 0; ii--) {
            tmp = *(destination);
            *(destination) = *(destination + ii);
            *(destination + ii) = tmp;
            this->size_--;
            max_heapify(destination, 0);
        }
        this->size_ = orig_size;
    }

    template <typename T, typename A>
    void Min_bin_heap<T,A>::insert(T key) {
        if (this->space_ == 0)
            Heap_base<T,A>::reserve(8);
        else if (this->size_ == this->space_)
            Heap_base<T,A>::reserve(2 * this->size_);
        this->size_++;
        decrease_key(this->size_ - 1, key);
    }

    template <typename T, typename A>
    void Min_bin_heap<T,A>::decrease_key(size_t idx, T key) {
        T tmp;
        this->data_[idx] = key;
        while (idx > 0 && this->data_[Heap_base<T,A>::parent(idx)] > this->data_[idx]) {
            tmp = this->data_[idx];
            this->data_[idx] = this->data_[Heap_base<T,A>::parent(idx)];
            this->data_[Heap_base<T,A>::parent(idx)] = tmp;
            idx = Heap_base<T,A>::parent(idx);
        }
    }

    template <typename T, typename A>
    T Min_bin_heap<T,A>::min() const {
        if (this->size_ == 0)
            throw new std::runtime_error("Size of heap is 0");
        return this->data_[0];
    }
    
    template <typename T, typename A>
    T Min_bin_heap<T,A>::extract_min() {
        T min;
        
        // Can't extract anything from a size 0 heap
        // Throw an exception and let the caller deal with it
        if (this->size_ == 0)
            throw new std::runtime_error("Size of the heap is 0");
            
        min = this->data_[0];
        // Copy the last val to the max position, resize the array and heapify
        this->data_[0] = this->data_[this->size_ - 1];
        Heap_base<T,A>::resize(this->size_ - 1);
        min_heapify(this->data_, 0);
        return min;
    }
}

#endif /* heap_h */
