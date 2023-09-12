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
 
    value_type* p;
    size_t size;

    // construct/copy/destroy
    vector(){
        size = 10;
        p = new value_type [size];
    }
    vector(const vector& x){
        size =x.size;
        p = new value_type [size];
        for(int i = 0; i != size; i++)
            *(p+i) = *(x.p + i);
 
    }
    vector(vector&& x) noexcept{
        size =x.size;
        p = x.p;
        x.p = nullptr;
    }

    ~vector(){
        delete []p;
    }

    vector& operator=(const vector& x){=
        size =x.size;
        p = new value_type [size];
        for(int i = 0; i != size; i++)
            *(p+i) = *(x.p + i);
    }

    vector& operator=(vector&& x) noexcept(){
        size =x.size;
        p = x.p;
        x.p = nullptr;
    }

    void assign(size_type n, const T& u):size(n){
        p = new value_type [size];
        for(int i = 0; i != size; i++)
            *(p+i) = u;
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
