//
//  main.cpp
//  jn_algo_test
//
//  Created by Joshua Navon on 7/14/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//
#include "gtest/gtest.h"
#include "red_black_tree.hpp"
#include "BST.hpp"
#include "sort_jn.hpp"
#include "binary_heap.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <time.h>
#include <typeinfo>
#include <random>

using namespace std;
int main(int argc, const char * argv[]) {
    int arr[10];
    int arr2[10];
    int arr3[10] = {1,5,2,9,9,4,7,0,6,3};
    vector<int> v2;
    vector<double> v3;
    srand(time(0));
    std::uniform_real_distribution<double> dist(0, 10);
    std::default_random_engine engine;
    
    for (int ii = 0; ii < 10; ii++) {
        arr[ii] = rand() % 10;
        arr2[ii] = rand() % 10;
        v2.push_back(rand() % 10);
        v3.push_back(dist(engine));
    }
    vector<double> v4(v3);
    vector<int> v5(v2);
    vector <int> v;
    //srand(time(0));
    for (int ii = 0; ii < 100; ii++ ) {
        v.push_back(rand() % 1000);
    }
    clrs_impl_jn::Max_bin_heap<double> myheap(&v3.at(0), v3.size());
    
    for (auto p : v)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::insertion_sort_jn_cmp(v.begin(), v.end());
    
    for (auto p : v)
        cout << p <<" ";
    cout << endl << endl << "Merge sort:" << endl;
    for (auto p : arr)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::merge_gen(arr, arr+10);
    
    cout << "Sorted:" << endl;
    for (auto p : arr)
        cout << p << " ";
    cout << endl << endl << "Merge sort:" << endl;
    for (auto p : v2)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::merge_gen(v2.begin(), v2.end(), std::greater<int>{});
    cout << "Sorted:" << endl;
    for (auto p : v2)
        cout << p << " ";
    cout << endl;
    
    cout << endl << "Heap sort:" << endl;
    for (auto p : v3)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::heapsort<double>(&v3.at(0), v3.size());
    
    cout << "Sorted:" << endl;
    for (auto p : v3)
        cout << p << " ";
    cout << endl;
    
    cout << endl << "Heap sort:" << endl;
    for (auto p : arr2)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::heapsort<int>(arr2, 10);
    
    cout << "Sorted:" << endl;
    for (auto p : arr2)
        cout << p << " ";
    cout << endl;
    
    cout << "Max is: " << myheap.max() << endl;
    size_t tmp_size = myheap.get_size();
    for (int ii = 0; ii < tmp_size; ii++)
        cout << "Max extracted: " << myheap.extract_max() << endl << "New size is: " << myheap.get_size() << endl << endl;
    for (int ii = 0; ii < 25; ii++)
        myheap.insert(dist(engine));
    myheap.print_data();
    clrs_impl_jn::Max_bin_heap<double> heap2(myheap); // Copy constructor
    heap2.print_data();
    clrs_impl_jn::Max_bin_heap<double> heap3(1.0);
    heap3 = myheap;
    heap3.print_data();
    
    cout << "Quicksort:" << endl;
    for (auto p : arr3)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::randomized_quicksort(arr3, arr3 + 10);
    
    cout << "Sorted:" << endl;
    for (auto p : arr3)
        cout << p << " ";
    cout << endl;
    
    cout << "Quicksort:" << endl;
    for (auto p : v4)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::randomized_quicksort(v4.begin(), v4.end());
    
    cout << "Sorted:" << endl;
    for (auto p : v4)
        cout << p << " ";
    cout << endl;
    
    cout << "Quicksort:" << endl;
    for (auto p : v5)
        cout << p << " ";
    cout << endl;
    
    clrs_impl_jn::randomized_quicksort(v5.begin(), v5.end());
    
    cout << "Sorted:" << endl;
    for (auto p : v5)
        cout << p << " ";
    cout << endl << endl;
    
    clrs_impl_jn::BST<int> bst = clrs_impl_jn::BST<int>();
    for (int ii = 0; ii < 10; ii++) {
        int num = dist(engine);
        cout << num << " ";
        bst.insert(num);
    }
    cout << endl;
    bst.inorder_walk();
    return 0;
}
