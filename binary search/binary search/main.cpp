//
//  main.cpp
//  binary search
//
//  Created by Joshua Navon on 7/13/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include "bin_search.h"
using namespace std;

template <typename T>
void print_results(bool result, T val, string name) {
    cout << "The value " << to_string(val) << " " << ((result) ? "is " : "is not ") << "in "<<name<<endl;
}
 
int main(int argc, const char * argv[]) {
    vector<int> v{0,1,2,3,4,5,6,7,8,9};
    int arr1[10] = {0,1,2,3,4,5,6,7,8,9};
    int n = 10;
    int arr2[] = {0,1,2,3,4,5,6,7,8,9};
    int val = 0;
    
    while (val < 11) {
        print_results(bin_search(&arr1[0], &arr1[0]+sizeof(arr1)/sizeof(int)-1, val), val,"arr1");
        print_results(bin_search(&arr2[0], &arr2[0]+sizeof(arr2)/sizeof(int)-1, val), val,"arr2");
        print_results(bin_search(v.begin(), v.end(), val), val,"v");
        cout<<endl;
        val++;
    }
    
    return 0;
}
