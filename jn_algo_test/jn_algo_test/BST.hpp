//
//  BST.h
//  jn_algo_test
//
//  Created by Joshua Navon on 7/28/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#ifndef BST_hpp
#define BST_hpp
#include "node.hpp"
#include <limits>

namespace clrs_impl_jn {
    template <typename T>
    class BST {
    public:
        BST();
        BST(T);
        BST(T*,std::size_t);
        ~BST();
        BST(const BST&);
        BST& operator=(const BST&);
        BST(BST&&);
        BST& operator=(BST&&);
        std::size_t size() const;
        T max() const;
        Tree_node<T>* max(Tree_node<T>*) const;
        T min() const;
        Tree_node<T>* min(Tree_node<T>*) const;
        Tree_node<T>* successor(Tree_node<T>*) const;
        Tree_node<T>* predecessor(Tree_node<T>*) const;
        bool contains(T) const;
        Tree_node<T>* find(T) const;
        void remove(T);
        void insert(T);
        void inorder_walk() const;
        void postorder_walk() const;
        void preorder_walk() const;
    private:
        void inorder_walk(Tree_node<T>*) const;
        void postorder_walk(Tree_node<T>*) const;
        void preorder_walk(Tree_node<T>*) const;
        void delete_node(Tree_node<T>*);
        void transplant(Tree_node<T>*,Tree_node<T>*);
        void deallocate_nodes(Tree_node<T>*);
        void fill_tree(Tree_node<T>*,Tree_node<T>*);
        Tree_node<T>* root_;
        std::size_t size_;
    };
    
    template <typename T>
    BST<T>::BST() : root_{nullptr}, size_{0} {}
    
    template <typename T>
    BST<T>::BST(T key) : root_{new Tree_node<T>(key)}, size_{1} {
        root_->parent_ = nullptr;
    }
    
    template <typename T>
    BST<T>::BST(T* data, size_t size) {
        if (size <= 0)
            throw new std::runtime_error("Invalid size");
        size_ = size;
        root_ = new Tree_node<T>(*data);
        root_->parent_ = nullptr;
        
        for (int ii = 1; ii < size; ii++)
            insert(*(data + ii));
    }
    
    template <typename T>
    BST<T>::~BST() {
        size_ = 0;
        deallocate_nodes(root_);
    }
    
    template <typename T>
    void BST<T>::deallocate_nodes(Tree_node<T>* node) {
        if (node == nullptr)
            return;
        deallocate_nodes(node->left_);
        deallocate_nodes(node->right_);
        delete node;
        node = nullptr;
    }
    
    // Deep Copy
    template <typename T>
    BST<T>::BST(const BST& old_bst) {
        size_ = old_bst.size_;
        if (size_ == 0)
            root_ = nullptr;
        else {
            root_ = new Tree_node<T>(old_bst.root_->key);
            root_->parent_ = nullptr;
            fill_tree(root_->left_, old_bst.root_->left_);
            fill_tree(root_->right_, old_bst.root_->right_);
        }
    }
    
    template <typename T>
    BST<T>& BST<T>::operator=(const BST<T>& old_bst) {
        // deallocate the old bst and build the new one
        deallocate_nodes(root_);
        
        // now build the tree
        size_ = old_bst.size_;
        if (size_ == 0)
            root_ = nullptr;
        else {
            root_ = new Tree_node<T>(old_bst.root_->key);
            fill_tree(root_->left_, old_bst.root_->left_);
            fill_tree(root_->right_, old_bst.root_->right_);
        }
        return *this;
    }
    
    // Helper function to copy nodes
    template <typename T>
    void BST<T>::fill_tree(Tree_node<T>* member_node, Tree_node<T>* tocopy_node) {
        if (tocopy_node == nullptr)
            member_node = nullptr;
        else {
            member_node = new Tree_node<T>(tocopy_node->key);
            if (tocopy_node->left_ == nullptr)
                member_node->left_ = nullptr;
            else
                fill_tree(member_node->left_, tocopy_node->left_);
            if (tocopy_node->right_ == nullptr)
                member_node->right_ = nullptr;
            else
                fill_tree(member_node->right_, tocopy_node->right_);
        }
    }
    
    template <typename T>
    BST<T>::BST(BST&& old_bst) : size_{old_bst.size_}, root_{old_bst.root_} {
        old_bst.size_ = 0;
        old_bst.root_ = nullptr;
    }
    
    template <typename T>
    BST<T>& BST<T>::operator=(BST&& old_bst) {
        deallocate_nodes(root_);
        size_ = old_bst.size_;
        root_ = old_bst.root_;
        old_bst.size_ = 0;
        old_bst.root_ = nullptr;
        return *this;
    }
    
    template <typename T>
    T BST<T>::max() const {
        if (root_ == nullptr)
            return std::numeric_limits<T>::quiet_NaN();
        Tree_node<T>* node = root_;
        while (node->right_ != nullptr)
            node = node->right_;
        return node->key_;
    }
    
    template <typename T>
    T BST<T>::min() const {
        if (root_ == nullptr)
            return std::numeric_limits<T>::quiet_NaN();
        Tree_node<T>* node = root_;
        while (node->left_ != nullptr)
            node = node->left_;
        return node->key_;
    }
    
    template <typename T>
    Tree_node<T>* BST<T>::max(Tree_node<T>* node) const {
        if (node == nullptr)
            return nullptr;
        while (node->right_ != nullptr)
            node = node->right_;
        return node;
    }
    
    template <typename T>
    Tree_node<T>* BST<T>::min(Tree_node<T>* node) const {
        if (node == nullptr)
            return nullptr;
        while (node->left_ != nullptr)
            node = node->left_;
        return node;
    }
    
    template <typename T>
    Tree_node<T>* BST<T>::successor(Tree_node<T>* node) const {
        if (node->right_ != nullptr)
            return min(node->right_);
        Tree_node<T>* parent = node->parent_;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent_;
        }
        return parent;
    }
    
    template <typename T>
    Tree_node<T>* BST<T>::predecessor(Tree_node<T>* node) const {
        if (node == nullptr || node->left_ == nullptr)
            return nullptr;
        else
            return max(node->left_);
    }
    
    template <typename T>
    std::size_t BST<T>::size() const { return size_; }
    
    template <typename T>
    void BST<T>::inorder_walk() const {
        inorder_walk(root_);
        std::cout << std::endl;
    }
    
    template <typename T>
    void BST<T>::postorder_walk() const {
        postorder_walk(root_);
        std::cout << std::endl;
    }
    
    template <typename T>
    void BST<T>::preorder_walk() const {
        preorder_walk(root_);
        std::cout << std::endl;
    }
    
    template <typename T>
    void BST<T>::inorder_walk(Tree_node<T>* node) const {
        if (node != nullptr) {
            inorder_walk(node->left_);
            std::cout << node->key_ << " ";
            inorder_walk(node->right_);
        }
    }
    
    template <typename T>
    void BST<T>::postorder_walk(Tree_node<T>* node) const {
        if (node != nullptr) {
            postorder_walk(node->left_);
            postorder_walk(node->right_);
            std::cout << node->key_ << " ";
        }
    }
    
    template <typename T>
    void BST<T>::preorder_walk(Tree_node<T>* node) const {
        if (node != nullptr) {
            std::cout << node->key_ << " ";
            preorder_walk(node->left_);
            preorder_walk(node->right_);
        }
    }
    
    template <typename T>
    bool BST<T>::contains(T key) const {
        Tree_node<T>* curr = root_;
        while (curr != nullptr && key != curr->key_)
            (key < curr->key_) ? curr = curr->left_ : curr = curr->right_;
        return (curr == nullptr) ? false : true;
    }
    
    template <typename T>
    Tree_node<T>* BST<T>::find(T key) const {
        Tree_node<T>* curr = root_;
        while (curr != nullptr && key != curr->key_)
            (key < curr->key_) ? curr = curr->left_ : curr = curr->right_;
        return curr;
    }
    
    template <typename T>
    void BST<T>::insert(T key) {
        Tree_node<T>* key_node{new Tree_node<T>(key)};
        Tree_node<T>* tmp{nullptr};
        Tree_node<T>* curr{root_};
        while (curr != nullptr) {
            tmp = curr;
            if (key < curr->key_)
                curr = curr->left_;
            else
                curr = curr->right_;
        }
        key_node->parent_ = tmp;
        if (tmp == nullptr)
            root_ = key_node;
        else if (key_node->key_ < tmp->key_)
            tmp->left_ = key_node;
        else
            tmp->right_ = key_node;
    }
    
    template <typename T>
    void BST<T>::remove(T key) {
        Tree_node<T>* node = find(key);
        delete_node(node);
    }
    
    template <typename T>
    void BST<T>::transplant(Tree_node<T>* p, Tree_node<T>* u) {
        if (p->parent_ == nullptr)
            root_ = u;
        else if (p == p->parent_->left_)
            p->parent_->left_ = u;
        else
            p->parent_->right_ = u;
        if (u != nullptr)
            u->parent_ = p->parent_;
    }
    
    template <typename T>
    void BST<T>::delete_node(Tree_node<T>* node) {
        if (node->left_ == nullptr)
            transplant(node, node->right_);
        else if (node->right_ == nullptr)
            transplant(node, node->left_);
        else {
            Tree_node<T>* min = min(node->right_);
            if (min->parent_ != node) {
                transplant(min, min->right_);
                min->right_ = node->right_;
                min->right_->parent_ = min;
            }
            transplant(node, min);
            min->left_ = node->left_;
            min->left_->parent_ = min;
        }
    }
    
} // End clrs::impl::jn

#endif /* BST_hpp */
