namespace zys {

template<class T>
class vector {
public:
    // types
    using value_type = T;
    using size_type = size_t;

//-------------------------------iterator----------------------------------------
    class iterator {
    private:
        value_type* data_; 

    public:
    //build and destory
        iterator(){
            data_ = new value_type;
            data_ = nullptr;
        }

        iterator(value_type* x){
            data_ = new value_type(x);
        }

        iterator(iterator& x){
            data_ = new value_type(*(x.data_));
        }

        iterator(iterator&& x){
            data_ = x.data_;
            x.data_ = nullptr;
        }

        ~iterator(){
            delete data_;
        }

    //operator
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
            iterator temp;(*this);
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
    //build and destory
        const_iterator(value_type* x){
            data_ = new value_type const;
            data_ = x;

        }
        const_iterator(cosnt value_type* x){
            data_ = new value_type const;
            data_ = x;
        }

        const_iterator(iterator& x){
            data_ = new value_type const (*(x.data_));
        }

        const_iterator(const_iterator& x){
            data_ = new value_type(*(x.data_));
        }

        const_iterator(const_iterator&& x){
            data_ = x.data_;
            x.data_ = nullptr;
        }

        ~const_iterator(){
            delete data_;
        }

    //operator
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

//--------------------------------------------------------------------------------------

private:
    value_type* data_;
    size_t currentsize_, maxsize_;

    void DoubleSize() {
        value_type* temp = new value_type[maxsize_*2];
        for(i = 0; i != maxsize; i++)
            *(temp + i) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        maxsize_ *= 2;
    }

public:
    // construct/copy/destroy
    vector(){
        currentsize_ = 0;
        maxsize_ = 10;
        data_ = new value_type [maxsize_];
    }

    vector(const vector& x) {
        currentsize_ = x.currentsize_;
        maxsize_ = x.maxsize_;
        data_ = new value_type [maxsize_];
        for(int i = 0; i != currentsize_; i++)
            *(data_+i) = *(x.data_ + i);
    }

    vector(vector&& x) noexcept {
        currentsize_ = x.currentsize_;
        maxsize_ = x.maxsize_;
        data_ = x.data_;
        x.data_ = nullptr;
    }

    ~vector(){
        delete [] data_;
    }

    vector& operator=(const vector& x) {
        clear();
        currentsize_ = x.currentsize_;
        maxsize_ = x.maxsize_;
        data_ = new value_type [maxsize_];
        for(int i = 0; i != currentsize_; i++)
            *(data_+i) = *(x.data_ + i);
    }

    vector& operator=(vector&& x) noexcept {
        clear();
        currentsize_ = x.currentsize_;
        maxsize_ = x.maxsize_;
        data_ = x.data_;
        x.data_ = nullptr;
    }

    void assign(size__type n, const T& u){
        clear();
        maxsize_ = n*2;
        currentsize_ = n;
        data_ = new value_type [maxsize_];
        for(int i = 0; i != currentsize_; i++)
            *(data_+i) = u;
    }
 
    // iterators
    iterator begin() noexcept{
        iterator temp(data_);
        return temp;
    }

    iterator end() noexcept {
        iterator temp(data_+currentsize_-1);
        return temp;
    }

    const iterator begin() const noexcept {
        const iterator temp(data_);
        return temp;
    }

    const iterator end() const noexcept {
        const iterator temp(data_+currentsize_-1);
        return temp;
    }

    const_iterator cbegin() const noexcept {
        const_iterator temp(data_);
        return temp;
    }
    const_iterator cend() const noexcept {
        const_iterator temp(data_+currentsize_-1);
        return temp;
    }

    // capacity
    [[nodiscard]] bool empty() const noexcept{
        return (currentsize_ == 0);
    }

    size_type size() const noexcept {
        return currentsize_;
    }

    size_type capacity() const noexcept {
        return maxsize_;
    }

    void resize(size_type sz){
        value_type* temp = new value_type[sz];
        for(int i = 0; i!= sz && i!= currentsize_; i++)
            *(temp + i) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        if(currentsize_ > sz)currentsize_ = sz;
        maxsize_ = sz;
    }

    void resize(size_type sz, const value_type& c){
        value_type* temp = new value_type[sz];
        for(int i = 0; i!= sz && i!= currentsize_; i++)
            *(temp + i) = *(data_ + i);
        for(int i = currentsize_ + 1; i < sz; i++)
            *(temp + i) = c;
    }

    void reserve(size_type n){
        value_type* temp = new value_type[n];
        for(int i = 0; i!= n && i!= currentsize_; i++)
            *(temp + i) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        maxsize_ = n;
    }
    void shrink_to_fit(){
        reserve(currentsize_);
    }
 
    // element access
    value_type& operator[](size_type n) {
        return *(data_ + n);
    }

    const value_type& operator[](size_type n) const {
        return *(data_ + n);
    }

    value_type& at(size_type n) {
        return *(data_ + n);
    }

    const value_type& at(size_type n) const {
        return *(data_ + n);
    }

    value_type& front() {
        return *data_;
    }

    const value_type& front() const {
        return *data_;
    }
    value_type& back() {
        return *(data_ + currentsize_ -1);
    }
    const value_type& back() const {
        return *(data_ + currentsize_ -1);
    }

    // data access
    value_type* data() noexcept {
        return data_;
    }
    const value_type* data() const noexcept {
        return data_;
    }
 
    // modifiers
    void push_back(const T& x) {
        if(currentsize_ == maxsize_)DoubleSize();
        currentsize_ += 1;
        *(data_ + currentsize_) = x;
    }
    void push_back(T&& x) {
        if(currentsize_ == maxsize_)DoubleSize();
        currentsize_ += 1;
        *(data_ + currentsize_) = x;
    }
    void pop_back(){
        if(currentsize_ == 0)return;
        currentsize_ -= 1;
    }
 
    iterator insert(const_iterator position, const T& x) {
        if(currentsize_ == maxsize_)DoubleSize();
        value_type* temp = new value_type[maxsize_];
        for(int i = 0; i != position; i++)
            *(temp + i) = *(data_ + i);
        *(temp + position) = x;
        for(int i = position; i != currentsize_; i++)
            *(temp + i + 1) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        currentsize_ += 1;
    }
    iterator insert(const_iterator position, T&& x) {
        if(currentsize_ == maxsize_)DoubleSize();
        value_type* temp = new value_type[maxsize_];
        for(int i = 0; i != position; i++)
            *(temp + i) = *(data_ + i);
        *(temp + position) = x;
        for(int i = position; i != currentsize_; i++)
            *(temp + i + 1) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        currentsize_ += 1;
    }

    iterator insert(const_iterator position, size_type n, const T& x) {
        while(currentsize_ + n > maxsize_)DoubleSize();
        value_type* temp = new value_type[maxsize_];
        for(int i = 0; i != position; i++)
            *(temp + i) = *(data_ + i);
        for(int i = 0; i != n; i++)
            *(temp + position + i) = x;
        for(int i = position; i != currentsize_; i++)
            *(temp + i + n) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        currentsize_ += n;
    }

    iterator erase(const_iterator position) {
        value_type* temp = new value_type[maxsize_];
        for(int i= 0; i != position; i++)
            *(temp + i) = *(data_ + i);
        for(int i= position + 1; i != currentsize_; i++)
            *(temp + i) = *(data_ + i);
        delete [] data_;
        data_ = temp;
        currentsize_ -= 1;
    }

    iterator erase(const_iterator first, const_iterator last) {
        value_type* temp = new value_type[maxsize_];
        for(const_iterator i = cbegin; i != first + 1; i++)
            *(temp + i - cbegin) = *(data_ + i - cbegin);
        for(const_iterator i = last + 1; i != cend; i++)
            *(temp + i - cbegin - (last - first)) = *(data_ + i- - cbegin);
        delete [] data_;
        data_ = temp;
        currentsize_ -= (last - first);
    }

    void swap(vector& x) {
        vector temp(x);
        x = *this;
        maxsize_ = temp.maxsize_;
        currentsize_ = temp.currentsize_;
        data_ = temp.data_;
    }

    void clear() noexcept {
        delete [] data_;
        currentsize_ = 0;
        maxsize_ = 10;
    }
};

}