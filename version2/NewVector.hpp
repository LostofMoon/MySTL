#ifndef VECTOR_H
#define VECTOR_H

#include<utility>
#include<cstddef>
namespace sjtu {

template <class Tp>
void swap(Tp& lhs, Tp& rhs)
{
  auto tmp(std::move(lhs));
  lhs = std::move(rhs);
  rhs = std::move(tmp);
}

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
            *(begin_ + i) = value;
        }
    }

    vector(const vector& rhs):
        currentsize_(rhs.currentsize_), maxsize_(rhs.maxsize_) {
        begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
        for(size_type i = 0; i != rhs.size(); i++){
            *(begin_ + i) = *(rhs.begin_ + i);
        }
    }


    vector(vector&& rhs):
        currentsize_(rhs.currentsize_), maxsize_(rhs.maxsize_){
        begin_ = rhs.begin_;
        rhs.begin_ = nullptr;
    }

    //复制赋值
    vector& operator=(const vector& rhs) {
        if(this == &rhs)return *this;

        size_type len = rhs.size();
        if(size() < len){
            destroy();
            maxsize_ = rhs.capacity();
            currentsize_ = len;
            begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
            for(size_type i = 0; i!= len; ++i){
                *(begin_ + i) = *(rhs.begin_ + i);            
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

    ~vector(){
        destroy();
    }

public:

  // 迭代器相关操作
    iterator begin() {
    return begin_;
    }

    const_iterator begin() const {
        return begin_;
    }

    iterator end() {
        return begin_ + currentsize_;
    }

    const_iterator end() const {
        return begin_ + currentsize_;
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }

    // 容量相关操作
    bool empty() const {
        return currentsize_ == 0;
    }

    size_type size() const {
        return currentsize_;
    }

    size_type capacity() const {
        return maxsize_;
    }

    void reserve(size_type n){
        if(n <= capacity()) return;
        iterator temp_begin_ = static_cast<T*>(operator new(n * sizeof(T)));
        for(size_type i = 0; i != size(); i++){
            *(temp_begin_ + i) = *(begin_ + i);
            (begin_+i)->~T();
        }
        operator delete(begin_);
        begin_ = temp_begin_;
        maxsize_ = n;
    }

    void shrink_to_fit(){
        iterator temp_begin_ = static_cast<T*>(operator new(size() * sizeof(T)));
        for(size_type i = 0; i != size(); i++){
            *(temp_begin_ + i) = *(begin_ + i);
            (begin_+i)->~T();
        }
        operator delete(begin_);
        begin_ = temp_begin_;
        maxsize_ = size();
    }

    void assign(size_type n, const T& value){
        if(n > capacity()){
            iterator temp_begin_ = static_cast<T*>(operator new(n * sizeof(T)));
            for(size_type i = 0; i != n; i++){
                *(temp_begin_ + i) = value;
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
//用insert
    void resize(size_type n, const value_type& value){
        if(n > capacity()){
            iterator temp_begin_ = static_cast<T*>(operator new(n * sizeof(T)));
            for(size_type i = 0; i != size(); i++){
                *(temp_begin_ + i) = *(begin_ + i);
                (begin_+i)->~T();
            }
            for(size_type i = size(); i != n; i++){
                *(temp_begin_ + i) = value;
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
                *(begin_+i) = value;
            }
            currentsize_ = n;
        }
    }


    void resize(size_type n){
        resize(n, value_type());
    }

    
    value_type& operator[](size_type n) {
        //
        return *(begin_ + n);
    }

    const value_type& operator[](size_type n) const {
        //
        return *(begin_+ n);
    }

    value_type& at(size_type n) {
        //throw
        return *(begin_ + n);
    }

    const value_type& at(size_type n) const {
        //throw
        return *(begin_ + n);
    }

    value_type& front() {
        //if(empty) throw
        return *begin_;
    }

    const value_type& front() const {
        //if(empty) throw
        return *begin_;
    }
    value_type& back() {
        //if(empty) throw
        return *(begin_ + currentsize_ -1);
    }
    const value_type& back() const {
        //if(empty) throw
        return *(begin_ + currentsize_ -1);
    }
    
    value_type* data() {
        return begin_;
    }

    const value_type* data() const {
        return begin_;
    }

    iterator insert(const_iterator pos, size_type n, const value_type& value){
        //if(pos < begin_ || pos > begin_ + currentsize_)throw
        iterator xpos = const_cast<iterator>(pos);
        size_type index = xpos - begin_;
        if(currentsize_ + n <= maxsize_){
            for(size_type i = size() + n - 1; i != index + n - 1; --i){
                *(begin_ + i) = *(begin_ + i - n);
            }
            for(size_type i = index; i != index + n; ++i){
                *(begin_ + i) = value;
            }
            currentsize_ += n;
        }
        else{
            maxsize_ += n;
            iterator temp_begin_ = static_cast<T*>(operator new(maxsize_ * sizeof(T)));
            for(size_type i = size() + n - 1; i != index + n - 1; --i){
                *(temp_begin_ + i) = *(begin_ + i - n);
                (begin_ + i - n)->~T();
            }

            for(size_type i = index; i != index + n; ++i){
                *(temp_begin_ + i) = value;
            }

            for(size_type i = 0; i != index; ++i){
                *(temp_begin_ + i) = *(begin_ + i);
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

    void push_back(const T& x) {
        insert(end(), x);
    }
    void push_back(T&& x) {
        insert(end(), const_cast<const value_type &>(x));
    }
    void pop_back(){
        if(currentsize_ == 0)return;
        end()->~T();
        --currentsize_;
    }

    void clear(){
        for (size_type i = 0; i != currentsize_; i++)
            (begin_+i)->~T();
        currentsize_ = 0;
    }

    iterator erase(const_iterator first, const_iterator last){
        //if(!(first >= cbegin() && last <= cend() && first <= last))throw
        if(first == last) return end();
        iterator xfirst = const_cast<iterator>(first);//可以吗
        iterator xlast = const_cast<iterator>(last);//可以吗
        size_type delta = xfirst - cbegin();
        size_type n = last - first;
        for(size_type i = 0; i != size() - n - delta; ++i){
            *(xfirst + i + 1) = *(xlast + i + 1);
        }
        for(size_type i = size() - n; i != size(); ++i){
            (begin_ + i)->~T();
        }
        currentsize_ -= n;

        return begin_ + delta + 1;
    }

    iterator erase(const_iterator pos){
        return erase(pos - 1, pos);
    }

    void swap(vector& rhs) {
        if (this != &rhs) {
            sjtu::swap(begin_, rhs.begin_);
            sjtu::swap(currentsize_, rhs.currentsize_);
            sjtu::swap(maxsize_, rhs.maxsize_);
        }
    }

    };

}
#endif // !VECTOR_H