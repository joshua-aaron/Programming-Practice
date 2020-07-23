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
	    Tree_node(T key, Tree_node<T>* left = nullptr, Tree_node<T>* right = nullptr)
	    	: key{key}, left{left}, right{right} {}
	    Tree_node* left;
	    Tree_node* right;
	    T key;
	}
}
#endif /* node_h */
