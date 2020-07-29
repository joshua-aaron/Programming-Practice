//
//  red_black_tree.hpp
//  jn_algo_test
//
//  Created by Joshua Navon on 7/28/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#ifndef red_black_tree_hpp
#define red_black_tree_hpp
#include "node.hpp"
namespace clrs_impl_jn {
    template <typename T>
    class RB_Tree {
    public:
        RB_Tree();
        RB_Tree(T);
        RB_Tree(T*,std::size_t);
        ~RB_Tree();
        RB_Tree(RB_Tree&);
        RB_Tree& operator=(RB_Tree&);
        RB_Tree(RB_Tree&&);
        RB_Tree& operator=(RB_Tree&&);
        std::size_t size() const;
    private:
        RB_Node<T>* root_;
        std::size_t size_;
    };
    
    template <typename T>
    RB_Tree<T>::RB_Tree() : root_{nullptr}, size_{0} {}
    
    template <typename T>
    RB_Tree<T>::RB_Tree(T key) {}
    
    template <typename T>
    RB_Tree<T>::RB_Tree(T* data, std::size_t size) {}
    
    template <typename T>
    RB_Tree<T>::~RB_Tree() {}
    
    template <typename T>
    RB_Tree<T>::RB_Tree(RB_Tree<T>& old_tree) {}
    
    template <typename T>
    RB_Tree<T>& RB_Tree<T>::operator=(RB_Tree<T>& old_tree) {}
    
    template <typename T>
    RB_Tree<T>::RB_Tree(RB_Tree<T>&& old_tree) {}
    
    template <typename T>
    RB_Tree<T>& RB_Tree<T>::operator=(RB_Tree<T>&& old_tree) {}
    
    template <typename T>
    std::size_t RB_Tree<T>::size() const { return size_; }
    
} // End clrs_impl_jn

#endif /* red_black_tree_hpp */
