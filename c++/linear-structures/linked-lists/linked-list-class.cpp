#include <iostream>


struct Node {
    int key;
    Node *next;
};


class LinkedList {
private:
    Node *head;
public:
    LinkedList(int arr[], int length) {
        Node *node = createNode(arr[length - 1], nullptr);
        for (int i = length - 2; i > -1; i--) {
            node = createNode(arr[i], node);
        }
        head = node;
    }

    Node *createNode(int key, Node *next) {
        Node *node = new Node;
        node->key = key;
        node->next = next;
        return node;
    }

    void display() {
        for (Node *node = head; node != nullptr; node = node->next) {
            std::cout << node->key << " -> ";
        }
        std::cout << "*" << std::endl;
    }

    Node *search(int key) {
        Node *temp = head;
        while ((temp != nullptr) && (temp->key != key)) {
            temp = temp->next;
        }
        return temp;
    }

    void insertStart(int value) {
        Node *temp = new Node;
        temp->key = value;
        temp->next = head;
        head = temp;
    }

    void insertPosition(int pos, int value) {
        Node *prev = new Node;
        Node *cur = new Node;
        cur = head;
        for (int i = 0; i < pos; i++) {
            prev = cur;
            cur = cur->next;
        }
        Node *newNode = new Node;
        newNode->key = value;
        prev->next = newNode;
        newNode->next = cur;
    }

    void deleteFirst() {
        Node *temp = new Node;
        temp = head;
        head = head->next;
        delete temp;
    }

    void deletePosition(int pos) {
        Node *prev = new Node;
        Node *cur = new Node;
        cur = head;
        for (int i = 0; i < pos; i++) {
            prev = cur;
            cur = cur->next;
        }
        Node *node = prev->next;
        prev->next = cur->next;
        delete node;
    }

    void deleteWithKey(int key) {
        Node *prev = new Node;
        Node *cur = new Node;
        cur = head;
        while ((cur != nullptr) && (cur->key != key)) {
            prev = cur;
            cur = cur->next;
        }
        if (cur == nullptr) {
            std::cout << "There is no such key!" << std::endl;
        } else {
            Node *node = prev->next;
            prev->next = cur->next;
            delete node;
        }
    }
};


int main() {
    static const int LENGTH = 5;
    int arr[LENGTH] = {1, 2, 3, 4, 5};

    LinkedList list(arr, LENGTH);
    list.display();

    //Node *a = list.search(5);
    //std::cout << a->key;

    list.insertStart(10);
    list.display();

    // list.insertPosition(3, 6);
    // list.display();

    list.deletePosition(2);
    list.display();
}   