namespace zys {

template<class T>
class vector {
public:
    // types
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = size_t;
    
    class iterator {};
    class const_iterator{};
 
private:
    value_type* data_;
    size_t size_;


public:
    // construct/copy/destroy
    vector(){
        size_ = 10;
        data_ = new value_type [size_];
    }
    vector(const vector& x){
        size_ =x.size_;
        data_ = new value_type [size_];
        for(int i = 0; i != size_; i++)
            *(data_+i) = *(x.data_ + i);
 
    }
    vector(vector&& x) noexcept{
        size_ =x.size_;
        data_ = x.data_;
        x.data_ = nullptr;
    }

    ~vector(){
        delete []data_;
    }

    vector& operator=(const vector& x){
        size_ =x.size_;
        data_ = new value_type [size_];
        for(int i = 0; i != size_; i++)
            *(data_+i) = *(x.data_ + i);
    }

    vector& operator=(vector&& x) noexcept(){
        size_ =x.size_;
        data_ = x.data_;
        x.data_ = nullptr;
    }

    void assign(size__type n, const T& u){
        size = n;
        data_ = new value_type [size_];
        for(int i = 0; i != size_; i++)
            *(data_+i) = u;
    }
 
    // iterators
    iterator               begin() noexcept;
    const_iterator         begin() const noexcept;
    iterator               end() noexcept;
    const_iterator         end() const noexcept;
 
    const_iterator         cbegin() const noexcept;
    const_iterator         cend() const noexcept;
 
    // capacity
    [[nodiscard]] bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    void      resize(size_type sz);
    void      resize(size_type sz, const T& c);
    void      reserve(size_type n);
    void      shrink_to_fit();
 
    // element access
    reference       operator[](size_type n);
    const_reference operator[](size_type n) const;
    const_reference at(size_type n) const;
    reference       at(size_type n);
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;
 
    // data access
    T*       data() noexcept;
    const T* data() const noexcept;
 
    // modifiers
    void push_back(const T& x);
    void push_back(T&& x);
    void pop_back();
 
    iterator insert(const_iterator position, const T& x);
    iterator insert(const_iterator position, T&& x);
    iterator insert(const_iterator position, size_type n, const T& x);
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void     swap(vector&);
    void     clear() noexcept;
};

}
