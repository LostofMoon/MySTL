#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
public:
    // types
    using value_type = T;
    using size_type = unsigned long long;

protected:
    class node {
    public:
        node *pre, *next;
        value_type *data;

        node():pre(nullptr),next(nullptr),data(nullptr){
        }

        node(value_type x, node* p = nullptr,  node* q = nullptr):data(new value_type(x) ),pre(p),next(q){
        }

    };

protected:
    node* head, tail;
    size_type currentsize;

    node *insert(node *pos, node *cur) {
        node* pre = cur->pre;

        pos->pre = pre;
        pre->next = pos;
        pos->next = cur;
        cur->pre = pos;

        return cur;
    }

    node *erase(node *pos) {
        node *pre = pos->pre, *next = pos->next;
        pre->next = next;
        next->pre = pre;
        delete pos;

        return next;
    }

public:
    //statement
    class const_iterator;

//-------------------------------iterator------------------------------------------
    class iterator {
        friend list::const_iterator;
    private:
        node* data_;
    public:
        //build and destroy
        iterator():data_(nullptr) {
        }
        
        iterator(const value_type* x):data_(x) {
        }

        iterator(const iterator& x):data_(x.data_) {
        }

        iterator(const const_iterator& x):data_(x.data_) {
        }

        iterator(iterator&& x):data_(x.data_) {
            x.data_ = nullptr;
        }

        //operator
        iterator & operator++() {
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->next;
            if(data_ == nullptr) throw invalid_iterator();
            return *this;
        }
        
        iterator & operator--() {
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->pre;
            if(data_ == nullptr) throw invalid_iterator();
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->next;
            if(data_ == nullptr) throw invalid_iterator();
            return temp;
        }
        
        iterator operator--(int) {
            iterator temp = *this;
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->pre;
            if(data_ == nullptr) throw invalid_iterator();
            return temp;
        }

        value_type & operator *() const {
            if(data_ == nullptr) throw invalid_iterator();
            return *data_;
        }
        
        value_type * operator ->() const noexcept {
            if(data_ == nullptr) throw invalid_iterator();
            return data_;
        }

        bool operator==(const iterator &rhs) const {
            return data_ == rhs->data_;
        }
        bool operator==(const const_iterator &rhs) const {
            return data_ == rhs->data_;
        }

        bool operator!=(const iterator &rhs) const {
            return data_ != rhs->data_;
        }

        bool operator!=(const const_iterator &rhs) const {
            return data_ != rhs->data_;
        }
    };

//-------------------------------const_iterator------------------------------------------

    class const_iterator {
    friend list::iterator;
    private:
        node const * data_;
    public:
        //build and destroy
        const_iterator(const value_type const * x):data_(x) {
        }

        const_iterator(const iterator const & x):data_(x.data_) {
        }

        const_iterator(const const_iterator & x):data_(x.data_) {
        }

        const_iterator(iterator&& x):data_(x.data_) {
            x.data_ = nullptr;
        }

        //operator
        const_iterator & operator++() {
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->next;
            if(data_ == nullptr) throw invalid_iterator();
            return *this;
        }
        
        const_iterator & operator--() {
            if(tdata_ == nullptr) throw invalid_iterator();
            data_ = data_->pre;
            if(data_ == nullptr) throw invalid_iterator();
            return *this;
        }

        const_iterator operator++(int) {
            iterator temp = *this;
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->next;
            if(data_ == nullptr) throw invalid_iterator();
            return temp;
        }
        
        const_iterator operator--(int) {
            const_iterator temp = *this;
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->pre;
            if(data_ == nullptr) throw invalid_iterator();
            return temp;
        }

        value_type & operator *() const {
            if(data_ == nullptr) throw invalid_iterator();
            return *data_;
        }
        
        value_type * operator ->() const noexcept {
            if(data_ == nullptr) throw invalid_iterator();
            return data_;
        }

        bool operator==(const iterator &rhs) const {
            return data_ == rhs->data_;
        }
        bool operator==(const const_iterator &rhs) const {
            return data_ == rhs->data_;
        }

        bool operator!=(const iterator &rhs) const {
            return data_ != rhs->data_;
        }

        bool operator!=(const const_iterator &rhs) const {
            return data_ != rhs->data_;
        }
    };

//--------------------------------------------------------------------------------------------------

    //build and destroy
    list() {
        head = new node;
        tail = new node;
        head->next = tail;
        tail->pre = head;
        currentsize = 0;
    }

    list(const list &other):list() {
        node* add = other.head->next;
        while(add != other->tail){
            insert(tail, new node(*(add->data)) );
            add = add->next;
        }
        currentsize = other.currentsize;
    }

    virtual ~list() {
        node* p = head;
        while(p){
            node* temp = p->next;
            delete p;
            p = temp;
        }
    }

    //operator
    list &operator=(const list &other) {
        head = new node;
        tail = new node;
        head->next = tail;
        tail->pre = head;

        node* add = other.head->next;
        while(add != other->tail){
            insert(tail, new node(*(add->data)) );
            add = add->next;
        }
        currentsize = other.currentsize;
    }
    
    const value_type & front() const {
        if(empty())throw container_is_empty();
        return head->next->data;
    }

    const value_type & back() const {
        if(empty())throw container_is_empty();
        return tail->pre->data;
    }
     
    iterator begin() {
        return iterator(head->next);
    }

    const_iterator cbegin() const {
        return const_terator(head->next);
    }

    iterator end() {
        return const_terator(tail);
    }

    const_iterator cend() const {
        return const_terator(tail);
    }
    
    virtual bool empty() const {
        return (currentsize == 0);
    }
    
    virtual size_t size() const {
        return currentsize;
    }

    virtual void clear() {
        node* p = head;
        while(p){
            node* temp = p->next;
            delete p;
            p = temp;
        }

        head = new node;
        tail = new node;
        head->next = tail;
        tail->pre = head;
        currentsize = 0;
    }
    
    void push_back(const value_type &value) {
        insert(tail, new node(value) );
    }

    void push_front(const T &value) {
        insert(head->next, new node(value) );
    }

    void pop_back() {
        erase(tail->pre);
    }
    
    void pop_front() {
        erase(head->next);
    }

    virtual iterator insert(iterator pos, const T &value) {
        insert(pos, new node(value) );
    }

    virtual iterator erase(iterator pos) {
        erase(pos.data_);
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {}
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {}
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {}
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {}
};

}

#endif //SJTU_LIST_HPP
