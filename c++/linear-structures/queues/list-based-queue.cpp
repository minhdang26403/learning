#include <iostream>

template<typename type>
struct Cell {
    type data;
    Cell *link;
};


template<typename type>
class Queue {
private:
    Cell *head;
    Cell *tail;
    int count;
public:
    Queue() {
        head = tail = nullptr;
        count = 0;
    }
    ~Queue() {
        clear();
    }

    int size() const {
        return count;
    }

    bool isEmpty() {
        return count == 0;
    }

    void clear() {
        count = 0;
    }

    void enqueue(type value) {
        Cell *cp = new Cell;
        cp->data = value;
        cp->link = nullptr;
        if (head == nullptr) {
            head = cp;
        } else {
            tail->link = cp;
        }
        tail = cp;
        count++;
    }

    type dequeue() {
        if (isEmpty()) std::cout << "The queue is empty!";
        Cell *cp = head;
        type result = cp->data;
        head = cp->link;
        if (head == nullptr) tail = nullptr;
        delete cp;
        count--;
        return result;
    }

    type peek() const {
        if (isEmpty()) std::cout << "The queue is empty!";
        return head->data;
    }
};