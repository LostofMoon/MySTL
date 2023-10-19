#ifndef LIST_H
#define LIST_H


#include<utility>
#include<cstddef>
#include<algorithm>
namespace sjtu {

//声明数据节点和基本节点
template <class T> struct list_node_base;//基本节点
template <class T> struct list_node;//数据节点
template <class T> struct list_iterator;
template <class T> struct list_const_iterator;

//-----------------------------------------list_node_base-----------------------------------------------------

template <class T>
struct list_node_base {
	using base_ptr = list_node_base<T>*;
	using node_ptr = list_node<T>*;

	base_ptr prev;  // 前一节点
	base_ptr next;  // 下一节点

	list_node_base() = default;

	node_ptr as_node() {
		return static_cast<node_ptr>(this);
	}

	void unlink(){
		prev = next = this;
	}

};

//-----------------------------------------list_node----------------------------------------------------------

template <class T>
struct list_node : public list_node_base<T> {
	using base_ptr = list_node_base<T>*;
	using node_ptr = list_node<T>*;

	T value;  // 数据域

	list_node() = default;
	list_node(const T& v) :value(v) { }
	list_node(T&& v) :value(std::move(v)) { }

	base_ptr as_base() {
		return static_cast<base_ptr>(this);
	}
};

//-----------------------------------------list_iterator------------------------------------------------------

template <class T>
struct list_iterator {
	using value_type = T;
	using base_ptr = list_node_base<T>*;
	using node_ptr = list_node<T>*;

	base_ptr node_;  // 指向当前节点

	// 重载操作符
	list_iterator() = default;
	list_iterator(base_ptr x) :node_(x) { }
	list_iterator(node_ptr x) :node_(x->as_base()) { }
	list_iterator(const list_iterator& rhs) :node_(rhs.node_) { }
	list_iterator(const list_const_iterator<T>& rhs) :node_(rhs.node_) { }

  	// 重载操作符
	value_type& operator*()  const { return node_->as_node()->value; }
	value_type* operator->() const { return &(operator*()); }  // 返回value的指针

	list_iterator<T>& operator++(){
		//MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}
	list_iterator<T> operator++(int){
		list_iterator<T> tmp = *this;
		++*this;
		return tmp;
	}
	list_iterator<T>& operator--(){
		//MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}
	list_iterator<T> operator--(int){
		list_iterator<T> tmp = *this;
		--*this;
		return tmp;
	}

	// 重载比较操作符
	bool operator==(const list_iterator<T>& rhs) const { return node_ == rhs.node_; }
	bool operator!=(const list_iterator<T>& rhs) const { return node_ != rhs.node_; }

	//奇怪的东西
	bool operator==(list_const_iterator<T>& rhs) const { return node_ == rhs.node_; }
	bool operator!=(list_const_iterator<T>& rhs) const { return node_ != rhs.node_; }
};


//-----------------------------------------list_const_iterator-----------------------------------------------------

template <class T>
struct list_const_iterator {//有啥区别
	using value_type = T;
	using base_ptr = list_node_base<T>*;
	using node_ptr = list_node<T>*;

	base_ptr node_;

	list_const_iterator() = default;
	list_const_iterator(base_ptr x) :node_(x) {}
	list_const_iterator(node_ptr x) :node_(x->as_base()) {}
	list_const_iterator(const list_iterator<T>& rhs) :node_(rhs.node_) {}
	list_const_iterator(const list_const_iterator& rhs) :node_(rhs.node_) {}

	const value_type& operator*()  const { return node_->as_node()->value; }
	const value_type* operator->() const { return &(operator*()); }

	list_const_iterator<T>& operator++() {
		//MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}
	list_const_iterator<T> operator++(int) {
		list_const_iterator<T> tmp = *this;
		++*this;
		return tmp;
	}
	list_const_iterator<T>& operator--() {
		//MYSTL_DEBUG(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}
	list_const_iterator<T> operator--(int) {
		list_const_iterator<T> tmp = *this;
		--*this;
		return tmp;
	}

	// 重载比较操作符
	bool operator==(const list_const_iterator<T>& rhs) const { return node_ == rhs.node_; }
	bool operator!=(const list_const_iterator<T>& rhs) const { return node_ != rhs.node_; }
};



template <class T>
class list {
public:
	using value_type = T;
	using base_ptr = list_node_base<T>*;
	using node_ptr = list_node<T>*;
	using size_type = size_t;

	using iterator = list_iterator<T>;
	using const_iterator = list_const_iterator<T>;

private:
	base_ptr  node_;  // 指向末尾节点
	size_type size_;  // 链表长度

public:

	//声明
	/*
	iterator insert(const_iterator pos, const value_type& value);
	iterator insert(const_iterator pos, size_type n, const value_type& value);
	iterator insert(const_iterator pos, const_iterator first, const_iterator last);

	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	*/

	// 构造、复制、移动、析构函数
	list(){
  		node_ = static_cast<list_node_base<T>*>(operator new(sizeof(list_node_base<T>)));
		node_->unlink();
  		size_ = 0;
	}
	
	list(size_type n, const T& value){
		node_ = static_cast<list_node_base<T>*>(operator new(sizeof(list_node_base<T>)));
		node_->unlink();
  		size_ = n;
		for(auto i = 0; i < n; ++i){
			node_ptr p = static_cast<list_node<T>*>(operator new(sizeof(list_node<T>)));
			new (&(p->value)) value_type(value);
			p->next = node_;
			p->prev = node_->prev;
			node_->prev->next = p;
			node_->prev = p;

		}
	}

	list(const list& rhs){
		node_ = static_cast<list_node_base<T>*>(operator new(sizeof(list_node_base<T>)));
		node_->unlink();
  		size_ = rhs.size();

		for(auto it = rhs.cbegin(); it != rhs.cend(); ++it){
			node_ptr p = static_cast<list_node<T>*>(operator new(sizeof(list_node<T>)));
			new (&(p->value)) value_type(*it);
			p->next = node_;
			p->prev = node_->prev;
			node_->prev->next = p;
			node_->prev = p;
		}
	}

  	list(list&& rhs):node_(rhs.node_), size_(rhs.size()){
    	rhs.node_ = nullptr;
    	rhs.size_ = 0;
  	}

  	list& operator=(const list& rhs){
    	if (this == &rhs)return *this;
		//内部成员没改
		auto lhs_it = begin();
		auto rhs_it = rhs.cbegin();//细节用cbegin()
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

	~list(){
		if (node_){
    		clear();
			operator delete(node_);
    		node_ = nullptr;
    		size_ = 0;
		}
	}

public:
	// 迭代器相关操作
	iterator begin() {
		return node_->next;
	}
	const_iterator begin() const {
		return node_->next;
	}
	iterator end(){
		return node_;
	}
	const_iterator end() const {

		return node_;
	}
	const_iterator cbegin() const {
		return begin();
	}
	const_iterator cend() const {
		return end();
	}

	// 访问元素相关操作
  	value_type& front() { 
    	//MYSTL_DEBUG(!empty());
    	return *begin();
  	}

  	const value_type& front() const {  // 细节
    	//MYSTL_DEBUG(!empty()); 
    	return *begin(); 
  	}

    value_type& back() { 
    	//MYSTL_DEBUG(!empty());
    	return *(--end());
  	}

    const value_type& back()  const { 
    	//MYSTL_DEBUG(!empty()); 
    	return *(--end());
  	}

	// 容量相关操作
    bool empty() const {
		return node_->next == node_;
	}

  	size_type size() const {
		return size_;
	}

  	size_type max_size() const {
		return static_cast<size_type>(-1);
	
	}

	// insert erase push pop clear

	iterator insert(const_iterator pos, const value_type& value) {
    	//THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");

		node_ptr p = new list_node<T>(value);
		//node_ptr p = static_cast<list_node<T>*>(operator new(sizeof(list_node<T>)));
		//new (&(p->value)) value_type(value);
		
		p->next = pos.node_;
		p->prev = pos.node_->prev;
		pos.node_->prev->next = p;
		pos.node_->prev = p;
    	++size_;
    	return iterator(p);
  	}

  	iterator insert(const_iterator pos, size_type n, const value_type& value) { 
    	//THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
		iterator temp = insert(pos, value);
		for(auto i = 1; i != n; ++i){
			insert(pos, value);
		}
    	return temp; 
  	}

	iterator insert(const_iterator pos, const_iterator first, const_iterator last){
   	 	//THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
		if (first == last)return last;
		iterator temp = insert(pos, *first);
		++first;
		while(first != last){
			insert(pos, *first);
			++first;
		}
  		return temp;
  	}

  	void push_front(const value_type& value) {
    	//THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		insert(node_->next, value);
  	}

  	void push_back(const value_type& value) {
    	//THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
		insert(node_, value);
	}

	iterator erase(const_iterator pos){
		//MYSTL_DEBUG(pos != cend());
		base_ptr n = pos.node_;
  		base_ptr next = n->next;

		n->prev->next = n->next;
		n->next->prev = n->prev;
  		--size_;

		delete n->as_node();//ssssssssssssssssssssssssssssssssssssssssssssss

  		return iterator(next);
	}
  	iterator erase(const_iterator first, const_iterator last){
		if (first == last)return last;
		base_ptr ft = first.node_;
		base_ptr lt = last.node_;

		ft->prev->next = lt;
		lt->prev = ft->prev;

		while(ft != lt){
			auto temp = ft->next;
			ft->as_node()->value.~value_type();
			operator delete(ft->as_node());
			ft = temp;
		}
  		return last;
	}

  	void pop_front() {
    	//MYSTL_DEBUG(!empty());
    	erase(iterator(node_->next));
  	}

  	void pop_back() { 
    	//MYSTL_DEBUG(!empty());
    	erase(iterator(node_->prev));
  	}

  	void clear(){
    	base_ptr cur = node_->next;
    	for (base_ptr next = cur->next; cur != node_; cur = next, next = cur->next){
			cur->as_node()->value.~value_type();
			operator delete(cur->as_node());
    	}
    	node_->unlink();
    	size_ = 0;
  	}

	//merge()和合并两个链表，保证两个链表升序，合并完之后要求升序，other要变成空列表，但列表仍存在
	void merge(list &other) {
		const_iterator lit = cbegin();
		const_iterator lend = cend();
		const_iterator rit = other.cbegin();
		const_iterator rend = other.cend();

		while (rit != rend) {
			if (lit == lend || *rit < *lit) {

				const_iterator temp = const_iterator(rit.node_->next);

				rit.node_->prev = lit.node_->prev;
				lit.node_->prev->next = rit.node_;
				rit.node_->next = lit.node_;
				lit.node_->prev = rit.node_;

				rit = temp;
				++size_;
			}
			else ++lit;
		}
		
    	other.node_->unlink();
    	other.size_ = 0;
	}

	//反转 : 超级帅的方法
	void reverse() {
  		if (size_ <= 1) return;
  	 	iterator it = begin();
		auto e = end();
  	 	while (it != e) {
     		std::swap(it.node_->prev, it.node_->next);
			it = iterator(it.node_->prev);//该节点反转了，应要向前移动 --it
  	 	}
  	 	std::swap(e.node_->prev, e.node_->next);
	}
	
	//sort()将链表内容排序（升序）
	void sort() {
		auto n = size();
		base_ptr *temp = new base_ptr [n];
		//value_type **temp = new value_type* [n];
		iterator it = begin();
		for(auto i = 0; i != n; ++i){
			*(temp + i) = it.node_;
			++it;
		}

		std::sort(temp, temp + n, [](const base_ptr & a, const base_ptr & b){
			return a->as_node()->value < b->as_node()->value;
		});

		node_->next = temp[0];
		temp[0]->prev = node_;

		node_->prev = temp[n-1];
		temp[n-1]->next = node_;

		for(auto i = 0; i != n - 1; ++i){
			temp[i]->next = temp[i + 1];
			temp[i + 1]->prev = temp[i];
		}

		delete []temp;
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
#endif //LIST_H