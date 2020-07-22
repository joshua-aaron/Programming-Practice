//
//  node.hpp
//  jn_algo_test
//
//  Created by Joshua Navon on 7/22/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#ifndef node_hpp
#define node_hpp
namespace clrs_impl_jn {

template <typename T>
struct Tree_node {
    Tree_node();
    Tree_node(T);
    ~Tree_node();
    Tree_node(const Tree_node&);
    Tree_node& operator=(const Tree_node&);
    Tree_node* left;
    Tree_node* right;
    T key;
}

}
#endif /* node_h */
