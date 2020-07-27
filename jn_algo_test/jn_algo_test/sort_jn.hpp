/*
  sort_jn.hpp
  Created by Joshua Navon on 7/14/20.
*/

#ifndef SORT_JN_HPP_
#define SORT_JN_HPP_
#include <iostream>
#include <random>
#include <algorithm>
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
    // Builds a max-heap and then copies the sorted data into the input container.
    // Input: Pointer to an array, and size of the array
    // Output: None...Sorts in place.
    template <typename T>
    void heapsort(T* data, std::size_t size) {
        Max_bin_heap<T> heap(data, size);
        heap.to_sorted_array(data);
    }


    // Partition function for Quicksort
    // Input: 2  Forward Iterators in range [first,last) and a comparator
    // Output: Forward Iterator pointing to the partition index
    template <typename FwdIt, typename Compare>
    FwdIt partition_helper(FwdIt first, FwdIt last, Compare cmp) {
        typename std::iterator_traits<FwdIt>::value_type partition_val =
            *(std::next(first, std::distance(first, last) - 1));
        
        FwdIt sub1_end; // Keeps track of the cmp()==true sub-array
        bool sub1_end_set_flag = false; // Keeps track if we've set sub1_end or not
        size_t size = std::distance(first, last);
        
        for (FwdIt iter = first; iter != std::next(first, size - 1); iter++) {
            if (cmp(*iter, partition_val)) {
                // If the compare is true and sub1_end isn't initialized,
                // set it to first, and toggle the flag
                if (!sub1_end_set_flag) {
                    sub1_end_set_flag = true;
                    sub1_end = first;
                }
                else
                    sub1_end++;
                std::iter_swap(sub1_end, iter);
            }
        }
        
        // Since sub1_end isn't initialized unless we find something that satisfies
        // the comparator, we swap first & last-1, and then return first.
        if (!sub1_end_set_flag) {
            std::iter_swap(first, std::next(first, size - 1));
            return first;
        }
        else {
            std::iter_swap(std::next(sub1_end), std::next(first, size - 1));
            return std::next(sub1_end);
        }
    }

    // Helper function for quicksort that picks a random partition index.
    // Partition index is picked at random using a standard uniform distribution
    // Input: 2  Forward Iterators in range [first,last) and a comparator
    // Output: Forward Iterator pointing to the partition index
    template <typename FwdIt, typename Compare>
    FwdIt randomized_partition(FwdIt first, FwdIt last, Compare cmp) {
        size_t size = std::distance(first, last);
        std::random_device rdev;
        std::seed_seq seq {rdev()};
        std::default_random_engine engine {seq};
        std::uniform_int_distribution<int> dist(0, size - 1); // Range is [0,size)
        int partition_idx = dist(engine);
        
        // Exchange Array[partition_idx] & Array[last - 1]
        std::iter_swap(std::next(first, partition_idx), std::next(first, size - 1));
        return partition_helper(first, last, cmp);
    }
    
    // Implementation of generic quicksort using a randomized partition
    // If the container is at least 2 elements, get the partition index.
    // Then, call quicksort on the sub-arrays separated by the index
    // Input: 2 Forward iterators in range [first,last) and a comparator
    // Output: None...Sorts container in place
    template <typename FwdIt, typename Compare  = std::less_equal<>>
    void randomized_quicksort(FwdIt first, FwdIt last, Compare cmp = Compare()) {
        if (std::distance(first, last) > 1) {
            FwdIt idx = randomized_partition(first, last, cmp);
            randomized_quicksort(first, idx, cmp);
            randomized_quicksort(std::next(idx), last, cmp);
        }
    }
}
#endif /* SORT_JN_HPP_ */
