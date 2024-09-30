//General Stack implementation using dynamic array

#include <iostream>


class Stack {
private:
    static const int INITIAL_CAPACITY = 10;

    char *array;
    int capacity;
    int count;

    template <typename type>
    void expandCapacity()
    {
        type *oldArray = array;
        capacity *= 2;
        array = new type[capacity];
        for (int i = 0; i < count; i++)
        {
            array[i] = oldArray[i];
        }
        delete[] oldArray;
    }

public:
    template<typename type>
    Stack() {
        capacity = INITIAL_CAPACITY;
        array = new type[capacity];
        count = 0;
    }

    ~Stack() {
        delete[] array;    
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    void clear() {
        count = 0;
    }

    template<typename type>
    void push(type value) {
        if (count == capacity) expandCapacity();
        array[count++] = value;
    }

    template<typename type>
    type pop() {
        if (isEmpty()) std::cout << "The stack is empty!" << std::endl;
        return array[--count];
    }

    template<typename type>
    type peek() {
        if (isEmpty()) std::cout << "The stack is empty!" << std::endl;
        return array[count - 1];
    }
};