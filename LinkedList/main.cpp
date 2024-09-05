//
//  main.cpp
//  LinkedList
//
//  Created by 张木林 on 9/4/24.
//

#include <iostream>
#include <assert.h>



// 节点类，代表链表中的一个节点
class Node {
public:
    int data;         // 节点的数据部分
    Node* next;       // 指向下一个节点的指针

    // 构造函数
    Node(int value) {
        data = value;
        next = nullptr;
    }
};

// 链表类
class LinkedList {
private:
    Node* head;       // 链表的头节点

public:
    // 构造函数
    LinkedList() {
        head = nullptr;
    }

    // 向链表末尾添加节点
    void append(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // 删除链表中的一个节点
    void deleteNode(int value) {
        if (head == nullptr) return;

        // 如果要删除的是头节点
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        // 搜索要删除的节点
        Node* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }

        // 找到目标节点并删除
        if (current->next != nullptr) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    // 显示链表内容
    void display() {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    // 析构函数，释放内存
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

int main() {
    LinkedList list;
    
    // 添加节点
    list.append(10);
    list.append(20);
    list.append(30);
    
    // 显示链表内容
    std::cout << "链表内容: ";
    list.display();
    
    // 删除节点
    std::cout << "删除节点20后: ";
    list.deleteNode(20);
    list.display();
    
    return 0;
}
