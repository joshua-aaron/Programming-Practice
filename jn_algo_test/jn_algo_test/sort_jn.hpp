/*
  sort_jn.hpp
  Created by Joshua Navon on 7/14/20.
*/

#ifndef SORT_JN_HPP_
#define SORT_JN_HPP_
#include <iostream>
#include "binary_heap.hpp"

namespace clrs_impl_jn {
    // This insertion sort implementation takes 2 iterators and a comparator object.
    // Range of the iterators is [first,last)
    template <typename Iterator, typename Compare = std::less<>>
    void insertion_sort_jn_cmp(Iterator first, Iterator last, Compare cmp = Compare()) {
        for (Iterator it = first; it != last; ++it) {
            std::rotate(std::upper_bound(first, it, *it, cmp), it, std::next(it));
        }
    }


    // Helper function for the generic merge sort that takes a comparator like std::less<>.
    // It handles the moving around of the original container elements.
    //
    // The iterators are in the range of [first,last]
    // Note how the variable last in this helper function is the last element
    // instead of a pointer to right after the last element.
    template <typename Iterator, typename Compare>
    void merge_gen_helper(Iterator first, Iterator mid, Iterator last, Compare cmp) {
        // Flags for whether we've reached the end of each sub-array
        bool sub2_flag = false;
        bool sub1_flag = false;
        
        // Index variables for each sub-array
        int sub1_idx = 0;
        int sub2_idx = 0;
        
        // Sub-arrays to split the container into.
        // sub1 gets [first,mid] and sub2 gets [mid_right,last]
        Iterator mid_right = std::next(mid);
        typename std::iterator_traits<Iterator>::value_type* sub1 = new typename std::iterator_traits<Iterator>::value_type[std::distance(first, mid) + 1];
        typename std::iterator_traits<Iterator>::value_type* sub2 = new typename std::iterator_traits<Iterator>::value_type[std::distance(mid_right,last) + 1];
        
        // Fill the sub-arrays
        for (int ii = 0; ii <= std::distance(first, mid);ii++) {
            sub1[ii] = *(std::next(first,ii));
        }
        for (int ii = 0; ii <= std::distance(mid_right, last); ii++)
            sub2[ii] = *(std::next(mid_right,ii));
        
        // Going through each sub-array linearly, input the sorted values into
        // the original container. Added simple logic to skip a comparison
        // if we've reached the end of either sub-array
        for (Iterator it = first; it != std::next(last); it++){
            //if (!sub1_flag && (sub2_flag || (!sub2_flag && *(std::next(sub1,sub1_idx)) <= *(std::next(sub2, sub2_idx))))) {
            if (!sub1_flag && (sub2_flag || (!sub2_flag && cmp(*(std::next(sub1,sub1_idx)),*(std::next(sub2, sub2_idx)))))) {
                *it = *(std::next(sub1,sub1_idx++));
                if (sub1_idx > std::distance(first, mid))
                    sub1_flag = true;
            }
            else if (!sub2_flag) {
                *it = *(std::next(sub2,sub2_idx++));
                if (sub2_idx > std::distance(mid, last) - 1)
                    sub2_flag = true;
            }
        }

        // Free up memory
        delete[] sub1;
        delete[] sub2;
    }
     
    // Generic merge sort that takes a Forward Iterator and a comparator like std::less<>.
    // Range of the iterators is [first,last).
    template <typename Iterator, typename Compare  = std::less<>>
    void merge_gen(Iterator first, Iterator last, Compare cmp = Compare()) {
        auto const size = std::distance(first, last);
        
        // Base case
        if (size < 2)
            return;
        
        // If size is even, then both sub-arrays will be of same size
        // If size is odd, then the [first,mid] will have one less element than (mid,last)
        auto mid = std::next(first, (size/2) - 1);
        merge_gen(first, std::next(mid), cmp);
        merge_gen(std::next(mid), last, cmp);
        merge_gen_helper(first, mid, std::next(first,size - 1), cmp);
    }

    // Implementation of generic Heapsort
    template <typename T, typename FwdIt>
    void heapsort(FwdIt first, FwdIt last) {
        T tmp;
        Max_bin_heap<T, FwdIt> max_heap(first, last);
        
        for (int ii = max_heap.get_size() - 1; ii > 0; ii--) {
            
        }
    }
}
#endif /* SORT_JN_HPP_ */
