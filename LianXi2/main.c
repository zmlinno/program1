//
//  main.c
//  LianXi2
//
//  Created by 张木林 on 3/10/24.
//

#include <stdio.h>

class Partition {
public:
    ListNode* partition(ListNode* pHead, int x) {
        if (pHead == nullptr || pHead->next == nullptr) {
            return pHead;
        }
        
        ListNode* lessHead = nullptr; // 小于x的结点头指针
        ListNode* greaterHead = nullptr; // 大于等于x的结点头指针
        ListNode* lessTail = nullptr; // 小于x的结点尾指针
        ListNode* greaterTail = nullptr; // 大于等于x的结点尾指针
        
        ListNode* cur = pHead;
        while (cur != nullptr) {
            ListNode* nextNode = cur->next; // 先保存下一个结点
            
            if (cur->val < x) {
                // 插入到小于x的链表中
                if (lessHead == nullptr) {
                    lessHead = lessTail = cur;
                } else {
                    lessTail->next = cur;
                    lessTail = cur;
                }
                lessTail->next = nullptr; // 将小于x的链表结尾置为nullptr
            } else {
                // 插入到大于等于x的链表中
                if (greaterHead == nullptr) {
                    greaterHead = greaterTail = cur;
                } else {
                    greaterTail->next = cur;
                    greaterTail = cur;
                }
                greaterTail->next = nullptr; // 将大于等于x的链表结尾置为nullptr
            }
            cur = nextNode; // 更新当前结点
        }
        
        // 拼接两个链表
        if (lessHead != nullptr) {
            lessTail->next = greaterHead;
            return lessHead;
        } else {
            return greaterHead;
        }
    }
};
