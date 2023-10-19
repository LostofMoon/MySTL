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
    using value_type = T;
	using size_type = size_t;

private:
    struct Node {
        value_type value;
	    Node* prev_;  // 前一节点
	    Node* next_;  // 下一节点
        list* id;
        Node() = default;
	    Node(const T& v, list* i):value(v), id(i) { }
	    Node(T&& v, list* i):value(std::move(v)), id(i) { }
};

public:
    class const_iterator;
    class iterator {
	    friend class list;
    private:
	    Node* node_;
    public:
	    iterator(Node* x) :node_(x) { }
	    iterator(const iterator& rhs) :node_(rhs.node_) { }
	    iterator(const const_iterator& rhs) :node_(rhs.node_) { }

	    value_type& operator*() const {
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
			return node_->value; 
		}
	    value_type* operator->() const {  // 返回value的指针
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
			return &(node_->value); 
		}

	    iterator& operator++(){  // end_不允许++
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
		    node_ = node_->next_;
		    return *this;
	    }

	    iterator operator++(int){  // end_不允许++
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
		    iterator tmp = *this;
			node_ = node_->next_;
		    return tmp;
	    }

	    iterator& operator--(){  // 第一个节点不允许--
			if (node_ == nullptr || node_ == node_->id->end_->next_)throw sjtu::invalid_iterator();
		    node_ = node_->prev_;
		    return *this;
	    }

	    iterator operator--(int){  // 第一个节点不允许--
			if (node_ == nullptr || node_ == node_->id->end_->next_)throw sjtu::invalid_iterator();
		    iterator tmp = *this;
			node_ = node_->prev_;
		    return tmp;
	    }

	    // 重载比较操作符
	    bool operator==(const iterator& rhs) const { return node_ == rhs.node_; }
	    bool operator==(const const_iterator& rhs) const { return node_ == rhs.node_; }
	    bool operator!=(const iterator& rhs) const { return node_ != rhs.node_; }
	    bool operator!=(const const_iterator& rhs) const { return node_ != rhs.node_; }
    };

    class const_iterator {
	    friend class list;
    private:
	    Node* node_;
    public:
	    const_iterator(Node* x) :node_(x) { }
	    const_iterator(const iterator& rhs) :node_(rhs.node_) { }
	    const_iterator(const const_iterator& rhs) :node_(rhs.node_) { }

	    const value_type& operator*() const {
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
			return node_->value; 
		}
	    const value_type* operator->() const {
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
			return &(node_->value); 
		}

	    const_iterator& operator++(){  // end_不允许++
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
		    node_ = node_->next_;
		    return *this;
	    }

	    const_iterator operator++(int){  // end_不允许++
			if (node_ == nullptr || node_ == node_->id->end_)throw sjtu::invalid_iterator();
		    const_iterator tmp = *this;
			node_ = node_->next_;
		    return tmp;
	    }

	    const_iterator& operator--(){  // 第一个节点不允许--
			if (node_ == nullptr || node_ == node_->id->end_->next_)throw sjtu::invalid_iterator();
		    node_ = node_->prev;
		    return *this;
	    }
        
	    const_iterator operator--(int){  // 第一个节点不允许--
			if (node_ == nullptr || node_ == node_->id->end_->next_)throw sjtu::invalid_iterator();
		    const_iterator tmp = *this;
			node_ = node_->prev_;
		    return tmp;
	    }

	    // 重载比较操作符
	    bool operator==(const iterator& rhs) const { return node_ == rhs.node_; }
	    bool operator==(const const_iterator& rhs) const { return node_ == rhs.node_; }
	    bool operator!=(const iterator& rhs) const { return node_ != rhs.node_; }
	    bool operator!=(const const_iterator& rhs) const { return node_ != rhs.node_; }
    };

private:
	Node*  end_;  // 指向末尾节点
	size_type size_;  // 链表长度

    Node* _insert(Node* pos, value_type x) {
		++size_;
        Node* newnode = new Node(x, this);
		pos->prev_->next_ = newnode;
		newnode->prev_ = pos->prev_;
		newnode->next_ = pos;
		pos->prev_ = newnode;
		return newnode;
	}

    Node* _erase(Node *pos) {
        --size_;
        Node* temp = pos->next_;
		pos->prev_->next_ = pos->next_;
        pos->next_->prev_ = pos->prev_;
        delete pos;
        return temp;
    }

	Node* _erase(Node *first, Node *last) {
		if(first == last) return last;
		first->prev_->next_ = last;
		last->prev_ = first->prev_;

		while(first != last){
			auto temp = first->next_;
			delete first;
			first = temp;
		}
  		return last;
    }

    void _clear(){
        for (iterator it = begin(); it != end(); ) {
			Node* temp = it.node_;
			++it;
			delete temp;
		}
		size_ = 0;
        end_->next_ = end_->prev_ = end_;
    }

public:
    list():size_(0) {
  		end_ = static_cast<Node*>(operator new(sizeof(Node)));
        end_->id = this;
		end_->prev_ = end_->next_ = end_;
	}

    list(size_type n, const T& value):size_(0) {
  		end_ = static_cast<Node*>(operator new(sizeof(Node)));
        end_->id = this;
		end_->prev = end_->next = end_;
		for(auto i = 0; i < n; ++i)
            _insert(end_, value);
	}

    list(const list &other):size_(0) {
  		end_ = static_cast<Node*>(operator new(sizeof(Node)));
        end_->id = this;
		end_->prev_ = end_->next_ = end_;
        for(auto it = other.begin(); it != other.end(); ++it)
			_insert(end_, *it);
    }

    ~list() {
		for (iterator it = begin(); it != end(); ) {
			Node* temp = it.node_;
			++it;
			delete temp;
		}
		size_ = 0;
		operator delete (end_);
    }

    list &operator=(const list &rhs) {
        if(&rhs == this) return *this;
        auto lhs_it = begin();
		auto rhs_it = rhs.cbegin();
  		while(lhs_it != end() && rhs_it != rhs.cend()){
    		*lhs_it = *rhs_it;
			++lhs_it;
			++rhs_it;
  		}
  		if (rhs_it == rhs.end()){
    		erase(lhs_it, end());
  		}
  		else{
  		  	insert(end(), rhs_it, rhs.end());
  		}
    	return *this;
    }

public:
	// 迭代器相关操作
    iterator begin() {
		return end_->next_;
	}
	const_iterator begin() const {
		return end_->next_;
	}
	iterator end(){
		return end_;
	}
	const_iterator end() const {

		return end_;
	}
	const_iterator cbegin() const {
		return begin();
	}
	const_iterator cend() const {
		return end();
	}

	// 容量相关操作
	bool empty() const { return size_ == 0; }

	size_t size() const {return size_; }

    virtual void clear() { _clear(); }

    // 访问元素相关操作
  	value_type& front() { 
        if(size_ == 0) throw sjtu::container_is_empty();
    	return *begin();
  	}

  	const value_type& front() const {
        if(size_ == 0) throw sjtu::container_is_empty();
    	return *begin(); 
  	}

    value_type& back() { 
        if(size_ == 0) throw sjtu::container_is_empty();
    	return *(--end());
  	}

    const value_type& back()  const { 
        if(size_ == 0) throw sjtu::container_is_empty();
    	return *(--end());
  	}

	// insert erase push pop clear
    iterator insert(iterator pos, const T &value) {
        if(pos.node_ == nullptr || pos.node_->id != this) throw sjtu::invalid_iterator();
		return iterator(_insert(pos.node_, value));
    }

	iterator insert(iterator pos, size_type n, const T &value) {
        if(pos.node_ == nullptr || pos.node_->id != this) throw sjtu::invalid_iterator();
		iterator temp = _insert(pos.node_, value);
		for(auto i = 1; i != n; ++i)_insert(pos.node_, value);
    	return temp; 
    }

	iterator insert(iterator pos, iterator first, iterator last) {
        if(first.node_ == nullptr) throw sjtu::invalid_iterator();
        if(last.node_ == nullptr) throw sjtu::invalid_iterator();
		if (first == last)return pos;
		iterator temp = _insert(pos.node_, *first);
		++first;
		for(;first != last;++first)_insert(pos.node_, *first);
  		return temp;
    }

    iterator erase(iterator pos) {
		if(pos.node_ == nullptr || pos.node_ == end_ || pos.node_->id != this) throw sjtu::invalid_iterator();
		return iterator(_erase(pos.node_));
	}

	iterator erase(const_iterator first, const_iterator last){
		if(first.node_ == nullptr || first.node_->id != this) throw sjtu::invalid_iterator();
		if(last.node_ == nullptr || last.node_->id != this) throw sjtu::invalid_iterator();
		return iterator(_erase(first.node_, last.node_));
	}

    void push_back(const T &value) {
		_insert(end_, value);
	}

    void push_front(const T &value) {
		_insert(end_->next_, value);
	}

    void pop_back() {
		if(size_ == 0) throw sjtu::container_is_empty();
		_erase(end_->prev_);
	}

    void pop_front() {
		if(size_ == 0) throw sjtu::container_is_empty();
		_erase(end_->next_);
	}

    //sort()将链表内容排序（升序）
    void sort() {
		auto n = size();
		Node** temp = new Node* [n];
		iterator it = begin();
		for(auto i = 0; i != n; ++i){
			*(temp + i) = it.node_;
			++it;
		}

		std::sort(temp, temp + n, [](const Node* a, const Node* b){
			return a->value < b->value;
		});

		end_->next_ = temp[0];
		temp[0]->prev_ = end_;

		end_->prev_ = temp[n-1];
		temp[n-1]->next_ = end_;

		for(auto i = 0; i != n - 1; ++i){
			temp[i]->next_ = temp[i + 1];
			temp[i + 1]->prev_ = temp[i];
		}

		delete []temp;
	}

	//merge()和合并两个链表，保证两个链表升序，合并完之后要求升序，other要变成空列表，但列表仍存在
    void merge(list &other) {
		const_iterator lit = cbegin();
		const_iterator lend = cend();
		const_iterator rit = other.cbegin();
		const_iterator rend = other.cend();

		while (rit != rend) {
			if (lit == lend || *rit < *lit) {

				const_iterator temp = const_iterator(rit.node_->next_);

				rit.node_->prev_ = lit.node_->prev_;
				lit.node_->prev_->next_ = rit.node_;
				rit.node_->next_ = lit.node_;
				lit.node_->prev_ = rit.node_;

				rit = temp;
				++size_;
			}
			else ++lit;
		}
		
    	other.end_->next_ = other.end_->prev_ = other.end_;
    	other.size_ = 0;
	}
    
	//反转 : 超级帅的方法
    void reverse() {
		if (size_ <= 1) return;
  	 	iterator it = begin();
		auto e = end();
  	 	while (it != e) {
     		std::swap(it.node_->prev_, it.node_->next_);
			it = iterator(it.node_->prev_);  // 该节点反转了，应要向前移动 --it
  	 	}
  	 	std::swap(e.node_->prev_, e.node_->next_);
	}
    
	//unique()对于每个连续相同的数据，只保留第一个，删除其他相同数据
    void unique() {
		if (size_ <= 1) return;
		iterator cur = begin();
		iterator next = ++begin();
		iterator e = end();
		while (next != e) {
			if (*cur == *next)
				next = erase(next);
			else {
				++cur;
				++next;
			}
		}
	}
};

}

#endif //SJTU_LIST_HPP
