#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include "vector.hpp"

namespace sjtu {
    
// In linked_hashmap, iteration ordering is differ from map, which is the order in which keys were inserted into the map.
// You should maintain a doubly-linked list running through all of its entries to keep the correct iteration order. 
// Note that insertion order is not affected if a key is re-inserted into the map.  
    
template<class Key, class T, class Hash = std::hash<Key>,  class Equal = std::equal_to<Key>>
class linked_hashmap {
public:
	using value_type = pair<Key, T>;
	using size_type = unsigned long long;
 private:
	class Node {
		friend class linked_hashmap;
	private:
		value_type kv_;
		linked_hashmap* id;

		Node* hashnext_;
		Node* prev_;
		Node* next_;
	public:
		Node() = default;

		Node(const value_type& value, linked_hashmap* i, Node* hnext = nullptr) :
			kv_(value), id(i), hashnext_(hnext), prev_(nullptr), next_(nullptr) { }

		Node(value_type&& value, linked_hashmap* i, Node* hnext = nullptr) :
			kv_(value), id(i), hashnext_(hnext), prev_(nullptr), next_(nullptr) { }

		~Node() { }
	};

public:
	class const_iterator;
	class iterator {
		friend class linked_hashmap;
	private:
		Node* node_;
	public:
        iterator() = default;
        iterator(Node* x) :node_(x) { }
        iterator(const iterator& other) :node_(other.node_) { }
        iterator(const const_iterator& other) :node_(other.node_) { }

		value_type& operator*() const { return node_->kv_; }
        value_type* operator->() const { return &(node_->kv_); }

		iterator& operator++() {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_) throw sjtu::invalid_iterator();
 			node_ = node_->next_;
            return *this;
        }

		iterator operator++(int) {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_) throw sjtu::invalid_iterator();
			iterator temp = *this;
 			node_ = node_->next_;
            return temp;
		}

		iterator & operator--() {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_->next_) throw sjtu::invalid_iterator();
 			node_ = node_->prev_;
            return *this;
		}

		iterator operator--(int) {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_->next_) throw sjtu::invalid_iterator();
			iterator temp = *this;
 			node_ = node_->prev_;
            return temp;
		}

		bool operator==(const iterator &rhs) const { return node_ == rhs.node_; }
		bool operator==(const const_iterator &rhs) const { return node_ == rhs.node_; }
		bool operator!=(const iterator &rhs) const { return node_ != rhs.node_; }
		bool operator!=(const const_iterator &rhs) const { return node_ != rhs.node_; }
	};
 
	class const_iterator {
		friend class linked_hashmap;
	private:
		Node* node_;
	public:
        const_iterator() = default;
        const_iterator(Node* x) :node_(x) { }
        const_iterator(const iterator& other) :node_(other.node_) { }
        const_iterator(const const_iterator& other) :node_(other.node_) { }

		const value_type& operator*() const { return node_->kv_; }
        const value_type* operator->() const { return &(node_->kv_); }

		const_iterator& operator++() {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_) throw sjtu::invalid_iterator();
 			node_ = node_->next_;
            return *this;
        }

		const_iterator operator++(int) {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_) throw sjtu::invalid_iterator();
			const_iterator temp = *this;
 			node_ = node_->next_;
            return temp;
		}

		const_iterator & operator--() {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_->next_) throw sjtu::invalid_iterator();
 			node_ = node_->prev_;
            return *this;
		}

		const_iterator operator--(int) {
            if (node_ == nullptr) throw sjtu::invalid_iterator();
            if (node_ == node_->id->end_->next_) throw sjtu::invalid_iterator();
			const_iterator temp = *this;
 			node_ = node_->prev_;
            return temp;
		}

		bool operator==(const iterator &rhs) const { return node_ == rhs.node_; }
		bool operator==(const const_iterator &rhs) const { return node_ == rhs.node_; }
		bool operator!=(const iterator &rhs) const { return node_ != rhs.node_; }
		bool operator!=(const const_iterator &rhs) const { return node_ != rhs.node_; }
	};
	
private:
	Node* end_;  // 指向list的尾节点 循环双向
	vector<Node*> table_;
	size_type size_;
	Hash hash;
	Equal equal;

	Node* _find(const Key& key) const {
		size_type h_index = hash(key) % table_.size();
		Node* pos = table_[h_index];
		while (pos) {
			if (equal(pos->kv_.first, key)) return pos;
			pos = pos->hashnext_;
		}
		return end_;
	}

	void _doubleSize() {
		vector<Node*> new_table(table_.size() * 2, nullptr);
		for (iterator it = begin(); it != end(); ++it) {
			Node* pos = it.node_;
			size_type h_index = hash(pos->kv_.first) % (table_.size() * 2);
			pos->hashnext_ = new_table[h_index];
			new_table[h_index] = pos;
		}
		table_ = new_table;
	}

	void _shrinkSize() {
		vector<Node*> new_table(table_.size() / 2, nullptr);
		for (iterator it = begin(); it != end(); ++it) {
			Node* pos = it.node_;
			size_type h_index = hash(pos->kv_.first) % (table_.size() / 2);
			pos->hashnext_ = new_table[h_index];
			new_table[h_index] = pos;
		}
		table_ = new_table;
	}

	Node* _insert(value_type value) {
		if (size_ == table_.size()) _doubleSize();
		
		++size_;
		size_type h_index = hash(value.first) % table_.size();
		Node* newnode = new Node(value, this, table_[h_index]);
		table_[h_index] = newnode;

		end_->prev_->next_ = newnode;
		newnode->prev_ = end_->prev_;
		newnode->next_ = end_;
		end_->prev_ = newnode;

		return newnode;
	}

	void _erase(Node* pos){
		--size_;
		pos->prev_->next_ = pos->next_;
		pos->next_->prev_ = pos->prev_;

		size_type h_index = hash(pos->kv_.first) % table_.size();

		Node* cur = table_[h_index];
		Node* pre = nullptr;
		while (cur) {
			if (equal(cur->kv_.first, pos->kv_.first)) {
				if (pre == nullptr) table_[h_index] = cur->hashnext_;
				else pre->hashnext_ = cur->hashnext_;
				delete cur;
				break;
			}
			pre = cur;
			cur = cur->hashnext_;
		}
		if(size_ < table_.size() / 4) _shrinkSize();
	}

	void _clear(){
		for (iterator it = begin(); it != end(); ) {
			Node* temp = it.node_;
			++it;
			delete temp;
		}
		size_ = 0;
		end_->next_ = end_->prev_ = end_;
		table_ = vector<Node*>(10, nullptr);
	}

public:
	linked_hashmap() :size_(0), table_(10, nullptr) {
		end_ = static_cast<Node*>(operator new(sizeof(Node)));
		end_->next_ = end_->prev_ = end_;
		end_->id = this;
	}

	linked_hashmap(const linked_hashmap &other):size_(0), table_(other.table_.size(), nullptr) {
		end_ = static_cast<Node*>(operator new(sizeof(Node)));
		end_->next_ = end_->prev_ = end_;
		end_->id = this;
		for (iterator it = other.begin(); it != other.end(); ++it)_insert(*it);
	}

	linked_hashmap & operator=(const linked_hashmap &other) {
		if(&other == this) return *this;
		for (iterator it = begin(); it != end(); ) {
			Node* temp = it.node_;
			++it;
			delete temp;
		}
		end_->next_ = end_->prev_ = end_;
		size_ = 0;
		table_ = vector<Node*>(other.table_.size(), nullptr);
		for (iterator it = other.begin(); it != other.end(); ++it)_insert(*it);
		return *this;
	}
 
	~linked_hashmap() {
		for (iterator it = begin(); it != end(); ) {
			Node* temp = it.node_;
			++it;
			delete temp;
		}
		size_ = 0;
		operator delete (end_);
	}

	T & at(const Key& key) {
		Node* pos = _find(key);
		if (pos != end_) return pos->kv_.second;
		throw index_out_of_bound();
	}

	const T & at(const Key &key) const {
		Node* pos = _find(key);
		if (pos != end_) return pos->kv_.second;
		throw index_out_of_bound();
	}
 
	T & operator[](const Key &key) {
		Node* pos = _find(key);
		if (pos != end_) return pos->kv_.second;
		return _insert(value_type(key, T()))->kv_.second;
	}
 
	const T & operator[](const Key &key) const {
		Node* pos = _find(key);
		if (pos != end_) return pos->kv_.second;
		throw index_out_of_bound();
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
 
	bool empty() const { return size_ == 0; }

	size_t size() const {return size_; }
 
	void clear() { _clear(); }
 
	pair<iterator, bool> insert(const value_type &value) {
		Node* temp = _find(value.first);
		if (temp != end_)return sjtu::pair<iterator, bool>(iterator(temp), false);
		return sjtu::pair<iterator, bool>(iterator(_insert(value)), true);
	}
 
	void erase(iterator pos) {
		if(pos.node_ == nullptr || pos.node_ == end_ || pos.node_->id != this) throw invalid_iterator();
		_erase(pos.node_);
	}

	size_t count(const Key &key) const { return _find(key) != end_ ? 1 : 0; }
	iterator find(const Key &key) { return iterator(_find(key)); }
	const_iterator find(const Key &key) const { return const_iterator(_find(key)); }
};

}

#endif
