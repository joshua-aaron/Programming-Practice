//
//  binary_heap.h
//  jn_algo_test
//
//  Created by Joshua Navon on 7/22/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#ifndef heap_hpp
#define heap_hpp

#include "node.hpp"

namespace clrs_impl_jn {
    template <typename T>
    class Min_bin_heap {
    public:
        Min_bin_heap();
        ~Min_bin_heap();
        Min_bin_heap(const Min_bin_heap& heap);
        Min_bin_heap& operator=(const Min_bin_heap&);
        Min_bin_heap(Min_bin_heap&& heap);
        Min_bin_heap& operator=(Min_bin_heap&&);
        bool operator==(Min_bin_heap&);
        bool operator!=(Min_bin_heap&);
        void insert(T);
        void find(T);
    private:
        
    }

    template <typename T>
    class Max_bin_heap {
    public:
        Max_bin_heap();
        ~Max_bin_heap();
        Max_bin_heap(const Max_bin_heap&);
        Max_bin_heap& operator=(const Max_bin_heap&);
        Max_bin_heap(Max_bin_heap&&);
        Max_bin_heap& operator=(Max_bin_heap&&);
        bool operator==(Max_bin_heap&);
        bool operator!=(Max_bin_heap&);
        void insert(T);
        void find(T);
        
    private:
    }
}

#endif /* heap_h */
