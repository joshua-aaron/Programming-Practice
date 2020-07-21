/*
  sort_jn.hpp
  Created by Joshua Navon on 7/14/20.
*/

#ifndef SORT_JN_HPP_
#define SORT_JN_HPP_
#include <iostream>

/* This insertion sort implementation takes 2 iterators and sorts in place/by reference
    begin points to the first element
    end points to the past-the-last element
 */
template <typename Iterator>
void insertion_sort_jn(Iterator begin, Iterator end) {
    for (Iterator it = begin; it != end; ++it) {
        std::rotate(std::upper_bound(begin, it, *it), it, std::next(it));
    }
}

template <typename Iterator, typename Compare>
void insertion_sort_jn_cmp(Iterator begin, Iterator end, Compare cmp) {
    for (Iterator it = begin; it != end; ++it) {
        std::rotate(std::upper_bound(begin, it, *it, cmp), it, std::next(it));
    }
}

void merge_int_helper(int* arr, int start_index, int mid_index, int end_index) {
    bool sub2_flag = false;
    bool sub1_flag = false;
    int sub1_idx = 0;
    int sub2_idx = 0;
    int* sub1 = new int[mid_index - start_index + 1];
    int* sub2 = new int[end_index - mid_index + 1];
    
    /* Copy the contents of arr into the sub-arrays */
    memcpy(sub1, arr + start_index, (mid_index - start_index + 1)*sizeof(int));
    memcpy(sub2, arr + mid_index + 1, (end_index - mid_index) * sizeof(int));
    
    for (int ii = start_index; ii <= end_index; ii++) {
        if (!sub1_flag && (sub2_flag || (!sub2_flag && sub1[sub1_idx] <= sub2[sub2_idx]))) {
            arr[ii] = sub1[sub1_idx++];
            if (sub1_idx > mid_index - start_index)
                sub1_flag = true;
        }
        else if (!sub2_flag) {
            arr[ii] = sub2[sub2_idx++];
            if (sub2_idx > (end_index - mid_index - 1))
                sub2_flag = true;
        }
    }
    
    delete[] sub1;
    delete[] sub2;
}

void merge_int(int* arr, int start, int end) {
    // Catch the empty array case and just return
    if (arr == nullptr)
        return;
    
    // All other cases
    if (start < end) {
        int mid = (start + end) / 2; // Essentially a floor since it'll truncate
        merge_int(arr, start, mid);
        merge_int(arr,mid + 1, end);
        merge_int_helper(arr, start, mid, end);
    }
}

template <typename Iterator>
void merge_gen_helper(Iterator first, Iterator mid, Iterator last) {
    bool sub2_flag = false;
    bool sub1_flag = false;
    int sub1_idx = 0;
    int sub2_idx = 0;
    typename std::iterator_traits<Iterator>::value_type* sub1 = new typename std::iterator_traits<Iterator>::value_type[std::distance(first, mid) + 1];
    typename std::iterator_traits<Iterator>::value_type* sub2 = new typename std::iterator_traits<Iterator>::value_type[std::distance(std::next(mid),last) + 1];
    Iterator mid_right = std::next(mid);
    for (int ii = 0; ii <= std::distance(first, mid);ii++) {
        sub1[ii] = *(std::next(first,ii));
    }
    for (int ii = 0; ii <= std::distance(mid_right, last); ii++)
        sub2[ii] = *(std::next(mid_right,ii));
    
    /* helper output
    Iterator tmp;
    std::cout << "starting array is: ";
    for (int ii = 0;;ii++) {
        tmp = std::next(first, ii);
        if (tmp == std::next(last))
            break;
        std::cout << *tmp << " ";
    }
    std::cout << std::endl;
    
    std::cout << "sub-array 1 is: ";
    for (int ii = 0;;ii++) {
        tmp = std::next(first,ii);
        if (tmp == std::next(mid))
            break;
        std::cout << *tmp << " ";
    }
    std::cout << std::endl;
    
    std::cout << "sub-array 2 is: ";
    for (int ii = 0;;ii++) {
        tmp = std::next(mid_right,ii);
        if (tmp == std::next(last))
            break;
        std::cout << *tmp << " ";
    }
    std::cout << std::endl;*/

    
    for (Iterator it = first; it != std::next(last); it++){
        if (!sub1_flag && (sub2_flag || (!sub2_flag && *(std::next(sub1,sub1_idx)) <= *(std::next(sub2, sub2_idx))))) {
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
    
    /*std::cout << "Sorted sub-array is: ";
    for (int ii = 0;;ii++) {
        tmp = std::next(first, ii);
        if (tmp == std::next(last))
            break;
        std::cout << *tmp << " ";
    }
    std::cout << std::endl;*/
    delete[] sub1;
    delete[] sub2;
}

template <typename Iterator>
void merge_gen(Iterator first, Iterator last) {
    auto const size = std::distance(first, last);
    if (size < 1)
        return;
    
    auto mid = std::next(first, size / 2);
    merge_gen(first, mid);
    merge_gen(std::next(mid), last);
    merge_gen_helper(first, mid, last);
}

#endif /* SORT_JN_HPP_ */
