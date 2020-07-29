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
	    Tree_Node(T key = T(), Tree_Node<T>* parent = nullptr, Tree_Node<T>* left = nullptr, Tree_Node<T>* right = nullptr)
        : key_{key}, parent_{parent}, left_{left}, right_{right} {}
        Tree_Node* parent_;
	    Tree_Node* left_;
	    Tree_Node* right_;
	    T key_;
    };
    
    template <typename T>
    struct RB_Node {
        enum color_t {black_, red_};
        color_t color_;
        RB_Node* parent_;
        RB_Node* left_;
        RB_Node* right_;
        T key_;
        RB_Node(T key = T(), color_t color = black_,
                RB_Node<T>* parent = nullptr, RB_Node<T>* left = nullptr,
                RB_Node<T>* right = nullptr)
        : key_{key}, color_{color}, parent_{parent}, left_{left}, right_{right} {}
    };
}
#endif /* node_h */
