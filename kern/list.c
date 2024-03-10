#include "inc/list.h"

void add(struct ListNode* list_head, struct ListNode* node) {
    node->next = list_head->next;
    node->prev = list_head;
    node->next->prev = node;
    list_head->next = node;
}

void remove(struct ListNode* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void init(struct ListNode* list_head) {
    list_head->next = list_head;
    list_head->prev = list_head;
}