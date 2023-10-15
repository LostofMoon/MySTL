/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

 // only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<class Key, class T, class Compare = std::less<Key>>
    class map {
    public:
        using value_type = pair<const Key, T>;
        using key_type = Key;
        using mapped_type = T;
        using size_type = size_t;
        static const bool RED = true, BLACK = false;

    private:
        struct RBNode {
            RBNode* left;
            RBNode* right;
            RBNode* parent;
            value_type value;
            bool col;
            map* id;

            RBNode() = default;
            RBNode(const value_type& x, map* i, RBNode* p = nullptr, bool color = RED, RBNode* l = nullptr, RBNode* r = nullptr)
                :value(x), id(i), parent(p), col(color), left(l), right(r) { }

            RBNode(value_type&& x, map* i, RBNode* p = nullptr, bool color = RED, RBNode* l = nullptr, RBNode* r = nullptr)
                :value(x), id(i), parent(p), col(color), left(l), right(r) { }

            ~RBNode() { }

            void leftRotate() {
                if (parent == nullptr) return;
                RBNode* fa = parent;
                RBNode* gf = fa->parent;
                fa->right = left;
                if (left) left->parent = fa;
                left = fa;
                fa->parent = this;

                parent = gf;
                if (gf == nullptr) return;
                if (gf->left == fa) gf->left = this;
                else gf->right = this;
            }

            void rightRotate() {
                if (parent == nullptr) return;
                RBNode* fa = parent;
                RBNode* gf = fa->parent;
                fa->left = right;
                if (right) right->parent = fa;
                right = fa;
                fa->parent = this;

                parent = gf;
                if (gf == nullptr) return;
                if (gf->left == fa)
                    gf->left = this;
                else gf->right = this;
            }

            RBNode* next() {  // 直接后继
                RBNode* pos = this;
                if (pos->right) {
                    pos = pos->right;
                    while (pos->left) pos = pos->left;
                    return pos;
                }
                else {
                    RBNode* fa = pos->parent;
                    while (fa->right == pos) {
                        pos = fa;
                        fa = fa->parent;
                    }
                    return fa;
                }
            }

            RBNode* prev() {  // 直接前驱
                RBNode* pos = this;
                if (pos->left) {
                    pos = pos->left;
                    while (pos->right) pos = pos->right;
                    return pos;
                }
                else {
                    RBNode* fa = pos->parent;
                    while (fa->left == pos) {
                        pos = fa;
                        fa = fa->parent;
                    }
                    return fa;
                }

            }

            RBNode* sucleft() {  // 找到左侧最大
                RBNode* pos = left;
                while (pos->right) {
                    pos = pos->right;
                }
                return pos;
            }

            RBNode* sucright() {  // 找到右侧最小
                RBNode* pos = right;
                while (pos->left) {
                    pos = pos->left;
                }
                return pos;
            }
        };

    private:
        size_type size_;
        Compare comp_;
        RBNode* root_;
        RBNode* leftmost_;
        RBNode* rightmost_;
        RBNode* end_;

    public:
        struct const_iterator;
        struct iterator {
            RBNode* node_;

            iterator() = default;
            iterator(RBNode* x) :node_(x) { }
            iterator(const iterator& other) :node_(other.node_) { }
            iterator(const const_iterator& other) :node_(other.node_) { }

            value_type& operator*() const { return node_->value; }
            value_type* operator->() const { return &(node_->value); }

            iterator& operator++() {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->end_) throw sjtu::invalid_iterator();

                if (node_ == node_->id->rightmost_) node_ = node_->id->end_;
                else node_ = node_->next();
                return *this;
            }

            iterator operator++(int) {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->end_) throw sjtu::invalid_iterator();

                iterator temp = *this;
                if (node_ == node_->id->rightmost_) node_ = node_->id->end_;
                else node_ = node_->next();
                return temp;
            }
            iterator& operator--() {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->leftmost_) throw sjtu::invalid_iterator();

                if (node_ == node_->id->end_) node_ = node_->id->rightmost_;
                else node_ = node_->prev();
                return *this;
            }
            iterator operator--(int) {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->leftmost_) throw sjtu::invalid_iterator();

                iterator temp = *this;
                if (node_ == node_->id->end_) node_ = node_->id->rightmost_;
                else node_ = node_->prev();
                return temp;
            }

            bool operator==(const iterator& rhs) const { return node_ == rhs.node_; }
            bool operator!=(const iterator& rhs) const { return node_ != rhs.node_; }
            bool operator==(const const_iterator& rhs) const { return node_ == rhs.node_; }
            bool operator!=(const const_iterator& rhs) const { return node_ != rhs.node_; }
        };

        struct const_iterator {
            RBNode* node_;

            const_iterator() = default;
            const_iterator(RBNode* x) :node_(x) { }
            const_iterator(const iterator& other) :node_(other.node_) { }
            const_iterator(const const_iterator& other) :node_(other.node_) { }

            const value_type& operator*() const { return node_->value; }
            const value_type* operator->() const { return &(node_->value); }

            const_iterator& operator++() {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->end_) throw sjtu::invalid_iterator();

                if (node_ == node_->id->rightmost_) node_ = node_->id->end_;
                else node_ = node_->next();
                return *this;
            }

            const_iterator operator++(int) {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->end_) throw sjtu::invalid_iterator();

                const_iterator temp = *this;
                if (node_ == node_->id->rightmost_) node_ = node_->id->end_;
                else node_ = node_->next();
                return temp;
            }
            const_iterator& operator--() {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->leftmost_) throw sjtu::invalid_iterator();

                if (node_ == node_->id->end_) node_ = node_->id->rightmost_;
                else node_ = node_->prev();
                return *this;
            }
            const_iterator operator--(int) {
                if (node_ == nullptr) throw sjtu::invalid_iterator();
                if (node_ == node_->id->leftmost_) throw sjtu::invalid_iterator();

                const_iterator temp = *this;
                if (node_ == node_->id->end_) node_ = node_->id->rightmost_;
                else node_ = node_->prev();
                return temp;
            }

            bool operator==(const iterator& rhs) const { return node_ == rhs.node_; }
            bool operator!=(const iterator& rhs) const { return node_ != rhs.node_; }
            bool operator==(const const_iterator& rhs) const { return node_ == rhs.node_; }
            bool operator!=(const const_iterator& rhs) const { return node_ != rhs.node_; }
        };

    private:

        void _solveDoubleRed(RBNode* pos) {
            if (pos == root_) {
                pos->col = BLACK;
                return;
            }
            // 注意 不会出现fa为root_节点为RED
            RBNode* fa = pos->parent;
            RBNode* gf = fa->parent;
            RBNode* unc;
            if (gf->left == fa) unc = gf->right;
            else unc = gf->left;

            if (unc && unc->col == RED) {
                fa->col = BLACK;
                unc->col = BLACK;
                gf->col = RED;
                if (gf == root_ || gf->parent->col == RED)_solveDoubleRed(gf);
                return;
            }
            if (fa->left == pos && gf->left == fa) {
                fa->rightRotate();
                fa->col = BLACK;
                gf->col = RED;
                if (gf == root_) root_ = fa;
            }
            else if (fa->right == pos && gf->right == fa) {
                fa->leftRotate();
                fa->col = BLACK;
                gf->col = RED;
                if (gf == root_) root_ = fa;
            }
            else if (fa->right == pos && gf->left == fa) {
                pos->leftRotate();
                pos->rightRotate();
                pos->col = BLACK;
                gf->col = RED;
                if (gf == root_) root_ = pos;
            }
            else {
                pos->rightRotate();
                pos->leftRotate();
                pos->col = BLACK;
                gf->col = RED;
                if (gf == root_) root_ = pos;
            }
        }

        void _solveRemoveBlack(RBNode* pos) {
            if (pos == root_)return;
            // 初始化参数
            RBNode* fa = pos->parent;
            RBNode* bro = fa->left;
            bool posisleft = false;
            if (fa->left == pos) {
                bro = fa->right;
                posisleft = true;
            }

            if (posisleft) {
                //情况一：兄弟节点为RED => 创造兄弟节点为BLACK
                if (bro->col == RED) {
                    bro->leftRotate();
                    bro->col = BLACK;
                    fa->col = RED;
                    if (fa == root_) root_ = bro;
                    bro = fa->right;
                }

                //情况二：兄弟节点为黑，子节点有红
                if (bro->right && bro->right->col == RED) {
                    bro->col = fa->col;
                    bro->right->col = BLACK;
                    fa->col = BLACK;
                    bro->leftRotate();
                    if (fa == root_) root_ = bro;
                }
                else if (bro->left && bro->left->col == RED) {
                    RBNode* nep = bro->left;
                    nep->col = fa->col;
                    fa->col = BLACK;
                    nep->rightRotate();
                    nep->leftRotate();
                    if (fa == root_) root_ = nep;
                }
                //情况四：兄弟为黑并且没有子节点或者子节点全部为黑，且父亲节点为红
                else if (fa->col == RED) {
                    fa->col = BLACK;
                    bro->col = RED;
                }
                //情况五：兄弟为黑并且没有子节点或者子节点全部为黑，且父亲节点为黑（全是黑的，上移）
                else {
                    bro->col = RED;
                    _solveRemoveBlack(fa);
                }
            }

            else {
                //情况一：兄弟节点为RED
                if (bro->col == RED) {
                    bro->rightRotate();
                    bro->col = BLACK;
                    fa->col = RED;
                    if (fa == root_) root_ = bro;
                    bro = fa->left;
                }
                //情况二：兄弟节点为黑，子节点有红
                if (bro->left && bro->left->col == RED) {
                    bro->col = fa->col;
                    bro->left->col = BLACK;
                    fa->col = BLACK;
                    bro->rightRotate();
                    if (fa == root_) root_ = bro;
                }
                else if (bro->right && bro->right->col == RED) {
                    RBNode* nep = bro->right;
                    nep->col = fa->col;
                    fa->col = BLACK;
                    nep->leftRotate();
                    nep->rightRotate();
                    if (fa == root_) root_ = nep;
                }
                //情况四：兄弟为黑并且没有子节点或者子节点全部为黑，且父亲节点为红
                else if (fa->col == RED) {
                    fa->col = BLACK;
                    bro->col = RED;
                }
                //情况五：兄弟为黑并且没有子节点或者子节点全部为黑，且父亲节点为黑（全是黑的，上移）
                else {
                    bro->col = RED;
                    _solveRemoveBlack(fa);
                }
            }

        }

        bool _locate(const Key& key, RBNode*& pos) const {  // 返回true：找到节点
            if (pos == end_) return false;
            while (pos) {
                if (comp_(pos->value.first, key)) {
                    if (pos->right == nullptr) return false;
                    pos = pos->right;
                }
                else if (comp_(key, pos->value.first)) {
                    if (pos->left == nullptr) return false;
                    pos = pos->left;
                }
                else return true;
            }
            return false;  // root_ = nullptr
        }

        RBNode* _insert(RBNode* pos, RBNode* newnode) {
            ++size_;
            if (pos == end_) {  // 树为空
                root_ = leftmost_ = rightmost_ = newnode;
                newnode->col = BLACK;
                newnode->parent = nullptr;
                return newnode;
            }

            if (comp_(newnode->value.first, leftmost_->value.first))leftmost_ = newnode;
            if (comp_(rightmost_->value.first, newnode->value.first))rightmost_ = newnode;

            if (comp_(pos->value.first, newnode->value.first)) pos->right = newnode;
            else pos->left = newnode;

            if (pos->col == RED)_solveDoubleRed(newnode);

            return newnode;
        }

        void _erase(RBNode* pos) {
            if (pos == nullptr || pos == end_) return;//???????????????????????????????????????????????????????????????????????????????

            if (leftmost_ == pos) leftmost_ = nullptr;
            if (rightmost_ == pos) rightmost_ = nullptr;
            --size_;

            RBNode* suc;//直接后继
            while (pos->left || pos->right) {
                if (pos->right) suc = pos->sucright();  // 右侧最小
                else suc = pos->sucleft();  // 左侧最大

                bool suc_is_left = suc->parent->left == suc;
                if (suc->parent == pos) {
                    if (pos == root_) root_ = suc;
                    else {
                        if (pos->parent->left == pos)pos->parent->left = suc;
                        else pos->parent->right = suc;
                        suc->parent = pos->parent;
                    }
                    if (suc_is_left) {
                        if (suc->left) suc->left->parent = pos;
                        pos->left = suc->left;
                        if (suc->right) suc->right->parent = pos;
                        if (pos->right) pos->right->parent = suc;
                        std::swap(suc->right, pos->right);
                        suc->left = pos;
                        pos->parent = suc;
                    }
                    else {
                        if (suc->right) suc->right->parent = pos;
                        pos->right = suc->right;
                        if (suc->left) suc->left->parent = pos;
                        if (pos->left) pos->left->parent = suc;
                        std::swap(suc->left, pos->left);
                        suc->right = pos;
                        pos->parent = suc;
                    }
                    std::swap(suc->col, pos->col);
                }
                else {
                    if (pos == root_) root_ = suc;
                    else {
                        if (pos->parent->left == pos)pos->parent->left = suc;
                        else pos->parent->right = suc;
                    }

                    if (suc_is_left)suc->parent->left = pos;
                    else suc->parent->right = pos;

                    if (suc->left) suc->left->parent = pos;
                    if (suc->right) suc->right->parent = pos;
                    if (pos->left) pos->left->parent = suc;
                    if (pos->right) pos->right->parent = suc;

                    std::swap(suc->parent, pos->parent);
                    std::swap(suc->left, pos->left);
                    std::swap(suc->right, pos->right);
                    std::swap(suc->col, pos->col);
                }
            }
            
            if (pos->col == BLACK)_solveRemoveBlack(pos); 
            if (pos == root_) leftmost_ = rightmost_ = root_ = end_;
            else if (pos == pos->parent->left) pos->parent->left = nullptr;
            else pos->parent->right = nullptr;

            delete pos;

            if (leftmost_ == nullptr) {
                leftmost_ = root_;
                if(leftmost_) while (leftmost_->left) leftmost_ = leftmost_->left;
            }
            if (rightmost_ == nullptr) {
                rightmost_ = root_;
                if (rightmost_) while (rightmost_->right) rightmost_ = rightmost_->right;
            }

        }

        void _clear(RBNode* pos) {
            if (pos == nullptr || pos == end_) return;
            if (pos->left) _clear(pos->left);
            if (pos->right) _clear(pos->right);
            delete pos;
        }

        RBNode* _copy(RBNode* pos, map* i, RBNode* fa) {
            if (pos == nullptr) return nullptr;
            RBNode* newnode = new RBNode(pos->value, i, fa, pos->col);
            if (pos->left) newnode->left = _copy(pos->left, i, newnode);
            if (pos->right) newnode->right = _copy(pos->right, i, newnode);
            return newnode;
        }



    public:

        map() :size_(0) {
            end_ = static_cast<RBNode*>(operator new(sizeof(RBNode)));
            end_->id = this;
            root_ = leftmost_ = rightmost_ = end_;
        }

        map(const map& other) :size_(other.size_) {
            end_ = static_cast<RBNode*>(operator new(sizeof(RBNode)));
            end_->id = this;
            root_ = leftmost_ = rightmost_ = end_;
            if (other.size_ == 0) return;
            root_ = _copy(other.root_, this, nullptr);

            leftmost_ = rightmost_ = root_;
            while (leftmost_->left) leftmost_ = leftmost_->left;
            while (rightmost_->right) rightmost_ = rightmost_->right;
        }

        ~map() {
            _clear(root_);
            size_ = 0;
            operator delete (end_);
        }

        map& operator=(const map& other) {
            if (&other == this) return *this;
            clear();
            size_ = other.size_;
            root_ = leftmost_ = rightmost_ = end_;
            if (other.size_ == 0) return *this;
            root_ = _copy(other.root_, this, nullptr);

            leftmost_ = rightmost_ = root_;
            while (leftmost_->left) leftmost_ = leftmost_->left;
            while (rightmost_->right) rightmost_ = rightmost_->right;
            return *this;
        }

        T& at(const Key& key) {
            RBNode* pos = root_;
            if (_locate(key, pos))return pos->value.second;
            throw sjtu::index_out_of_bound();
        }

        const T& at(const Key& key) const {
            RBNode* pos = root_;
            if (_locate(key, pos))return pos->value.second;
            throw sjtu::index_out_of_bound();
        }

        T& operator[](const Key& key) {
            RBNode* pos = root_;
            if (_locate(key, pos))return pos->value.second;
            return _insert(pos, new RBNode(value_type(key, T()), this, pos))->value.second;
        }

        const T& operator[](const Key& key) const {
            RBNode* pos = root_;
            if (_locate(key, pos))return pos->value.second;
            throw sjtu::index_out_of_bound();
        }

        //迭代器相关操作
        iterator begin() { return iterator(leftmost_); }
        const_iterator cbegin() const { return const_iterator(leftmost_); }
        iterator end() { return iterator(end_); }
        const_iterator cend() const { return const_iterator(end_); }

        bool empty() const { return size_ == 0; }

        size_t size() const { return size_; }

        void clear() {
            _clear(root_);
            leftmost_ = rightmost_ = root_ = end_;
            size_ = 0;
        }

        pair<iterator, bool> insert(const value_type& value) {
            RBNode* pos = root_;
            if (_locate(value.first, pos)) {
                return pair<iterator, bool>(iterator(pos), false);  // 插入失败，返回找到的节点
            }
            return pair<iterator, bool>(iterator(_insert(pos, new RBNode(value, this, pos))), true);  // 插入成功，返回插入的节点
        }
        pair<iterator, bool> insert(value_type&& value) {
            RBNode* pos = root_;
            if (_locate(value.first, pos)) {
                return pair<iterator, bool>(iterator(pos), false);  // 插入失败，返回找到的节点
            }
            return pair<iterator, bool>(iterator(_insert(pos, new RBNode(value, this, pos))), true);  // 插入成功，返回插入的节点
        }

        void erase(iterator pos) {
            if (pos.node_->id != this || pos.node_ == end_) throw sjtu::invalid_iterator();
            _erase(pos.node_);
        }

        size_t count(const Key& key) const {  // const 函数内只能调用const函数
            RBNode* pos = root_;
            return _locate(key, pos) ? 1 : 0;
        }

        iterator find(const Key& key) {
            RBNode* pos = root_;
            if (_locate(key, pos)) return iterator(pos);
            return end();
        }
        const_iterator find(const Key& key) const {
            RBNode* pos = root_;
            if (_locate(key, pos)) return const_iterator(pos);
            return cend();
        }
    };

}

#endif
