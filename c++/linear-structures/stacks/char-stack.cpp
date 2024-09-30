#include <iostream>
/*Implementing the Stack data structure using a dynamic array*/


class CharStack {
private:
    static const int INITIAL_CAPACITY = 10;

    char *array; //Dynamic array of characters
    int capacity; //Allocated size of that array
    int count; //Current number of elements in array

    void expandCapacity()
    {
        char *oldArray = array;
        capacity *= 2;
        array = new char[capacity];
        for (int i = 0; i < count; i++)
        {
            array[i] = oldArray[i];
        }
        delete[] oldArray;
    }

public:
    CharStack() {
        capacity = INITIAL_CAPACITY;
        array = new char[capacity];
        count = 0;
    }
    
    ~CharStack() {
        delete[] array;
    }

    int size() {
        return count;
    }

    bool isEmpty() {
        return count == 0;
    }

    void clear() {
        count = 0;
    }

    void push(char ch) {
        if (count == capacity) expandCapacity();
        array[count++] = ch;
    }
    char pop() {
        if (isEmpty()) std::cout << "The stack is empty!" << std::endl;
        return array[--count];
    }

    char peek() {
        if (isEmpty()) std::cout << "The stack is empty!" << std::endl;
        return array[count - 1];
    }
};