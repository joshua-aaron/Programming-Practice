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
        virtual void heapify(T*, std::size_t)=0;
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
    class Min_bin_heap : public Heap_base<T,A> {
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
        void heapify(T*,std::size_t);
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
        void heapify(T*,std::size_t);
        void increase_key(size_t, T);
    };

    /// TODO: Implement find by key, remove by key, range checked at(size_t index), const copy, and remove by index in the Min & Max heap classes
    /// The Heap_base class is meant to encapsulate the base functionality shared between min & max heaps.
    /// This class will handle the initial storage of data, but note that the heap property will not be maintained.
    /// Handles the allocation and deallocation of data in the "heap," implements access functions and operators,
    /// and houses the data istelf. This makes it so that all that Min & Max heaps really need to do is implement heapify.
    /// These heaps also work as priority queues

    // Make an empty heap
    // Input: none
    template <typename T, typename A>
    Heap_base<T,A>::Heap_base() : size_{0}, data_{nullptr}, space_{0} {}

    // Make a heap of size 1 from a given key
    // Input: A single data input of type T
    template <typename T, typename A>
    Heap_base<T,A>::Heap_base(T key) : size_{1}, data_{new T[1]}, space_{1} {
        data_[0] = key;
    }

    // Make a base heap object from an array
    // Input: A data array of some type T, and its size
    template <typename T, typename A>
    Heap_base<T,A>::Heap_base(T* data, std::size_t size) : size_{size}, data_{new T[size]}, space_{size} {
        for (int ii = 0; ii < size_; ii++)
            data_[ii] = *(data + ii);
    }
    
    // Destructor. Deallocate each element and then deallocate the array
    template <typename T, typename A>
    Heap_base<T,A>::~Heap_base() {
        for (int ii = 0; ii < size_; ii++)
            alloc_.destroy(&data_[ii]);
        alloc_.deallocate(data_, space_);
    }

    // Copy operator (Deep copy)
    template <typename T, typename A>
    Heap_base<T,A>::Heap_base(const Heap_base& old_heap) {
        if (old_heap.data_ == nullptr) {
            space_ = 0;
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

    // Copy assignment (Deep copy)
    // Delete whatever "this" has in it and perform a deep copy
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

    // Move constructor. Simply moving the pointer from the old object to the new
    template<typename T, typename A>
    Heap_base<T,A>::Heap_base(Heap_base&& old_heap) : size_{old_heap.size_}, data_{old_heap.data_} {
        old_heap.size_ = 0;
        old_heap.data_ = nullptr;
    }
    
    // Move assignment. Deallocate old data and reassign the data
    // Input: rvalue reference of a Heap_base
    // Output: Heap_base reference
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

    // Input: Array index
    // Output: The index of the parent of the input
    template <typename T, typename A>
    std::size_t Heap_base<T,A>::parent(size_t idx) {
        return idx / 2; // Implicit floor
    }

    // Input: Array index
    // Output: Index of the left child of the input
    template <typename T, typename A>
    std::size_t Heap_base<T,A>::left(size_t idx) {
            return (idx * 2) + 1;
    }

    // Input: Array index
    // Output: Index of the right child of the input
    template <typename T, typename A>
    std::size_t Heap_base<T,A>::right(size_t idx) {
            return (idx * 2) + 2;
    }

    // Access operator. Not range checked.
    template <typename T, typename A>
    T& Heap_base<T,A>::operator[](size_t idx) {
        return *(data_ + idx);
    }

    // Access operator for const object. Not range checked.
    template <typename T, typename A>
    const T& Heap_base<T,A>::operator[](size_t idx) const {
        return *(data_ + idx);
    }

    // Size getter
    // Output: Size of the array in elements
    template <typename T, typename A>
    std::size_t Heap_base<T,A>::get_size() const {
        return size_;
    }
    
    // Capacity getter
    // Output: The amount of space left in the array
    template <typename T, typename A>
    std::size_t Heap_base<T,A>::capacity() const {
        return space_;
    }
    
    // If the requested allocation is larger than what's been allocated already,
    // Allocate a new object, copy the data from data_, deallocate data_, update space_
    // and then point data_ to that new object.
    // Input: allocation amount
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

    // Input: size of the new array and a value to initialize the new space with
    template <typename T, typename A>
    void Heap_base<T,A>::resize(std::size_t newsize, T val) {
        reserve(newsize);
        for (int ii = size_; ii < newsize; ii++)
            alloc_.construct(&data_[ii], val);
        for (int ii = newsize; ii < size_; ii++)
            alloc_.destroy(&data_[ii]);
        size_ = newsize;
    }

    // Prints out the contents of the array in the order of indices
    // Less useful, but was nice for making sure things obeyed the heap property
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
    // Input: A single key of type T
    template <typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(T key) : Heap_base<T,A>::Heap_base(key) {}

    // Make a max heap from a data array
    // Input: A data array of elements of type T, and its size
    template <typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(T* data, size_t size) :
        Heap_base<T,A>::Heap_base(data, size) {
        for (int ii = this->size_ / 2 - 1; ii >= 0; ii--)
            heapify(this->data_, ii);
    }

    // Destructor. Just utilitize the base class destructor
    template <typename T, typename A>
    Max_bin_heap<T,A>::~Max_bin_heap() {}

    // Copy constructor. Deep copy
    template <typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(const Max_bin_heap& old_heap) :
        Heap_base<T,A>(old_heap) {}

    // Copy assignment. Delete whatever "this" has in it and perform a deep copy
    template <typename T, typename A>
    Max_bin_heap<T,A>& Max_bin_heap<T,A>::operator=(const Max_bin_heap& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    // Move constructor
    template<typename T, typename A>
    Max_bin_heap<T,A>::Max_bin_heap(Max_bin_heap&& old_heap) :
        Heap_base<T,A>::Heap(old_heap) {}

    // Move assignment
    template <typename T, typename A>
    Max_bin_heap<T,A>& Max_bin_heap<T,A>::operator=(Max_bin_heap&& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    // Access operator. Not range checked
    template <typename T, typename A>
    T& Max_bin_heap<T,A>::operator[](std::size_t idx) {
        return Heap_base<T,A>::operator[](idx);
    }

    // Access operator for const object. Not range checked
    template <typename T, typename A>
    const T& Max_bin_heap<T,A>::operator[](std::size_t idx) const {
        return Heap_base<T,A>::operator[](idx);
    }

    // Heapify maintains the max-heap property
    // Input: Pointer to the data, and an index to heapify on
    template <typename T, typename A>
    void Max_bin_heap<T,A>::heapify(T* data, std::size_t idx) {
        std::size_t left_idx = Heap_base<T,A>::left(idx);
        std::size_t right_idx = Heap_base<T,A>::right(idx);
        std::size_t largest = 0;
    
        if (left_idx < this->size_ && data[left_idx] > data[idx])
            largest = left_idx;
        else
            largest = idx;
        
        if (right_idx < this->size_ && data[right_idx] > data[largest])
            largest = right_idx;
        
        if (largest != idx) {
            std::swap(data[idx], data[largest]);
            heapify(data, largest);
        }
    }

    // This is used for implementing heapsort.
    // Input: Destination array to copy the contents of the heap in sorted order into.
    template <typename T, typename A>
    void Max_bin_heap<T,A>::to_sorted_array(T* destination) {
        std::copy(this->data_,this->data_ + this->size_, destination);
        size_t orig_size = this->size_;
        for (int ii = this->size_ - 1; ii > 0; ii--) {
            std::swap(*destination, *(destination + ii));
            this->size_--;
            heapify(destination, 0);
        }
        this->size_ = orig_size;
    }

    // Inserts a key into the heap. If it needs more space, it reserves more
    template <typename T, typename A>
    void Max_bin_heap<T,A>::insert(T key) {
        if (this->space_ == 0)
            Heap_base<T,A>::reserve(8);
        else if (this->size_ == this->space_)
            Heap_base<T,A>::reserve(2 * this->size_);
        this->size_++;
        increase_key(this->size_ - 1, key);
    }

    // This helper function for insertion places the key at the given index
    // and walks up the parents, swapping if it's greater.
    // Input: Index to insert the key at and the key.
    template <typename T, typename A>
    void Max_bin_heap<T,A>::increase_key(size_t idx, T key) {
        this->data_[idx] = key;
        while (idx > 0 && this->data_[Heap_base<T,A>::parent(idx)] < this->data_[idx]) {
            std::swap(this->data_[idx], this->data_[Heap_base<T,A>::parent(idx)]);
            idx = Heap_base<T,A>::parent(idx);
        }
    }

    // Outputs the max value without altering anything
    template <typename T, typename A>
    T Max_bin_heap<T,A>::max() const {
        if (this->size_ == 0)
            throw new std::runtime_error("Size of heap is 0");
        return this->data_[0];
    }
    
    // Outputs and removes the max value (root) from the heap. Copy the last value
    // to the first, and heapify on the new heap of size - 1
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
        heapify(this->data_, 0);
        return max;
    }


    /// Min_binary_heap implementations
    // Input: None
    template <typename T, typename A>
        Min_bin_heap<T,A>::Min_bin_heap() : Heap_base<T,A>() {}

    // Make a heap of size 1 from a given key
    // Input: A single data input of type T
    template <typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(T key) : Heap_base<T,A>::Heap_base(key) {}

    // Make a base heap object from an array
    // Input: A data array of some type T, and its size
    template <typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(T* data, size_t size) :
        Heap_base<T,A>::Heap_base(data, size) {
        for (int ii = this->size_ / 2 - 1; ii >= 0; ii--)
            heapify(this->data_, ii);
    }

    // Destructor. Just utilitize the base class destructor
    template <typename T, typename A>
    Min_bin_heap<T,A>::~Min_bin_heap() {}

    // Copy constructor. Deep copy
    template <typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(const Min_bin_heap& old_heap) :
        Heap_base<T,A>(old_heap) {}

    // Copy assignment. Delete whatever "this" has in it and perform a deep copy
    template <typename T, typename A>
    Min_bin_heap<T,A>& Min_bin_heap<T,A>::operator=(const Min_bin_heap& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    // Move constructor
    template<typename T, typename A>
    Min_bin_heap<T,A>::Min_bin_heap(Min_bin_heap&& old_heap) :
        Heap_base<T,A>::Heap(old_heap) {}

    // Move assignment
    template <typename T, typename A>
    Min_bin_heap<T,A>& Min_bin_heap<T,A>::operator=(Min_bin_heap&& old_heap) {
        Heap_base<T,A>::operator=(old_heap);
        return *this;
    }

    // Access operator. Not range checked
    template <typename T, typename A>
    T& Min_bin_heap<T,A>::operator[](std::size_t idx) {
        return Heap_base<T,A>::operator[](idx);
    }

    // Access operator for const object. Not range checked
    template <typename T, typename A>
    const T& Min_bin_heap<T,A>::operator[](std::size_t idx) const {
        return Heap_base<T,A>::operator[](idx);
    }

    // Heapify maintains the max-heap property
    // Input: Pointer to the data, and an index to heapify on
    template <typename T, typename A>
    void Min_bin_heap<T,A>::heapify(T* data, std::size_t idx) {
        std::size_t left_idx = Heap_base<T,A>::left(idx);
        std::size_t right_idx = Heap_base<T,A>::right(idx);
        std::size_t smallest = 0;
    
        if (left_idx < this->size_ && data[left_idx] < data[idx])
            smallest = left_idx;
        else
            smallest = idx;
        
        if (right_idx < this->size_ && data[right_idx] < data[smallest])
            smallest = right_idx;
        
        if (smallest != idx) {
            std::swap(data[idx], data[smallest]);
            heapify(data, smallest);
        }
    }
    
    // This is used for implementing heapsort.
    // Input: Destination array to copy the contents of the heap in sorted order into.
    template <typename T, typename A>
    void Min_bin_heap<T,A>::to_sorted_array(T* destination) {
        std::copy(this->data_,this->data_ + this->size_, destination);
        size_t orig_size = this->size_;
        for (int ii = this->size_ - 1; ii > 0; ii--) {
            std::swap(*destination, *(destination + ii));
            this->size_--;
            heapify(destination, 0);
        }
        this->size_ = orig_size;
    }

    // Inserts a key into the heap. If it needs more space, it reserves more
    template <typename T, typename A>
    void Min_bin_heap<T,A>::insert(T key) {
        if (this->space_ == 0)
            Heap_base<T,A>::reserve(8);
        else if (this->size_ == this->space_)
            Heap_base<T,A>::reserve(2 * this->size_);
        this->size_++;
        decrease_key(this->size_ - 1, key);
    }

    // This helper function for insertion places the key at the given index
    // and walks up the parents, swapping if it's less.
    // Input: Index to insert the key at and the key.
    template <typename T, typename A>
    void Min_bin_heap<T,A>::decrease_key(size_t idx, T key) {
        this->data_[idx] = key;
        while (idx > 0 && this->data_[Heap_base<T,A>::parent(idx)] > this->data_[idx]) {
            std::swap(this->data_[idx], this->data_[Heap_base<T,A>::parent(idx)]);
            idx = Heap_base<T,A>::parent(idx);
        }
    }

    // Outputs the min value without altering anything
    template <typename T, typename A>
    T Min_bin_heap<T,A>::min() const {
        if (this->size_ == 0)
            throw new std::runtime_error("Size of heap is 0");
        return this->data_[0];
    }
    
    // Outputs and removes the min value (root) from the heap. Copy the last value
    // to the first, and heapify on the new heap of size - 1
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
        heapify(this->data_, 0);
        return min;
    }
}

#endif /* heap_h */
