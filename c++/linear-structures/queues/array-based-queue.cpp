#include <iostream>


template<typename type>
class Queue {
private:
    static const int INITIAL_CAPACITY = 10;
    type *array;
    int capacity;
    int head;
    int tail;
    void expandCapacity()
    {
        int count = size();
        type *oldArray = array;
        array = new type[2 * capacity];
        for (int i = 0; i < count; i++)
        {
            array[i] = oldArray[(head + i) % capacity];
        }
        capacity *= 2;
        head = 0;
        tail = count;
        delete[] oldArray;
    }

public:
    Queue() {
        capacity = INITIAL_CAPACITY;
        array = new type[capacity];
        head = 0;
        tail = 0;
    }

    ~Queue() {
        delete[] array;
    }

    int size() const {
        return (tail + capacity - head) % capacity;
    }

    bool isEmpty() const {
        return head == tail;
    }

    void clear() {
        head = tail = 0;
    }

    void enqueue(type value) {
        if (size() == capacity - 1) expandCapacity();
        array[tail] = value;
        tail = (tail + 1) % capacity;
    }

    type dequeue() {
        if (isEmpty()) std::cout << "The queue is empty!";
        type result = array[head];
        head = (head + 1) % capacity;
        return result;
    }

    type peek() const {
        if (isEmpty()) std::cout << "The queue is empty!";
        return array[head];
    }
};