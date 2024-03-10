#ifndef JOS_LIST_H
#define JOS_LIST_H

struct ListNode {
    struct ListNode* prev;
    struct ListNode* next;
};

void add(struct ListNode* head, struct ListNode* node);

void remove(struct ListNode* node);

void init(struct ListNode* head);

#define container_of(ptr, type, member) ({            \
    const typeof(((type*)0)->member)* __mptr = (ptr); \
    (type*)((char*)__mptr - offsetof(type, member));  \
})

#define list_entry(ptr, type, member) container_of(ptr, type, member)

#endif
