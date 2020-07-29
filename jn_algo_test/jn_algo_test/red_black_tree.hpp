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
        void insert(T);
        RB_Node<T>* find(T) const;
        void remove(T);
        RB_Node<T>* min(RB_Node<T>*) const;
        T min() const;
        std::size_t size() const;
        void inorder_walk() const;
    private:
        RB_Node<T>* root_;
        RB_Node<T>* nil_;
        std::size_t size_;
        void fill_tree(RB_Node<T>*,RB_Node<T>*,RB_Node<T>*,RB_Node<T>*);
        void deallocate_nodes(RB_Node<T>*);
        void left_rotate(RB_Node<T>*);
        void right_rotate(RB_Node<T>*);
        void insert_fixup(RB_Node<T>*);
        void transplant (RB_Node<T>*,RB_Node<T>*);
        void remove(RB_Node<T>*);
        void remove_fixup(RB_Node<T>*);
        void inorder_walk(RB_Node<T>*) const;
    };
    
    template <typename T>
    RB_Tree<T>::RB_Tree() : root_{nullptr}, nil_{new RB_Node<T>()}, size_{0} {
        root_ = nil_;
    }
    
    template <typename T>
    RB_Tree<T>::RB_Tree(T key) : root_{new RB_Node<T>(key)}, nil_{new RB_Node<T>()}, size_{1} {
        root_->color_ = RB_Node<T>::black_;
        nil_->color_ = RB_Node<T>::black_;
        root_->parent_ = nil_;
        root_->left_ = nil_;
        root_->right_ = nil_;
    }
    
    template <typename T>
    RB_Tree<T>::RB_Tree(T* data, std::size_t size) {
        if (size <= 0)
            throw new std::runtime_error("Invalid size");
        size_ = size;
        nil_ = new RB_Node<T>();
        root_ = new RB_Node<T>(*data);
        root_->parent_ = nil_;
        root_->left_ = nil_;
        root_->right_ = nil_;
        
        for (int ii = 1; ii < size; ii++) {
            inorder_walk();
            insert(*(data + ii));
        }
        inorder_walk();
    }
    
    template <typename T>
    RB_Tree<T>::~RB_Tree() {
        size_ = 0;
        deallocate_nodes(root_);
        delete nil_;
    }
    
    template <typename T>
    void RB_Tree<T>::deallocate_nodes(RB_Node<T>* node) {
        if (node == nil_)
            return;
        deallocate_nodes(node->left_);
        deallocate_nodes(node->right_);
        delete node;
        node = nullptr;
    }
    
    template <typename T>
    RB_Tree<T>::RB_Tree(RB_Tree<T>& old_tree) {
        size_ = old_tree.size_;
        if (size_ == 0) {
            root_ = nullptr;
            nil_ = nullptr;
        }
        else {
            root_ = new RB_Node<T>(old_tree.root_->key_);
            nil_ = new RB_Node<T>();
            root_->color_ = old_tree.root_->color_;
            root_->parent_ = nil_;
            fill_tree(root_->left_, old_tree.root_->left_);
            fill_tree(root_->right_, old_tree.root_->right_);
        }
    }
    
    template <typename T>
    RB_Tree<T>& RB_Tree<T>::operator=(RB_Tree<T>& old_tree) {
        // deallocate the old bst and build the new one
        deallocate_nodes(root_);
        delete nil_;
        
        // now build the tree
        size_ = old_tree.size_;
        if (size_ == 0) {
            root_ = nullptr;
            nil_ = nullptr;
        }
        else {
            root_ = new RB_Node<T>(old_tree.root_->key_);
            root_->color_ = old_tree.root_->color_;
            nil_ = new RB_Node<T>();
            root_->parent_ = nil_;
            fill_tree(root_->left_, nil_, old_tree.root_->left_, old_tree.nil_);
            fill_tree(root_->right_, nil_, old_tree.root_->right_, old_tree.nil_);
        }
        return *this;
    }
    
    // Helper function to copy nodes
    template <typename T>
    void RB_Tree<T>::fill_tree(RB_Node<T>* member_node, RB_Node<T>* member_nil,
                               RB_Node<T>* tocopy_node, RB_Node<T>* tocopy_nil) {
        if (tocopy_node == tocopy_nil)
            member_node = member_node = member_nil;
        else {
            member_node = new RB_Node<T>(tocopy_node->key_);
            member_node->color_ = tocopy_node->color_;
            if (tocopy_node->left_ == tocopy_nil)
                member_node->left_ = member_nil;
            else
                fill_tree(member_node->left_, tocopy_node->left_);
            if (tocopy_node->right_ == tocopy_nil)
                member_node->right_ = member_nil;
            else
                fill_tree(member_node->right_, member_nil, tocopy_node->right_, tocopy_nil);
        }
    }
    
    template <typename T>
    RB_Tree<T>::RB_Tree(RB_Tree<T>&& old_tree) : size_{old_tree.size_}, root_{old_tree.root_}, nil_{old_tree.nil_} {
        old_tree.size_ = 0;
        old_tree.root_ = nullptr;
        old_tree.nil_ = nullptr;
    }
    
    template <typename T>
    RB_Tree<T>& RB_Tree<T>::operator=(RB_Tree<T>&& old_tree) {
        deallocate_nodes(root_);
        delete nil_;
        nil_ = old_tree.nil_;
        size_ = old_tree.size_;
        root_ = old_tree.root_;
        old_tree.size_ = 0;
        old_tree.root_ = nullptr;
        old_tree.nil_ = nullptr;
        return *this;
    }
    
    template <typename T>
    std::size_t RB_Tree<T>::size() const { return size_; }
    
    template <typename T>
    void RB_Tree<T>::left_rotate(RB_Node<T>* node) {
        RB_Node<T>* tmp = node->right_;
        node->right_ = tmp->left_;
        
        if (tmp->left_ != nil_)
            tmp->left_->parent_ = node;
        tmp->parent_ = node->parent_;
        if (node->parent_ == nil_)
            root_ = tmp;
        else if (node == node->parent_->left_)
            node->parent_->left_ = tmp;
        else
            node->parent_->right_ = tmp;
        tmp->left_ = node;
        node->parent_ = tmp;
    }
    
    template <typename T>
    void RB_Tree<T>::right_rotate(RB_Node<T>* node) {
        RB_Node<T>* tmp = node->left_;
        node->left_ = tmp->right_;
        
        if (tmp->right_ != nil_)
            tmp->right_->parent_ = node;
        tmp->parent_ = node->parent_;
        if (node->parent_ == nil_)
            root_ = tmp;
        else if (node == node->parent_->left_)
            node->parent_->left_ = tmp;
        else
            node->parent_->right_ = tmp;
        tmp->right_ = node;
        node->parent_ = tmp;
    }
    
    template <typename T>
    void RB_Tree<T>::insert(T key) {
        RB_Node<T>* new_node = new RB_Node<T>(key);
        RB_Node<T>* tmp1 = nil_;
        RB_Node<T>* tmp2 = root_;
        while (tmp2 != nil_) {
            tmp1 = tmp2;
            if (new_node->key_ < tmp2->key_)
                tmp2 = tmp2->left_;
            else
                tmp2 = tmp2->right_;
        }
        new_node->parent_ = tmp1;
        if (tmp1 == nil_)
            root_ = new_node;
        else if (new_node->key_ < tmp1->key_)
            tmp1->left_ = new_node;
        else
            tmp1->right_ = new_node;
        new_node->left_ = nil_;
        new_node->right_ = nil_;
        new_node->color_ = RB_Node<T>::red_;
        insert_fixup(new_node);
    }
    
    template <typename T>
    void RB_Tree<T>::insert_fixup(RB_Node<T>* node) {
        while (node->parent_->color_ == RB_Node<T>::red_) { // what if node is root_?
            if (node->parent_ == node->parent_->parent_->left_) {
                RB_Node<T>* tmp = node->parent_->parent_->right_;
                if (tmp->color_ == RB_Node<T>::red_) {
                    node->parent_->color_ = RB_Node<T>::black_;
                    tmp->color_ = RB_Node<T>::black_;
                    node->parent_->parent_->color_ = RB_Node<T>::red_;
                }
                else {
                    if (node == node->parent_->right_) {
                        node = node->parent_;
                        left_rotate(node);
                    }
                    node->parent_->color_ = RB_Node<T>::black_;
                    node->parent_->parent_->color_ = RB_Node<T>::red_;
                    right_rotate(node->parent_->parent_);
                }
            }
            else {
                RB_Node<T>* tmp = node->parent_->parent_->left_;
                if (tmp->color_ == RB_Node<T>::red_) {
                    node->parent_->color_ = RB_Node<T>::black_;
                    tmp->color_ = RB_Node<T>::black_;
                    node->parent_->parent_->color_ = RB_Node<T>::red_;
                }
                else {
                    if (node == node->parent_->left_) {
                        node = node->parent_;
                        right_rotate(node);
                    }
                    node->parent_->color_ = RB_Node<T>::black_;
                    node->parent_->parent_->color_ = RB_Node<T>::red_;
                    left_rotate(node->parent_->parent_);
                }
            }
        }
        root_->color_ = RB_Node<T>::black_;
    }
    
    template <typename T>
    void RB_Tree<T>::transplant(RB_Node<T>* u, RB_Node<T>* v) {
        if (u->parent_ == nil_)
            root_ = v;
        else if (u == u->parent_->left_)
            u->parent_->left_ = v;
        else
            u->parent_->right_ = v;
        v->parent_ = u->parent_;
    }
    
    template <typename T>
    void RB_Tree<T>::remove(T key) {
        RB_Node<T>* r_node = find(key);
        RB_Node<T>* tmp1 = r_node;
        RB_Node<T>* tmp2 = nil_;
        int orig_color = RB_Node<T>::black_;
        
        if (r_node->left_ == nil_) {
            tmp2 = r_node->right_;
            transplant(r_node, r_node->right_);
        }
        else if (r_node->right_ == nil_) {
            tmp2 = r_node->left_;
            transplant(r_node, r_node->left_);
        }
        else {
            tmp1 = min(r_node->right_);
            orig_color = tmp1->color_;
            tmp2 = tmp1->right_;
            if (tmp1->parent_ == r_node)
                tmp2->parent_ = tmp1;
            else {
                transplant(tmp1, tmp1->right_);
                tmp1->right_ = r_node->right_;
                tmp1->right_->parent_ = tmp1;
            }
            transplant(r_node, tmp1);
            tmp1->left_ = r_node->left_;
            tmp1->left_->parent_ = tmp1;
            tmp1->color_ = r_node->color_;
        }
        if (orig_color == RB_Node<T>::black_)
            remove_fixup(tmp2);
    }
    
    template <typename T>
    void RB_Tree<T>::remove_fixup(RB_Node<T>* node) {
        while (node != root_ && node->color_ == RB_Node<T>::black_) {
            if (node == node->parent_->left_) {
                RB_Node<T>* tmp = node->parent_->right_;
                if (tmp->color_ == RB_Node<T>::red_) {
                    tmp->color_ = RB_Node<T>::black_;
                    node->parent_->color_ = RB_Node<T>::red_;
                    left_rotate(node->parent_);
                    tmp = node->parent_->right_;
                }
                
                if (tmp->left_->color_ == RB_Node<T>::black_ &&
                    tmp->right_->color == RB_Node<T>::black_) {
                    tmp->color_ = RB_Node<T>::red_;
                    node = node->parent_;
                }
                else {
                    if (tmp->right_->color_ == RB_Node<T>::black_) {
                        tmp->left_->color_ = RB_Node<T>::black_;
                        tmp->color_ = RB_Node<T>::red_;
                        right_rotate(tmp);
                        tmp = node->parent_->right;
                    }
                    tmp->color_ = node->parent_->color_;
                    node->parent_->color_ = RB_Node<T>::black_;
                    left_rotate(node->parent_);
                    node = root_;
                }
            }
            else {
                RB_Node<T>* tmp = node->parent_->left_;
                if (tmp->color_ == RB_Node<T>::red_) {
                    tmp->color_ = RB_Node<T>::black_;
                    node->parent_->color_ = RB_Node<T>::red_;
                    right_rotate(node->parent_);
                    tmp = node->parent_->left_;
                }
                
                if (tmp->right_->color_ == RB_Node<T>::black_ &&
                    tmp->left_->color == RB_Node<T>::black_) {
                    tmp->color_ = RB_Node<T>::red_;
                    node = node->parent_;
                }
                else {
                    if (tmp->left_->color_ == RB_Node<T>::black_) {
                        tmp->right_->color_ = RB_Node<T>::black_;
                        tmp->color_ = RB_Node<T>::red_;
                        left_rotate(tmp);
                        tmp = node->parent_->left_;
                    }
                    tmp->color_ = node->parent_->color_;
                    node->parent_->color_ = RB_Node<T>::black_;
                    right_rotate(node->parent_);
                    node = root_;
                }
            }
        }
    }
    
    template <typename T>
    T RB_Tree<T>::min() const {
        if (root_ == nullptr)
            return std::numeric_limits<T>::quiet_NaN();
        RB_Node<T>* node = root_;
        while (node->left_ != nil_)
            node = node->left_;
        return node->key_;
    }
    
    template <typename T>
    RB_Node<T>* RB_Tree<T>::min(RB_Node<T>* node) const {
        if (node == nil_)
            return nil_;
        while (node->left_ != nil_)
            node = node->left_;
        return node;
    }
    
    template <typename T>
    RB_Node<T>* RB_Tree<T>::find(T key) const {
        RB_Node<T>* curr = root_;
        while (curr != nil_ && key != curr->key_)
            (key < curr->key_) ? curr = curr->left_ : curr = curr->right_;
        return curr;
    }
    
    template <typename T>
    void RB_Tree<T>::inorder_walk() const {
        inorder_walk(root_);
        std::cout << std::endl;
    }
    
    template <typename T>
    void RB_Tree<T>::inorder_walk(RB_Node<T>* node) const {
        if (node != nil_) {
            inorder_walk(node->left_);
            std::cout << node->key_ << ",";
            (node->color_ == RB_Node<T>::black_) ?
                std::cout << "Black  " : std::cout << "Red  ";
            inorder_walk(node->right_);
        }
    }
    
} // End clrs_impl_jn

#endif /* red_black_tree_hpp */
