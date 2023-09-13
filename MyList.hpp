#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {

template<typename T>
class list {
public:
    // types
    using value_type = T;
    using size_type = unsigned long long;
    //statement 
    class iterator;
    class const_iterator;

protected:
    class node {
    public:
        node *pre, *next;
        value_type *data;
        list *ID;

        node():pre(nullptr), next(nullptr), data(nullptr), ID(nullptr) {
        }

        node(value_type x, node* p = nullptr,  node* q = nullptr):data(new value_type(x) ),pre(p),next(q), ID(nullptr) {
        }

        node &operator=(const node &other) {
            pre = other.pre;
            next = other.next;
            data = other.data;
            ID = other.ID;
        }

        ~node() {
            delete data;
        }
    };

protected:
    node *head, *tail;
    size_type currentsize;

    node *insert(node *pos, node *cur) {
        cur->ID = pos->ID;
        node* pre = pos->pre;

        cur->pre = pre;
        pre->next = cur;

        cur->next = pos;
        pos->pre = cur;

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
//-------------------------------iterator------------------------------------------
    class iterator {
        friend list;
        friend list::const_iterator;
    private:
        node* data_;
    public:
        //build and destroy
        iterator():data_(nullptr) {
        }
        
        iterator(node* x):data_(x) {
        }

        iterator(const iterator& x):data_(x.data_) {
        }

        iterator(const_iterator& x):data_(x.data_) {
        }

        iterator(iterator&& x):data_(x.data_) {
            x.data_ = nullptr;
        }

        //operator
        iterator & operator++() {
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->next;
            if(data_->next == nullptr) throw invalid_iterator();
            return *this;
        }
        
        iterator & operator--() {
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->pre;
            if(data_->pre == nullptr) throw invalid_iterator();
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->next;
            if(data_->next == nullptr) throw invalid_iterator();
            return temp;
        }
        
        iterator operator--(int) {
            iterator temp = *this;
            if(data_ == nullptr) throw invalid_iterator();
            data_ = data_->pre;
            if(data_->pre == nullptr) throw invalid_iterator();
            return temp;
        }

        value_type & operator *() const {
            if(data_->data == nullptr) throw invalid_iterator();
            return *(data_->data);
        }
        
        value_type * operator ->() const noexcept {
            if(data_->data == nullptr) throw invalid_iterator();
            return data_->data;
        }

        bool operator==(const iterator &rhs) const {
            return data_ == rhs.data_;
        }
        bool operator==(const const_iterator &rhs) const {
            return data_ == rhs.data_;
        }

        bool operator!=(const iterator &rhs) const {
            return data_ != rhs.data_;
        }

        bool operator!=(const const_iterator &rhs) const {
            return data_ != rhs.data_;
        }
    };

//-------------------------------const_iterator------------------------------------------

    class const_iterator {
        friend list;
        friend list::iterator;
    private:
        node const * data_;
    public:
        //build and destroy
        const_iterator(const node* x):data_(x) {
        }

        const_iterator(const iterator & x):data_(x.data_) {
        }

        const_iterator(const_iterator & x):data_(x.data_) {
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
            if(data_ == nullptr) throw invalid_iterator();
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
            return *(data_->data);
        }
        
        value_type * operator ->() const noexcept {
            if(data_ == nullptr) throw invalid_iterator();
            return data_->data;
        }

        bool operator==(const iterator &rhs) const {
            return data_ == rhs.data_;
        }
        bool operator==(const const_iterator &rhs) const {
            return data_ == rhs.data_;
        }

        bool operator!=(const iterator &rhs) const {
            return data_ != rhs.data_;
        }

        bool operator!=(const const_iterator &rhs) const {
            return data_ != rhs.data_;
        }
    };

//--------------------------------------------------------------------------------------------------

    //build and destroy
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->pre = head;
        head->ID = this;
        tail->ID = this;
        currentsize = 0;
    }

    list(const list &other):list() {
        node* add = other.head->next;
        while(add != other.tail){
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
        head->ID = this;
        tail->ID = this;

        node* add = other.head->next;
        while(add != other.tail){
            insert(tail, new node(*(add->data)) );
            add = add->next;
        }
        currentsize = other.currentsize;
        return *this;
    }
    
    const value_type & front() const {
        if(empty())throw container_is_empty();
        return *(head->next->data);
    }

    const value_type & back() const {
        if(empty())throw container_is_empty();
        return *(tail->pre->data);
    }
    
    //iterator
    iterator begin() {
        return iterator(head->next);
    }

    const_iterator cbegin() const {
        return const_iterator(head->next);
    }

    iterator end() {
        return iterator(tail);
    }

    const_iterator cend() const {
        return const_iterator(tail);
    }
    
    //function
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
        currentsize++;
    }

    void push_front(const T &value) {
        insert(head->next, new node(value) );
        currentsize++;
    }

    void pop_back() {
        if(currentsize == 0)throw container_is_empty();
        erase(tail->pre);
        currentsize--;
    }
    
    void pop_front() {
        if(currentsize == 0)throw container_is_empty();
        erase(head->next);
        currentsize--;
    }

    virtual iterator insert(iterator pos, const T &value) {
        if(pos.data_->ID != this) throw invalid_iterator();
        if(pos.data_ == nullptr) throw index_out_of_bound();
        currentsize++;
        return insert(pos.data_, new node(value) );
    }

    virtual iterator erase(iterator pos) {
        if(pos.data_ == nullptr) throw index_out_of_bound();
        if(pos.data_->data == nullptr) throw index_out_of_bound();
        currentsize--;
        return erase(pos.data_);
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {}

    void merge(list &other) {
        node *index = head->next, *pos = other.head->next;
        while(pos != other.tail){
            if(index->data == nullptr || *(pos->data) < *(index->data)){
                insert(index, *(pos->data));
                pos = pos->next;
            }
            else index = index->next;
        }
    }

    void reverse() {
        list temp;

    }
    void unique() {
        if(currentsize < 2) return;
        node *now = head->next, *then = now->next;
        while(then != tail){
            if(*(now->data) == *(then->data)){
                then = erase(then);
                currentsize--;
            }
            else{
                then = then->next;
                now = now->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
