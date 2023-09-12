namespace zys {

template<class T>
class vector {
public:
    // types
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = size_t;
    

//-------------------------------iterator----------------------------------------
    class iterator {
    private:
        value_type* data_; 

    public:
        iterator(){
            data_ = new value_type;
            data_ = nullptr;
        }

        iterator(iterator& x){
            data_ = new value_type(*(x.data_));
        }

        iterator(iterator&& x){
            data_ = x.data_;
            x.data_ = nullptr;
        }

        value_type& operator*() {
            return *data_;
        }

        value_type* operator->() {
            return data_;
        }

        iterator& operator++() {
            ++data_;
            return *this;
        }

        iterator& operator--() {
            --data_;
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp(*this);
            ++data_;
            return temp;
        }

        iterator operator--(int) {
            iterator temp;
            temp.data_ = (*this).data_;
            --data_;
            return temp;
        }

        iterator operator+(long long offset) {
            iterator temp;
            temp.data_ = (*this).data_ + offset;
            return temp;
        }

        iterator operator-(long long offset) {
            iterator temp;
            temp.data_ = (*this).data_ - offset;
            return temp;
        }
        
        friend long long operator-(const iterator& lit, const iterator& rit) {
            return lit.data_ - rit.data_;
        }

        iterator& operator+=(long long offset) {
            data_ += offset;
            return *this;
        }

        iterator& operator-=(long long offset) {
            data_ -= offset;
            return *this;
        }
        
        bool operator==(const iterator& rit) {
            return data_ == rit.data_;
        }

        bool operator!=(const iterator& rhs) {
            return data_ != rit.data_;
        }
    };

//-------------------------------const_iterator----------------------------------------

    class const_iterator {
    private:
        value_type* const data_; 

    public:
        const_iterator(const_iterator& x){
            data_ = new value_type(*(x.data_));
        }

        const_iterator(iterator& x){
            data_ = new value_type(*(x.data_));
        }

        iterator(const_iterator&& x){
            data_ = x.data_;
            x.data_ = nullptr;
        }

        const value_type& operator*() {
            return *data_;
        }

        const value_type* operator->() {
            return data_;
        }

        const_iterator& operator++() {
            ++data_;
            return *this;
        }

        const_iterator& operator--() {
            --data_;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator temp(*this);
            ++data_;
            return temp;
        }

        const_iterator operator--(int) {
            const_iterator temp(*this);
            --data_;
            return temp;
        }

        const_iterator operator+(long long offset) {
            const_iterator temp;
            temp.data_ = (*this).data_ + offset;
            return temp;
        }
        
        const_iterator operator-(long long offset) {
            const_iterator temp;
            temp.data_ = (*this).data_ - offset;
            return temp;
        }

        friend long long operator-(const_iterator& lit, const_iterator& rit) {
            return lit.data_ - rit.data_;
        }

        const_iterator& operator+=(long long offset) {
            data_ += offset;
            return *this;
        }

        const_iterator& operator-=(long long offset) {
            data_ -= offset;
            return *this;
        }

        bool operator==(const_iterator& rit) {
            return data_ == rit.data_;
        }

        bool operator!=(const_iterator& rhs) {
            return data_ != rit.data_;
        }
    };

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
        delete []data_;
        size_ =x.size_;
        data_ = new value_type [size_];
        for(int i = 0; i != size_; i++)
            *(data_+i) = *(x.data_ + i);
    }

    vector& operator=(vector&& x) noexcept(){
        delete []data_;
        size_ =x.size_;
        data_ = x.data_;
        x.data_ = nullptr;
    }

    void assign(size__type n, const T& u){
        delete []data_;
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