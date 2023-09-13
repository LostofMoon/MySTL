#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
#include "MyVector.hpp"

using namespace zys;

namespace sjtu {

template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
    // types
    using value_type = T;
    using size_type = unsigned long long;

private:
	vector<T*> array_;
	Compare comp_;

public:

	//constructor and deconstructor
	priority_queue():array_() {
		array_.push_back(nullptr);
	}

	priority_queue(const priority_queue &other):array_(other.array_) {
	}

	~priority_queue() {
	}

	//operator
	priority_queue &operator=(const priority_queue &other) {
		if(this == &other)return *this;
		array_ = other.array_;
		return *this;
	}

	const T & top() const {
		if (empty()) throw container_is_empty();
		return (*(array_[1]));
	}

	void push(const T &e) {
		array_.push_back(new T(e));
		size_type pos = array_.size() -1;
		while(pos != 1){
			if(comp_(*(array_[pos/2]),*(array_[pos]) )){
				T* temp = array_[pos];
				array_[pos] = array_[pos/2];
				array_[pos/2] = temp;
				pos /=2;
			}
			else break;
		}
	}
	
	void pop() {
		if (array_.size() == 1)throw container_is_empty();
		size_type size = array_.size() -1;
		T* temp = array_[1];
		array_[1] = array_[size];
		array_[size] = temp;
		size--;
		array_.pop_back();

		size_type pos = 1, child_pos = pos * 2;
		while(child_pos <= size){
			if(child_pos + 1 <= size && comp_(*(array_[child_pos]), *(array_[child_pos + 1]) ))child_pos++;
			if (comp_(*(array_[pos]), *(array_[child_pos]) )){
				T* temp = array_[child_pos];
				array_[child_pos] = array_[pos];
				array_[pos] = temp;
				pos = child_pos;
				child_pos = pos * 2;
			}
			else break;
		}
	}

	size_t size() const {
		return array_.size() -1;
	}
	
	bool empty() const {
		return array_.size() == 1;
	}
};

}

#endif