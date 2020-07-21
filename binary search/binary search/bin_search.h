//
//  bin_search.h
//  binary search
//
//  Created by Joshua Navon on 7/14/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#ifndef bin_search_h
#define bin_search_h
template <typename Iterator, typename T>
bool bin_search(Iterator first, Iterator last, const T val) {
    if (first == last) {
        if (*first == val)
            return true;
        else
            return false;
    }
    
    auto size = last-first+1;
    Iterator mid = first + (size/2) - 1;
    if (*mid == val)
        return true;
    else if (*mid < val) {
        mid++;
        return bin_search(mid, last, val);
    }
    else {
        mid--;
        return bin_search(first, mid, val);
    }
}
#endif /* bin_search_h */
