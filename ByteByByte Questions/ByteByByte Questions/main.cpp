//
//  main.cpp
//  ByteByByte Questions
//
//  Created by Joshua Navon on 7/31/20.
//  Copyright © 2020 Joshua Navon. All rights reserved.
//

#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <utility>
#include <algorithm>
#include <exception>
#include <stack>

// Q1: Media of arrays
template <typename T>
double median(T* a1, std::size_t s1, T* a2, std::size_t s2) {
    std::size_t s3 = s1 + s2;
    if (s3 <= 0)
        return std::numeric_limits<T>::quiet_NaN();
    T* merge = new T(s3);
    std::size_t ii = 0; //a1 idx
    std::size_t jj = 0; // a2 idx
    for (std::size_t kk = 0; kk < s3; kk++) {
        if (ii >= s1)
            *(merge + kk) = *(a2 + jj++);
        else if (jj >= s2)
            *(merge + kk) = *(a1 + ii++);
        else
            (*(a1 + ii) < *(a2 + jj)) ? *(merge + kk) = *(a1 + ii++) :
                *(merge + kk) = *(a2 + jj++);
    }
    
    if (s3 % 2 == 1)
        return *(merge + s3/2);
    else
        return (*(merge + s3/2) + *(merge + s3/2 - 1)) / 2.0;
}

template <typename T, typename U>
U knapsack(std::vector<std::tuple<T,U>> items, T max_weight) {
    if (items.size() <= 0)
        return std::numeric_limits<T>::quiet_NaN();
    else if (items.size() == 1) {
        if (std::get<0>(items[0]) > max_weight)
            return std::numeric_limits<T>::quiet_NaN();
        else
            return std::get<1>(items[0]);
    }
    else {
        std::size_t k_comb_num = items.size() - 1;
        T curr_weight = 0;
        U curr_max = 0;
        int num_shifts = 0;
        std::vector<U> sums;
        typename std::vector<std::tuple<T,U>>::iterator it = items.begin();
        
        // First get sum of all elements and sum of individual elements
        while (it != items.end()) {
            if (std::get<0>(*it) <= max_weight)
                sums.push_back(std::get<1>(*it));
            curr_max += std::get<1>(*it);
            curr_weight += std::get<0>(*(it++));
        }
        if (curr_weight <= max_weight)
            sums.push_back(curr_max);
        
        curr_weight = 0;
        curr_max = 0;
        it = items.begin();
        
        // Now we start the permutations until k_comb_num = 1
        while (num_shifts <= k_comb_num) {
            // Get the sum of the sub-array
            while (it != (items.begin() + k_comb_num)) {
                curr_weight += std::get<0>(*it);
                curr_max += std::get<1>(*it++);
            }
            if (curr_weight <= max_weight)
                sums.push_back(curr_max);
            
            std::vector<std::tuple<T,U>> tmpvec(items.begin(), items.begin() + k_comb_num);
            sums.push_back(knapsack(tmpvec, max_weight));
            std::rotate(items.begin(), items.begin() + 1, items.end()); // Left shift
            num_shifts++;
            curr_weight = 0;
            curr_max = 0;
            it = items.begin();
        }
        for (U p : sums) {
            if (p > curr_max)
                curr_max = p;
        }
        return curr_max;
    }
}

template <typename T>
T greatest_product(T** mat, std::size_t m, std::size_t n) {
    T** min_cache = new T*[m];
    T** max_cache = new T*[m];
    
    for (int ii = 0; ii < n; ii++) {
        min_cache[ii] = new T[n];
        max_cache[ii] = new T[n];
    }
    
    // Fill the max and min caches with zeros
    for (int ii = 0; ii < m; ii ++) {
        for (int jj = 0; jj < n; jj++) {
            max_cache[ii][jj] = reinterpret_cast<T>(0);
            min_cache[ii][jj] = reinterpret_cast<T>(0);
        }
    }
    
    // Fill the caches
    for (int ii = 0; ii < m; ii ++) {
        for (int jj = 0; jj < n; jj++) {
            T maxv = std::numeric_limits<T>::min();
            T minv = std::numeric_limits<T>::max();
            
            // if we're at top left, simply fill the value and continue
            if (ii == 0 && jj == 0) {
                minv = mat[ii][jj];
                maxv = mat[ii][jj];
            }
            // Going along left column, fill taking min values in the previous spot
            if (ii > 0) {
                T tmp_max = std::max(mat[ii][jj] * min_cache[ii - 1][jj],
                                       mat[ii][jj] * max_cache[ii - 1][jj]);
                T tmp_min = std::min(mat[ii][jj] * min_cache[ii - 1][jj],
                                       mat[ii][jj] * max_cache[ii - 1][jj]);
                maxv = std::max(maxv, tmp_max);
                minv = std::min(minv, tmp_min);
            }
            // Going along top row, fill taking min values in the previous spot
            if (jj > 0) {
                T tmp_max = std::max(mat[ii][jj] * min_cache[ii][jj - 1], mat[ii][jj] * max_cache[ii][jj - 1]);
                T tmp_min = std::min(mat[ii][jj] * min_cache[ii][jj - 1], mat[ii][jj] * max_cache[ii][jj - 1]);
                maxv = std::max(maxv, tmp_max);
                minv = std::min(minv, tmp_min);
            }
            min_cache[ii][jj] = minv;
            max_cache[ii][jj] = maxv;
        }
    }
    return max_cache[m-1][n-1];
}

std::vector<int> duplicates(int* p, std::size_t size) {
    std::vector<int> found_vals;
    
    for (int ii = 0; ii < size; ii++) {
        // If the current element already exists within found_vals, skip that element
        if (std::find(found_vals.begin(), found_vals.end(), p[ii]) != found_vals.end())
            continue;
        for (int jj = ii + 1; jj < size; jj++) {
            if (p[ii] == p[jj]) {
                found_vals.push_back(p[ii]);
                break;
            }
        }
    }
    
    return found_vals;
}


//Given an unsorted array, find the length of the longest sequence of consecutive numbers in the array
template <typename T>
std::vector<std::pair<std::size_t, std::vector<T>>> consecutive(T* arr, std::size_t size) {
    std::size_t idx = 0;
    std::size_t tmp_idx = 0;
    std::size_t seq_len = 0;
    std::size_t curr_max_len = 0;
    std::sort(arr, arr + size);
    std::vector<std::pair<std::size_t, std::vector<T>>> con_vec;
    std::vector<T> sequences;
    
    while (idx < size) {
        seq_len = 1;
        std::size_t next_idx = idx + 1;
        tmp_idx = idx;
        sequences.push_back(*(arr + idx));
        
        while (next_idx < size) {
            if (*(arr + next_idx) == *(arr + tmp_idx) + 1) {
                sequences.push_back(*(arr + next_idx));
                seq_len++;
                next_idx++;
                tmp_idx++;
            }
            else
                break;
        }
        idx++;
        if (seq_len > curr_max_len) {
            curr_max_len = seq_len;
            con_vec.clear();
            con_vec.push_back(std::make_pair(seq_len, sequences));
        }
        else if (seq_len == curr_max_len)
            con_vec.push_back(std::make_pair(seq_len, sequences));
        sequences.clear();
    }
    return con_vec;
}


void transform_mat(bool mat[3][3], std::size_t m, std::size_t n) {
    //m x n matrix
    bool res[3][3] = {{false, false, false}, {false, false, false}, {false, false, false}};
    bool col[3] = {false, false, false};
    bool row[3] = {false, false, false};
    for (int ii = 0; ii < m; ii++) {
        for (int jj = 0; jj < n; jj++) {
            // If true, set the row and column
            if (mat[ii][jj]) {
                // Set the columns
                if (!row[ii]) {
                    row[ii] = true;
                    for (int pp = 0; pp < n; pp++)
                        res[ii][pp] = true;
                }
                if (!col[jj]) {
                    col[ii] = true;
                    // Set the rows
                    for (int pp = 0; pp < m; pp++)
                        res[pp][jj] = true;
                }
            }
        }
    }
    for (int ii = 0; ii < m; ii++)
    for (int jj = 0; jj < n; jj++)
        mat[ii][jj] = res[ii][jj];
}


bool has_true(bool** mat, std::size_t m, std::size_t n) {
    for (int ii = 0; ii < m; ii++) {
        for (int jj = 0; jj < n; jj++) {
            if (mat[ii][jj])
                return true;
        }
    }
    return false;
}

bool has_false(bool** mat, std::size_t m, std::size_t n) {
    for (int ii = 0; ii < m; ii++) {
        for (int jj = 0; jj < n; jj++) {
            if (!mat[ii][jj])
                return true;
        }
    }
    return false;
}

// helper that takes a square size arg
std::size_t largest_square_matrix(bool** mat, std::size_t m, std::size_t n, std::size_t sq_size) {
    if (sq_size == 1 && has_true(mat, m, n))
        return 1;
    std::size_t max = 0;
    bool zero_flag = false;
    
    for (int ii = 0; ii < m; ii++) {
        for (int jj = 0; jj < n; jj ++) {
            if (ii + sq_size < m && jj + sq_size < n) {
                for (int pp = ii; pp < ii + sq_size; pp++) {
                    for (int qq = jj; qq < jj + sq_size; qq++) {
                        if (!mat[pp][qq]) {
                            zero_flag = true;
                            break;
                        }
                    }
                    if (zero_flag)
                        break;
                }
                if (!zero_flag)
                    max = sq_size;
            }
            zero_flag = false;
        }
    }
    if (max > 0)
        return max;
    else
        return largest_square_matrix(mat, max, n, sq_size - 1);
}

// Takes a m x n matrix of booleans
std::size_t largest_square_matrix(bool** mat, std::size_t m, std::size_t n) {
    if (m <= 0 || n <= 0)
        return 0;
    else if ((m == 1 || n == 1) && has_true(mat, m, n))
        return 1;
    else if (!has_false(mat, m, n))
        return (m < n) ? m : n;
    else
        return largest_square_matrix(mat, m, n, (m < n) ? m - 1 : n - 1);
}


template <typename T>
void merge(std::vector<T>& merge_vec, std::vector<T> new_vec) {
    int ii = 0;
    int jj = 0;
    while (jj < new_vec.size()) {
        if (ii >= merge_vec.size())
            merge_vec.push_back(new_vec[jj++]);
        else {
            if (merge_vec[ii] <= new_vec[jj])
                ii++;
            else
                merge_vec.insert(merge_vec.begin() + ii, new_vec[jj++]);
        }
    }
}

template <typename T>
std::vector<T> merge(std::vector<std::vector<T>> vecs) {
    std::vector<T> merged;
    for (auto vec : vecs) {
        merge(merged, vec);
    }
    return merged;
}

// m x n matrix
template <typename T>
bool contains(T** mat, std::size_t m, std::size_t n, T val) {
    int ii = 0;
    for (ii = 0; ii < m; ii++) {
        if (val == mat[ii][0])
            return true;
        if (val < mat[ii][0]) {
            if (ii == 0)
                return false;
            else
                ii--;
            }
    }
    if (ii == m && val > mat [m-1][n-1])
        return false;
    else
        ii--;
    
    // Search the row. We don't need jj = 0 though since already checked
    for (int jj = 1; jj < n; jj++) {
        if (val < mat[ii][jj])
            return false;
        else if (val == mat[ii][jj])
            return true;
    }
    return false;
}


template <typename T>
int find_first_zero(T* a1, std::size_t size) {
    for (int ii = 0; ii < size; ii++)
        if (*(a1 + ii) == 0)
            return ii;
    return -1;
}

template <typename T>
void shift(T* a1, int first_zero, int shift_at) {
    for (int ii = first_zero - 1; ii >= shift_at; ii--) {
        *(a1 + ii + 1) = *(a1 + ii);
    }
}

template <typename T>
void merge_arrays(T* a1, std::size_t a1_size, T* a2, std::size_t a2_size) {
    // Find the first 0 in a1
    int start_idx = 0;
    int first_zero = find_first_zero(a1, a1_size);
    if (first_zero == -1 || a2_size > (a1_size - first_zero))
        throw std::length_error("No space in array to copy");
    for (int ii = 0; ii < a2_size; ii++) {
        for (int jj = start_idx; jj < a1_size; jj++) {
            if (*(a1 + jj) == 0 || *(a2 + ii) < *(a1 + jj)) {
                shift(a1, first_zero, jj);
                *(a1 + jj) = *(a2 + ii);
                first_zero++;
                start_idx = jj;
                break;
            }
        }
    }
}


template <typename T>
std::vector<std::vector<T>> zero(T* a1, std::size_t size) {
    std::vector<std::vector<T>> results;
    if (size <= 1)
        return results;
    int sub_size = size - 1;
    int sum = 0;
    while (sub_size > 0) {
        for (int ii = 0; ii < size; ii++) {
            sum = 0;
            if (ii + sub_size >= size)
                break;
            for (int jj = ii; jj < ii + sub_size; jj++) {
                sum += *(a1 + jj);
            }
            if (sum == 0) {
                results.push_back(std::vector<T>(a1 + ii, a1 + ii + sub_size));
            }
        }
        sub_size--;
    }
    return results;
}


template <typename T>
void printarr(T* a1, std::size_t size) {
    for (int ii = 0; ii < size; ii++)
        std::cout << *(a1 + ii) << " ";
    std::cout << std::endl;
}
template <typename T>
void permutations(T* a1, std::size_t size) {
    if (size == 0)
        return;
    if (size == 1) {
        std::cout << *a1 << std::endl;
        return;
    }
    T* tmp = new T(sizeof(a1)/sizeof(a1[0]));
    std::memcpy(tmp, a1, sizeof(T)*sizeof(a1));
    
    int swap_idx = size - 2;
    
    
    // Print & shift size - 1 times. Then permute and repeat
    while (swap_idx >= 0) {
        printarr(tmp, size);
        for (int jj = 0; jj < size - 1; jj++) {
            std::rotate(tmp, tmp + size - 1, tmp + size);
            printarr(tmp, size);
            
        }
        std::memcpy(tmp, a1, sizeof(T)*sizeof(a1));
        std::swap(*(tmp + size - 1), *(tmp + swap_idx--));
    }
    // Print the permutations from the final swap
    if (size > 3) {
        printarr(tmp, size);
        for (int jj = 0; jj < size - 1; jj++) {
            std::rotate(tmp, tmp + size - 1, tmp + size);
            printarr(tmp, size);
        }
    }
}


template <typename T>
class Stack {
public:
    Stack() =delete;
    Stack(int N, int capacity) {
        N_ = N;
        capacity_ = capacity;
        data_ = new T[capacity];
        top_of_stack_ = new int[N];
        next_idx_ = new int[N];
        
        for (int ii = 0; ii < N_ - 1; ii++) {
            top_of_stack_[ii] = (capacity_ / N_ * (ii + 1)) - 1; // Implicit floor bc of int division
        }
        top_of_stack_[N_ - 1] = capacity - 1;
        
        // Base case
        next_idx_[0] = 0;
        for (int ii = 1; ii < N_; ii++)
            next_idx_[ii] = top_of_stack_[ii - 1] + 1;
    }
    T pop(int stack_num) {
        if (stack_num >= N_)
            throw std::range_error("Stack out of range");
        
        // If the next element is the first element in the stack, then nothing to pop
        if ( next_idx_[stack_num] == 0 || next_idx_[stack_num] == top_of_stack_[stack_num - 1] + 1)
            throw std::runtime_error("Stack is empty. Nothing to pop()");
        
        T val =  data_[next_idx_[stack_num] - 1];
        next_idx_[stack_num]--;
        return val;
    }
    void push(int stack_num,T val) {
        if (stack_num >= N_)
            throw std::range_error("Stack out of range");
        
        // Throw an exception if the stack is full. Alternatively, we could insert in a different stack
        if (next_idx_[stack_num] == top_of_stack_[stack_num] + 1)
            throw std::runtime_error("Error: Stack is full. Cannot push value");
        data_[next_idx_[stack_num]++] = val;
    }
private:
    int N_;
    int capacity_;
    T* data_;
    int* top_of_stack_; // Top of each stack
    int* next_idx_; // The next index in each stack
};

bool isAnagram(std::string s1, std::string s2) {
    if (s1.size() != s2.size())
        return false;
    
    // We already know they're the same size.
    if (s1.size() == 0)
        return true;
    int ii = 0;
    int jj = s1.size() - 1;
    while (ii <= jj) {
        if (s1[ii++] != s2[jj--])
            return false;
    }
    return true;
}



void build_order(std::vector<std::vector<int>> processes) {
    std::vector<int> build_order;
    bool dependencies_found = true;
    for (int ii = 0; ii < processes.size(); ii++) {
        if (processes[ii].empty())
            build_order.push_back(ii);
    }
    if (build_order.size() == 0)
        throw std::invalid_argument("Nothing with 0 dependencies. Can't build.");
    while (processes.size() != build_order.size()) {
        for (int ii = 0; ii < processes.size(); ii++) {
            
            if (std::find(build_order.begin(), build_order.end(), ii) != build_order.end())
                continue;
            for (auto pp : processes[ii]) {
                if (std::find(build_order.begin(), build_order.end(), pp) == build_order.end())
                    dependencies_found = false;
            }
            if (dependencies_found)
                build_order.push_back(ii);
        }
    }
    for (auto pp : build_order)
        std::cout << pp << " ";
    std::cout <<std::endl;
}


struct Node {
    Node* pred;
    int distance;
    enum color_ {white, grey, black};
    color_ color;
    int node_num;
    
    Node(int num) {
        pred = nullptr;
        distance = -1;
        color = white;
        node_num = num;
    }
    
};

void print_path(std::vector<Node*> vertices, int src, int dest) {
    if (dest == src)
        std::cout << src << " -> ";
    else if (vertices.at(dest)->pred == nullptr)
        throw std::runtime_error("No path from dest");
    else {
        print_path(vertices, src, vertices.at(dest)->pred->node_num);
        std::cout << vertices.at(dest)->node_num << " -> ";
    }
}

// Using an adjecency list to represent the graph
void shortest_path(std::vector<int> adj[], int num_nodes, int src, int dest) {
    std::vector<Node*> vertices;
    
    for (int ii = 0; ii < num_nodes; ii++) {
        vertices.push_back(new Node(ii)); // The constructor handles the initialization step
    }
    vertices.at(src)->color = Node::grey;
    vertices.at(src)->distance = 0;
    
    std::vector<Node*> adj_queue;
    adj_queue.insert(adj_queue.begin(), vertices.at(src));
    while (!adj_queue.empty()) {
        Node* u = adj_queue.at(adj_queue.size() - 1);
        adj_queue.pop_back();
        for (int adj_node : adj[u->node_num]) {
            if (vertices.at(adj_node)->color == Node::white) {
                vertices.at(adj_node)->color = Node::grey;
                vertices.at(adj_node)->distance = u->distance + 1;
                vertices.at(adj_node)->pred = u;
                adj_queue.insert(adj_queue.begin(), vertices.at(adj_node));
            }
        }
        vertices.at(u->node_num)->color = Node::black;
    }
    print_path(vertices, src, dest);
}


//Q28
template <typename T>
void stack_sort(std::stack<T>& s1) {
    int count = 0;
    std::stack<T> aux;
    
    while (!s1.empty()) {
        T tmp_orig = s1.top();
        s1.pop();
        if (aux.empty() || tmp_orig > aux.top())
            aux.push(tmp_orig);
        else {
            while (!aux.empty() && tmp_orig <= aux.top()) {
                s1.push(aux.top());
                aux.pop();
                count++;
            }
            aux.push(tmp_orig);
            while (count > 0) {
                aux.push(s1.top());
                s1.pop();
                count--;
            }
        }
    }
    s1 = aux;
}


//Q32
void missing(int *arr, std::size_t size) {
    int missing_nums[2];
    int missing_idx = 0;
    for (int ii = 0; ii < size; ii++)
        std::cout << arr[ii] << " ";
    std::cout << std::endl;
    
    std::sort(arr, &arr[size]);
    
    for (int ii = 0; ii < size - 1; ii++) {
        if (arr[ii + 1] - arr[ii] == 2) {
            if (missing_idx > 1) {
                std::cout << "More than 2 missing numbers is the array" << std::endl;
                return;
            }
            else
                missing_nums[missing_idx++] = arr[ii] + 1;
        }
        if (arr[ii + 1] - arr[ii] == 3) {
            if (missing_idx != 0) {
                std::cout << "More than 2 missing numbers is the array" << std::endl;
                return;
            }
            else {
                missing_nums[missing_idx++] = arr[ii] + 1;
                missing_nums[missing_idx++] = arr[ii] + 2;
            }
        }
        if (arr[ii + 1] - arr[ii] > 3) {
            std::cout << "More than 2 missing numbers is the array" << std::endl;
            return;
        }
        
    }
    if (missing_idx == 0) {
        missing_nums[0] = arr[0] - 1;
        missing_nums[1] = arr[size - 1] + 1;
    }
    else if (missing_idx == 1)
        missing_nums[1] = arr[size - 1] + 1;
    
    std::cout << missing_nums[0] << ", " << missing_nums[1] << std::endl << std::endl;
}


struct Node2 {
    Node2* next_;
    Node2* rand_;
    int data_;
    
    Node2(int data) {
        next_ = nullptr;
        data_ = data;
        rand_ = nullptr;
    }
};

Node2* clone_list(Node2* head) {
    if (head == nullptr)
        return nullptr;
    Node2* new_head = new Node2(head->data_);
    Node2* node = new_head;
    Node2* curr = head->next_;
    
    // Copy the old list's next path
    while (curr != nullptr) {
        node->next_ = new Node2(curr->data_);
        node = node->next_;
        curr = curr->next_;
    }
    
    // Get the distances from head to the rand nodes
    std::vector<int> distances;
    for (Node2* node = head; node != nullptr; node = node->next_) {
        int dist = -1; // for null rand_ pointer
        curr = head;
        Node2* rand = node->rand_;
        if (rand == nullptr) {
            distances.push_back(dist);
            continue;
        }
        dist = 0; // Dist is no null, so init at 0 in case rand points to itself
        while (curr != rand) {
            dist++;
            curr = curr->next_;
        }
        distances.push_back(dist);
    }
    
    curr = new_head;
    while (curr != nullptr) {
        Node2* tmp = new_head;
        int dist = *distances.begin();
        distances.erase(distances.begin());
        if (dist == -1)
            curr->rand_ = nullptr;
        else {
            while (dist > 0) {
                tmp = tmp->next_;
                dist--;
            }
            curr->rand_ = tmp;
        }
    }
    return new_head;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::uniform_int_distribution<> dist(0,15);
    std::default_random_engine engine;
    int a1[3] = {1,3,5};
    int a4[5] = {4,2,1,6,5};
    int* a = new int[20];
    int a2[3] = {2,4,6};
    int** a3 = new int*[3];
    int Q10A[6] = {1,3,5,0,0,0};
    int Q10B[3] = {2,4,6};
    int Q11[6] = {1,2,-5,1,2,-1};
    for (int ii = 0; ii < 3; ii++)
        a3[ii] = new int[3];
    int count = 1;
    for (int ii = 0; ii < 3; ii++) {
        for (int jj = 0; jj < 3; jj++) {
            a3[ii][jj] = count++;
        }
    }
    for (int ii = 0; ii < 20; ii++)
        a[ii] = dist(engine);
    
    std::vector<std::tuple<int,double>> vec;
    vec.push_back(std::make_tuple<int, double>(1, 6));
    vec.push_back(std::make_tuple<int, double>(2, 10));
    vec.push_back(std::make_tuple<int, double>(3, 12));
    std::cout << median(a1, 3, a2, 3) << std::endl;
    std::cout << knapsack(vec, 5) << std::endl;
    std::cout << knapsack(vec, 4) << std::endl;
    std::cout << greatest_product(a3, 3, 3) << std::endl;
    a3[0][0] = -1;
    a3[1][2] = -6;
    std::cout << greatest_product(a3, 3, 3) << std::endl;
    for (int p : duplicates(new int[] {2,1,2,1}, 4)) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    std::vector<int> v(duplicates(new int[] {2,1}, 2));
    for (auto p : v)
        std::cout << p << " ";
    std::cout << std::endl;
    
    //q5
    for (int ii = 0; ii < 20; ii++)
        std::cout << a[ii] << " ";
    std::cout << std::endl << std::endl;
    std::vector<std::pair<std::size_t, std::vector<int>>> consec_vec = consecutive(a, 20);
    for (auto p : consec_vec) {
        std::vector<int> tmp_vec(std::get<1>(p));
        std::cout << std::get<0>(p) << ", [";
        if (tmp_vec.size() == 1)
            std::cout << tmp_vec[0] << "]";
        else {
            for (int ii = 0; ii < tmp_vec.size() - 1; ii++) {
                std::cout << tmp_vec[ii] << ", ";
            }
            std::cout << tmp_vec[tmp_vec.size() - 1] << "]";
        }
    }
    std::cout << std::endl << std::endl;
    
    //q5
    for (int ii = 0; ii < 5; ii++)
        std::cout << a4[ii] << " ";
    std::cout << std::endl << std::endl;
    std::vector<std::pair<std::size_t, std::vector<int>>> consec_vec2 = consecutive(a4, 5);
    for (auto p : consec_vec2) {
        std::vector<int> tmp_vec(std::get<1>(p));
        std::cout << std::get<0>(p) << ", [";
        if (tmp_vec.size() == 1)
            std::cout << tmp_vec[0] << "]";
        else {
            for (int ii = 0; ii < tmp_vec.size() - 1; ii++) {
                std::cout << tmp_vec[ii] << ", ";
            }
            std::cout << tmp_vec[tmp_vec.size() - 1] << "]";
        }
    }
    std::cout << std::endl << std::endl;
    
    
    // Q6
    bool mat[3][3] = {{true, false, false}, {false, false, false}, {false, false, false}};
    std::cout << "{" << std::endl;
    for (int ii = 0; ii < 3; ii++) {
        std::cout << "\t[";
        for (int jj = 0; jj < 3; jj++) {
            std::cout << mat[ii][jj] << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
    transform_mat(mat, 3, 3);
    std::cout << "{" << std::endl;
    for (int ii = 0; ii < 3; ii++) {
        std::cout << "\t[";
        for (int jj = 0; jj < 3; jj++) {
            std::cout << mat[ii][jj] << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
    
    
    bool** mat2 = new bool*[3];
    for (int ii = 0; ii < 3; ii++)
        mat2[ii] = new bool[4];
    for (int ii = 0; ii < 3; ii++) {
        for (int jj = 0; jj < 4; jj++) {
            mat2[ii][jj] = true;
        }
    }
    mat2[0][3] = false;
    mat2[2][2] = false;
    mat2[2][3] = false;
    
    std::cout << "{" << std::endl;
    for (int ii = 0; ii < 3; ii++) {
        std::cout << "\t[";
        for (int jj = 0; jj < 4; jj++) {
            std::cout << mat2[ii][jj] << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
    std::size_t maxv = largest_square_matrix(mat2, 3, 4);
    
    std::cout << maxv << ": {" << std::endl;
    for (int ii = 0; ii < 3; ii++) {
        std::cout << "\t[";
        for (int jj = 0; jj < 4; jj++) {
            std::cout << mat2[ii][jj] << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "}" << std::endl;
    
    std::vector<std::vector<int>> merge_vec;
    merge_vec.resize(3, std::vector<int>(3));
    int start = 0;
    
    merge_vec[0][0] = 1;
    merge_vec[0][1] = 4;
    merge_vec[0][2] = 7;
    merge_vec[1][0] = 2;
    merge_vec[1][1] = 5;
    merge_vec[1][2] = 8;
    merge_vec[2][0] = 3;
    merge_vec[2][1] = 6;
    merge_vec[2][2] = 9;
    
    
    for (auto p : merge_vec) {
        for (auto q : p)
            std::cout << q << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::vector<int> res_vec(merge(merge_vec));
    for (auto p : res_vec)
        std::cout << p << " ";
    std::cout << std::endl << std::endl;
    
    
    int** a5 = new int*[3];
    int cnt = 1;
    for (int ii = 0; ii < 3; ii++)
        a5[ii] = new int[3];
    for (int ii = 0; ii < 3;ii++)
        for (int jj = 0; jj < 3; jj++)
            a5[ii][jj] = cnt++;
    std::cout << contains(a5, 3, 3, 10) << std::endl;
    std::cout << contains(a5, 3, 3, 9) << std::endl;
    
    
    for (int p : Q10A) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    for (int p : Q10B) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    merge_arrays(&Q10A[0], 6, &Q10B[0], 3);
    for (int p : Q10A) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    
    for (int p : Q11) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    for (auto p : zero(Q11, 6)) {
        for (auto q : p)
            std::cout << q << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    int Q12[5] = {1,2,3, 4, 5};
    permutations(Q12, 5);
    std::cout << isAnagram("A", "B") << std::endl;
    std::cout << isAnagram("", "") << std::endl;
    std::cout << isAnagram("AB", "AB") << std::endl;
    std::cout << isAnagram("AB", "BA") << std::endl;
    
    std::vector<std::vector<int>> procs(5);
    procs[0] = std::vector<int>(0);
    procs[1] = std::vector<int>(1);
    procs[2] = std::vector<int>(1);
    procs[3] = std::vector<int>(2);
    procs[4] = std::vector<int>(1);
    
    procs[1][0] = 0;
    procs[2][0] = 1;
    procs[3][0] = 0;
    procs[3][1] = 1;
    procs[4][0] = 3;
    build_order(procs);
    
    
    std::vector<int> arr[5];
    arr[0].push_back(1);
    arr[1].push_back(4);
    arr[3].push_back(0);
    arr[3].push_back(2);
    arr[4].push_back(3);
    shortest_path(arr, 5, 1, 2);
    std::cout << std::endl << std::endl;
    
    //q28
    std::stack<int> s1;
    s1.push(1);
    s1.push(3);
    s1.push(2);
    s1.push(4);
    stack_sort(s1);
    while (!s1.empty()) {
        std::cout << s1.top() << " ";
        s1.pop();
    }
    std::cout << std::endl << std::endl;
    
    
    //Q32
    int arr32[3] = {2,4,3};
    int arr32_2[5] = {1,2,4,5,7};
    int arr32_3[5] = {1,2,4,5,6};
    missing(arr32, 3);
    missing (arr32_2, 5);
    missing (arr32_3, 5);
    
    
    
    return 0;
}
