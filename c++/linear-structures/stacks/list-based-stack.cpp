#include <iostream>

template <typename type>
struct Cell
{
    type data;
    Cell *link;
};

template<typename type>
class Stack {
private:
    Cell *list;
    int count;
public:
    Stack() {
        list = nullptr;
        count = 0;
    }

    ~Stack() {
        clear();
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return (count == 0);
    }

    void clear() {
        while (count > 0) {
            pop();
        }
    }

    void push(type value) {
        Cell *cp = new Cell;
        cp->data = value;
        cp->link = list;
        list = cp;
        count++;
    }

    type pop() {
        if (isEmpty()) std::cout << "The stack is empty!";
        Cell *cp = list;
        type result = cp->data;
        list = list->link;
        count--;
        delete cp;
        return result;
    }

    type peek() const {
        if (isEmpty()) std::cout << "The stack is empty!";
        return list->data;
    }

    Stack(const Stack &src) {
        deepCopy(src);
    }

    Stack & operator=(const Stack &src) {
        if (this != &src) {
            clear();
            deepCopy(src);
        }
        return *this;
    }

    void deepCopy(const Stack &src) {
        count = src.count;
        Cell *tail = nullptr;
        for (Cell *cp = src.list; cp != nullptr; cp = cp->link) {
            Cell *ncp = new Cell;
            ncp->data = cp->data;
            if (tail == nullptr) {
                list = ncp;
            } else {
                tail->link = ncp;
            }
            tail = ncp;
        }
        if (tail != nullptr) tail->link = nullptr;
    }
};