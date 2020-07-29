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
	struct Tree_Node {
	    Tree_Node(T key, Tree_Node<T>* parent = nullptr, Tree_Node<T>* left = nullptr, Tree_Node<T>* right = nullptr)
        : key_{key}, parent_{parent}, left_{left}, right_{right} {}
        Tree_Node* parent_;
	    Tree_Node* left_;
	    Tree_Node* right_;
	    T key_;
    };
    
    template <typename T>
    struct RB_Node : public Tree_Node<T> {
        enum color_ {black_, red_};
        color_ color__;
        RB_Node(T key, color_ color = black_) : Tree_Node<T>::Tree_Node(key) {
            color__ = color;
        }
    };
}
#endif /* node_h */
