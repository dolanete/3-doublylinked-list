#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node(nullptr) { //correct
    }; // Default constructor
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return node->data; // Will return the data in the node
        }
        pointer operator->() const {
            return &(node->data); // Return the address
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next; //correct
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator temp = *this; // Makes a temparary copy
            ++(*this); // increment current
            return temp; // return the one before
            // TODO
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            basic_iterator temp = *this; // Makes a temparary copy
            --(*this); // increment current
            return temp;// return the one before
        }

        bool operator==(const basic_iterator& other) const noexcept {
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            return node != other.node;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List(): _size(0), head(0), tail(0) {
        head.next = &tail;
        head.prev = nullptr;
        tail.next = nullptr;
        tail.prev = &head;

        // TODO - Don't forget the list initialier
    }
    List( size_type count, const T& value ): _size(count), head(0), tail(0) {

        /* head.next = &tail;
        tail.prev = &head;
        //Node* temp = new Node(value, &head, &tail);
        Node* ptr = &head;
        Node* temp;
        for (size_type i = 0; i < count; i++) {
            temp = new Node(value, ptr, &tail); // Make Nodes

            ptr->next = temp; // Connect the previous to current
            tail.prev = temp; // Connect the tail node to the current

            ptr = temp; 
            
        } */
        head.next = &tail;
        tail.prev = &head;
        _size = 0;
        for (int i = 0; i < count; i++) {
            insert(cbegin(), value);
        }

    }
    explicit List( size_type count ): _size(count), head(0), tail(0)  {
        head.next = &tail;
        tail.prev = &head;
        _size = 0;
        for (size_type i = 0; i < count; i++) {
            insert(cbegin(), T());
        }
    }
    List( const List& other ): _size(0), head(0), tail(0) { //Copy Constructor
        head.next = &tail; // create a blank linked list (head points to tail and tail points to head)
        tail.prev = &head;
            for (const_iterator i = other.cbegin(); i != other.cend(); i++) { //Iterate throughout the parameter list
                push_back(i.node->data); //Push back to the current linked list for each value that is iterated through
            }
        }
    List( List&& other ): _size(0), head(), tail() { //Move Constructor
        head.next = &tail; //Clear
        tail.prev = &head;

        head = other.head; //set the head to other head
        tail = other.tail; // set tail to other tail
        other.head.next->prev = &head; //Set the node after other head to point back at first head.
        other.tail.prev->next = &tail; // Set the node before the other tail to point forward at the first tail
        other.head.next = &other.tail; //Empty the other list.
        other.tail.prev = &other.head;

        _size = other._size; //Set sizes
        other._size = 0;

    }
    ~List() {
        clear();
    }
    List& operator=( const List& other ) {
        if (this == &other) {
            return *this;
        }
        clear();
        head.next = &tail;
        tail.prev = &head;
            for (const_iterator i = other.cbegin(); i != other.cend(); i++) {
                push_back(i.node->data);
            }
        return *this;
    }
    List& operator=( List&& other ) noexcept {
        if (this == &other) {
            return *this;
        }
        clear();
        head.next = &tail; //Clear
        tail.prev = &head;

        head = other.head; //set the head to other head
        tail = other.tail; // set tail to other tail
        other.head.next->prev = &head; //Set the node after other head to point back at first head.
        other.tail.prev->next = &tail; // Set the node before the other tail to point forward at the first tail
        other.head.next = &other.tail; //Empty the other list.
        other.tail.prev = &other.head;

        _size = other._size; //Set sizes
        other._size = 0;
        return *this;
        
    }

    reference front() {
        return head.next->data;
    }
    const_reference front() const {
        return head.next->data;
    }
	
    reference back() {
        return tail.prev->data;
    }
    const_reference back() const {
        return tail.prev->data;
    }
	
    iterator begin() noexcept {
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        if (_size == 0) {
            return true;
        }
        return false;
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        for (basic_iterator i = cbegin(); i != cend(); i++) {
            erase(i);
        }
        head.next = &tail;
        tail.prev = &head;
    }

    iterator insert( const_iterator pos, const T& value ) {
        Node* newNode = new Node(value, pos.node->prev, pos.node);
        
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        _size++;
        return iterator(newNode);
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node* newNode = new Node(std::move(value), pos.node->prev, pos.node);
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        _size++;
        return iterator(newNode);
    }

    iterator erase( const_iterator pos ) {
        pos.node->prev->next = pos.node->next;
        pos.node->next->prev = pos.node->prev;
        delete pos.node;
        _size--;
        return iterator(pos.node->next);
    }

    void push_back( const T& value ) {
        const_iterator pos = cend();
        Node* newNode = new Node(value, pos.node->prev, pos.node);
        
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        _size++;
    }
    void push_back( T&& value ) {
        const_iterator pos = cend();
        Node* newNode = new Node(std::move(value), pos.node->prev, pos.node);
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        _size++;
    }

    void pop_back() {
        const_iterator pos = cend();
        erase(--pos);
    }
	
    void push_front( const T& value ) {
        const_iterator pos = cbegin();
        Node* newNode = new Node(value, pos.node->prev, pos.node);
        
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        _size++;
    }
	void push_front( T&& value ) {
        const_iterator pos = cbegin();
        Node* newNode = new Node(std::move(value), pos.node->prev, pos.node);
        pos.node->prev->next = newNode;
        pos.node->prev = newNode;
        _size++;
    }

    void pop_front() {
        erase(cbegin());
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}