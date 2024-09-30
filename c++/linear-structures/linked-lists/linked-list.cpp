#include <iostream>

struct Node {
    int key;
    Node *next;
};

/*Function declearations*/
Node *createLinkedList(int arr[]);
Node *createNode(int key, Node *next);
Node *search(Node *node, int key);
void insertStart(Node *&head, int value);
void insertPosition(Node *&head, int pos, int value);
void deleteFirst(Node *&head);
void deletePosition(Node *&head, int pos);
void deleteWithKey(Node *&head, int key);


/*Implementations of operations on singly linked list*/

Node *createLinkedList(int arr[], int length) {
    Node *node = createNode(arr[0], NULL);
    for (int i = 1; i < length; i++) {
        node = createNode(arr[i], node);
    }
    return node;
}


Node *createNode(int key, Node *next) {
    Node *node = new Node;
    node->key = key;
    node->next = next;
    return node;
}

Node *search(Node *node, int key) {
    while ((node != nullptr) && (node->key != key)) {
        node = node->next;
    }
    return node;
}

void insertStart(Node *&head, int value) {
    Node *temp = new Node;
    temp->key = value;
    temp->next = head;
    head = temp;
} 

void insertPosition(Node *&head, int pos, int value) {
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

void deleteFirst(Node *&head) {
    Node *temp = new Node;
    temp = head;
    head = head->next;
    delete temp;
}


void deletePosition(Node *&head, int pos) {
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

void deleteWithKey(Node *&head, int key) {
    Node *prev = new Node;
    Node *cur = new Node;
    cur = head;
    while ((cur != nullptr) && (cur->key != key))
    {   
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


void display(Node *head) {
    for (Node *node = head; node != NULL; node = node->next)
    {
        std::cout << node->key << " -> ";
    }
    std::cout << "*" << std::endl;
}

int main() {
    static const int LENGTH = 5;
    int arr[LENGTH] = {1, 2, 3, 4, 5};

    Node *list = createLinkedList(arr, LENGTH);
    display(list);

    // Node *a = search(list, 3);
    // std::cout << a->key;

    insertStart(list, 9);
    display(list);

    //insertPosition(list, 2, 7);
    //display(list);

    //deletePosition(list, 3);
    //display(list);

    deleteWithKey(list, 1);
    display(list);
}