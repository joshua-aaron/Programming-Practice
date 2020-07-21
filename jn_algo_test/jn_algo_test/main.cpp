//
//  main.cpp
//  jn_algo_test
//
//  Created by Joshua Navon on 7/14/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//
#include "sort_jn.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <time.h>
#include <typeinfo>

using namespace std;
int main(int argc, const char * argv[]) {
    int arr[10];
    vector<int> v2;
    srand(time(0));
    for (int ii = 0; ii < 10; ii++) {
        arr[ii] = rand() % 10;
        v2.push_back(rand() % 10);
    }
    vector <int> v;
    //srand(time(0));
    for (int ii = 0; ii < 100; ii++ ) {
        v.push_back(rand() % 1000);
    }
    
    for (auto p : v)
        cout << p << " ";
    cout << endl;
    
    insertion_sort_jn_cmp(v.begin(), v.end());
    
    for (auto p : v)
        cout << p <<" ";
    cout << endl << endl << "Merge sort:" << endl;
    for (auto p : arr)
        cout << p << " ";
    cout << endl;
    
    merge_gen(arr, arr+10);
    
    cout << "Sorted:" << endl;
    for (auto p : arr)
        cout << p << " ";
    cout << endl << endl << "Merge sort:" << endl;
    for (auto p : v2)
        cout << p << " ";
    cout << endl;
    
    merge_gen(v2.begin(), v2.end(), std::greater<int>{});
    
    cout << "Sorted:" << endl;
    for (auto p : v2)
        cout << p << " ";
    cout << endl;
    
    return 0;
}
