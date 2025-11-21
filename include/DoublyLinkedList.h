#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <memory_resource>
#include <cstddef>
#include <iterator>

template<typename T>
struct ListNode {
    T value;
    ListNode* next = nullptr;
    ListNode* prev = nullptr;
};

template<typename T>
class DoublyLinkedList {
private:
    using Node = ListNode<T>;
    using Alloc = std::pmr::polymorphic_allocator<Node>;

public:
    explicit DoublyLinkedList(std::pmr::memory_resource* mr = nullptr)
        : alloc_(mr ? mr : std::pmr::get_default_resource()) 
        , head_(nullptr)
        , tail_(nullptr)
        , size_(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void push_back(const T& value) {
        Node* node = alloc_.allocate(1);
        new (&node->value) T(value);
        node->next = nullptr;
        node->prev = tail_;
        if (tail_) tail_->next = node;
        else head_ = node;
        tail_ = node;
        ++size_;
    }

    void push_front(const T& value) {
        Node* node = alloc_.allocate(1);
        new (&node->value) T(value);
        node->prev = nullptr;
        node->next = head_;
        if (head_) head_->prev = node;
        else tail_ = node;
        head_ = node;
        ++size_;
    }

    void clear() {
        while (head_) {
            Node* next = head_->next;
            head_->value.~T();
            alloc_.deallocate(head_, 1);
            head_ = next;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        explicit iterator(Node* node) : node_(node) {}
        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }
        iterator& operator++() { node_ = node_->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return node_ == other.node_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

    private:
        Node* node_;
    };

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }

private:
    Alloc alloc_;
    Node* head_;
    Node* tail_;
    std::size_t size_;
};

#endif