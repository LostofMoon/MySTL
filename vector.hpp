#ifndef VECTOR_H
#define VECTOR_H

#include "exceptions.hpp"

#include<utility>
#include<cstddef>

namespace sjtu {

template <class T>
class vector{

public:

    using value_type = T;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using size_type = size_t;

private:

    iterator begin_;
    size_type currentsize_ = 0;
    size_type maxsize_;

    void destroy(){
        for (size_type i = 0; i != currentsize_; i++)
            (begin_+i)->~T();
        operator delete(begin_);
        begin_ = nullptr;
        maxsize_ = 0;
        currentsize_ = 0;
    }

public:

#pragma region default constructor, copy constructor, Destructor

    vector():maxsize_(10) {
        begin_ = static_cast<T*>(operator new(10 * sizeof(T)));
    }

    vector(size_type n){
        maxsize_ = n > 10 ? n : 10;
        begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
    }

    vector(size_type n, const value_type& value){
        maxsize_ = n > 10 ? n : 10;
        currentsize_ = n;
        begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
        for(size_type i = 0; i != n; i++){
            new (begin_ + i) value_type(value);
        }
    }

    vector(const vector& rhs):
        currentsize_(rhs.currentsize_), maxsize_(rhs.maxsize_) {
        begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
        for(size_type i = 0; i != rhs.size(); i++){
            new (begin_ + i) value_type(*(rhs.begin_ + i));
        }
    }

    vector(vector&& rhs):
        currentsize_(rhs.currentsize_), maxsize_(rhs.maxsize_){
        begin_ = rhs.begin_;
        rhs.begin_ = nullptr;
    }

    ~vector(){
        destroy();
    }
#pragma endregion
    
#pragma region Assignment operator

    vector& operator=(const vector& rhs) {
        if(this == &rhs)return *this;

        size_type len = rhs.size();
        if(size() < len){
            destroy();
            maxsize_ = rhs.capacity();
            currentsize_ = len;
            begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
            for(size_type i = 0; i!= len; ++i){
                new (begin_ + i) value_type(*(rhs.begin_ + i));   
            }
        }
        else{
            currentsize_ = len;
            for(size_type i = 0; i!= len; ++i){
                *(begin_ + i) = *(rhs.begin_ + i);
            }
            for(size_type i = len; i!= size(); ++i){
                (begin_ + i)->~T();
            }
        }
        return *this;
    }

    vector& operator=(vector&& rhs) {
        destroy();
        maxsize_ = rhs.maxsize_;
        currentsize_ = rhs.currentsize_;
        begin_ = rhs.begin_;
        rhs.begin_ = nullptr;
        return *this;
    }
#pragma endregion

public:

#pragma region 迭代器相关操作

    iterator begin() {
    return begin_;
    }

    const_iterator begin() const {  // const的vector调用这个
        return begin_;
    }

    iterator end() {
        return begin_ + currentsize_;
    }

    const_iterator end() const {  // const的vector调用这个
        return begin_ + currentsize_;
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }
#pragma endregion

#pragma region 容量相关操作 empty(), size(), capacity(), clear()

    bool empty() const {
        return currentsize_ == 0;
    }

    size_type size() const {
        return currentsize_;
    }

    size_type capacity() const {
        return maxsize_;
    }

    void clear(){
        for (size_type i = 0; i != currentsize_; i++)
            (begin_+i)->~T();
        currentsize_ = 0;
    }
#pragma endregion

    //扩容
    void reserve(size_type n){
        if(n <= capacity()) return;
        iterator temp_begin_ = static_cast<T*>(operator new(n * sizeof(T)));
        for(size_type i = 0; i != size(); i++){
            new (temp_begin_ + i) value_type(*(begin_ + i));
            (begin_+i)->~T();
        }
        operator delete(begin_);
        begin_ = temp_begin_;
        maxsize_ = n;
    }

    //缩容到数据数量
    void shrink_to_fit(){
        iterator temp_begin_ = static_cast<T*>(operator new(size() * sizeof(T)));
        for(size_type i = 0; i != size(); i++){
            new (temp_begin_ + i) value_type(*(begin_ + i));
            (begin_+i)->~T();
        }
        operator delete(begin_);
        begin_ = temp_begin_;
        maxsize_ = size();
    }

    //扩容并填充数据
    void assign(size_type n, const T& value){
        if(n > capacity()){
            iterator temp_begin_ = static_cast<T*>(operator new(n * sizeof(T)));
            for(size_type i = 0; i != n; i++){
                new (temp_begin_ + i) value_type(value);
            }
            for(size_type i = 0; i != size(); i++){
                (begin_+i)->~T();
            }
            operator delete(begin_);
            begin_ = temp_begin_;
            maxsize_ = size();
            currentsize_ = size();
        }
    }

    //改变容量，多了则填充数据
    void resize(size_type n, const value_type& value){
        if(n > capacity()){
            iterator temp_begin_ = static_cast<T*>(operator new(n * sizeof(T)));
            for(size_type i = 0; i != size(); i++){
                new (temp_begin_ + i) value_type(*(begin_ + i));
                (begin_+i)->~T();
            }
            for(size_type i = size(); i != n; i++){
                new (temp_begin_ + i) value_type(value);
            }
            operator delete(begin_);
            begin_ = temp_begin_;
            maxsize_ = n;
            currentsize_ = n;
        }
        else if(n < size()){
            for(size_type i = n; i != size(); i++){
                (begin_+i)->~T();
            }
            currentsize_ = n;
        }
        else {
            for(size_type i = size(); i != n; i++){
                new (begin_ + i) value_type(value);
            }
            currentsize_ = n;
        }
    }

    //改变容量, 多了不填
    void resize(size_type n){
        resize(n, value_type());
    }

 #pragma region 访问元素

    value_type& operator[](size_type n) {
        if(n >= size()) throw index_out_of_bound();//正常STL不需要
        return *(begin_ + n);
    }

    const value_type& operator[](size_type n) const {
        if(n >= size()) throw index_out_of_bound();//正常STL不需要
        return *(begin_+ n);
    }

    value_type& at(size_type n) {
        if(n >= size()) throw index_out_of_bound();//越界
        return *(begin_ + n);
    }

    const value_type& at(size_type n) const {
        if(n >= size()) throw index_out_of_bound();//越界
        return *(begin_ + n);
    }

    value_type& front() {
        if(size() == 0) throw container_is_empty();//空
        return *begin_;
    }

    const value_type& front() const {
        if(size() == 0) throw container_is_empty();//空
        return *begin_;
    }
    value_type& back() {
        if(size() == 0) throw container_is_empty();//空
        return *(begin_ + currentsize_ -1);
    }
    const value_type& back() const {
        if(size() == 0) throw container_is_empty();//空
        return *(begin_ + currentsize_ -1);
    }

    value_type* data() {
        return begin_;
    }

    const value_type* data() const {
        return begin_;
    }
#pragma endregion

 #pragma region 插入 insert()
    iterator insert(const_iterator pos, size_type n, const value_type& value){
        //if(pos < begin_ || pos > begin_ + currentsize_)throw
        iterator xpos = const_cast<iterator>(pos);//留后门
        size_type index = xpos - begin_;
        if(currentsize_ + n <= maxsize_){
            for(size_type i = size() + n - 1; i != index + n - 1; --i){
                new (begin_ + i) value_type(*(begin_ + i - n));
                (begin_ + i - n)->~T();
            }
            for(size_type i = index; i != index + n; ++i){
                new (begin_ + i) value_type(value);
            }
            currentsize_ += n;
        }
        else{
            maxsize_ += maxsize_ > n ? maxsize_ : n * 2;
            iterator temp_begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
            for(size_type i = size() + n - 1; i != index + n - 1; --i){
                new (temp_begin_ + i) value_type(*(begin_ + i - n));
                (begin_ + i - n)->~T();
            }

            for(size_type i = index; i != index + n; ++i){
                new (temp_begin_ + i) value_type(value);
            }

            for(size_type i = 0; i != index; ++i){
                new (temp_begin_ + i) value_type(*(begin_ + i));
                (begin_+i)->~T();
            }
            operator delete(begin_);
            begin_ = temp_begin_;
            currentsize_ += n;
        }
        return begin_ + index + 1;
    }

    iterator insert(const_iterator pos, const value_type& x) {
        return insert(pos, 1, x);
    }

    iterator insert(const_iterator pos, T&& x) {
        return insert(pos, 1, const_cast<const value_type &>(x));
    }
#pragma endregion

#pragma region push(), pop()
    void push_back(const T& x) {
        insert(end(), x);
    }
    void push_back(T&& x) {
        insert(end(), const_cast<const value_type &>(x));
    }
    void pop_back(){
        if(size() == 0) throw container_is_empty();//空
        if(currentsize_ == 0)return;
        (end() - 1)->~T();
        --currentsize_;
    }
#pragma endregion

#pragma region 删除 erase()
    iterator erase(const_iterator first, const_iterator last){
        //if(!(first >= cbegin() && last <= cend() && first <= last))throw
        if(first == last) return end();
        iterator xfirst = const_cast<iterator>(first);//留后门
        iterator xlast = const_cast<iterator>(last);//留后门
        size_type delta = xfirst - cbegin();
        size_type n = last - first;
        for(size_type i = 0; i != size() - n - delta; ++i){
            *(xfirst + i + 1) = *(xlast + i + 1);
        }
        for(size_type i = size() - n; i != size(); ++i){
            (begin_ + i)->~T();
        }
        currentsize_ -= n;
        if(size() < capacity()/4){
            maxsize_ = currentsize_ * 2;
            iterator temp_begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
            for(size_type i = 0; i != size(); ++i){
                new (temp_begin_ + i) value_type(*(begin_ + i));
                (begin_ + i)->~T();
            }
            operator delete(begin_);
            begin_ = temp_begin_;
        }
        return begin_ + delta + 1;
    }

    iterator erase(const_iterator pos){
        return erase(pos - 1, pos);
    }
#pragma endregion

    void swap(vector& rhs) {
        if (this != &rhs) {
            std::swap(begin_, rhs.begin_);
            std::swap(currentsize_, rhs.currentsize_);
            std::swap(maxsize_, rhs.maxsize_);
        }
    }

    };

}
#endif // !VECTOR_H