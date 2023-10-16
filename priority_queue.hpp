#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
#include "vector.hpp"

namespace sjtu {

template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
    // types
    using value_type = T;
    using size_type = unsigned long long;

private:
	vector<T> array_;
	Compare comp_;

T& get(size_type x) {
	return array_[x-1];
}

const T& get(size_type x) const {
	return array_[x-1];
}

public:

	//constructor and deconstructor
	priority_queue():array_() {
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
		return (get(1));
	}

	void push(const T &e) {
		array_.push_back(e);
		size_type pos = array_.size();
		while(pos != 1){
			if(comp_(get(pos/2), get(pos))){
				std::swap(get(pos/2), get(pos));
				pos /= 2;
			}
			else break;
		}
	}
	
	void pop() {
		if (array_.empty())throw container_is_empty();
		std::swap(get(1), get(array_.size()));
		array_.pop_back();

		size_type size = array_.size();
		size_type pos = 1, child_pos = pos * 2;
		while(child_pos <= size){
			if(child_pos + 1 <= size && comp_(get(child_pos), get(child_pos + 1) ))child_pos++;
			if (comp_(get(pos), get(child_pos) )){
				std::swap(get(pos), get(child_pos));
				pos = child_pos;
				child_pos = pos * 2;
			}
			else break;
		}
	}

	size_t size() const {
		return array_.size();
	}
	
	bool empty() const {
		return array_.empty();
	}
};

}

#endif