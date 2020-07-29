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
	    Tree_node(T key, Tree_node<T>* parent = nullptr, Tree_node<T>* left = nullptr, Tree_node<T>* right = nullptr)
        : key_{key}, parent_{parent}, left_{left}, right_{right} {}
        Tree_node* parent_;
	    Tree_node* left_;
	    Tree_node* right_;
	    T key_;
    };
}
#endif /* node_h */
