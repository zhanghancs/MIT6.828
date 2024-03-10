#include "inc/stdio.h"
#include "inc/types.h"
#include <kern/buddy.h>

extern struct PageInfo* pages;
extern size_t           npages;

struct FreeArea* free_areas;

void set_page_alloc(struct PageInfo* page, uint32_t order) {
    for (uint32_t i = 0; i < (1 << order); i++) {
        // page[i].pp_link = NULL;
        page[i].pp_ref++;
        page[i].pp_order = order;
    }
}

void set_page_order(struct PageInfo* page, uint32_t order) {
    for (uint32_t i = 0; i < (1 << order); i++) {
        page[i].pp_order = order;
    }
}

void set_page_free(struct PageInfo* page, uint32_t order) {
    for (uint32_t i = 0; i < (1 << order); i++) {
        page[i].pp_ref--;
        page[i].pp_order = order;
    }
}

struct PageInfo* get_free_page(uint32_t order) {
    struct ListNode* free_list = &free_areas[order].free_list;
    struct PageInfo* page = list_entry(free_list->next, struct PageInfo, pp_node);
    remove(free_list->next);
    free_areas[order].nr_free--;
    return page;
}

void spilt_page(struct PageInfo* page, uint32_t aim_order, uint32_t now_order) {
    while (now_order != aim_order) {
        now_order--;
        set_page_order(page + (1 << now_order), now_order);
        add(&free_areas[now_order].free_list, &(page[1 << now_order].pp_node));
        free_areas[now_order].nr_free++;
    }
}

uint32_t log2(uint32_t value) {
    uint32_t log = 0;
    while (value >>= 1) {
        ++log;
    }
    return log;
}

void buddy_init(void) {
    for (uint32_t i = 0; i < MAX_ORDER; i++) {
        init(&free_areas[i].free_list);
        free_areas[i].nr_free = 0;
    }
    cprintf("success\n");
    uint32_t page_num = 0;
    for (int i = MAX_ORDER - 1; i >= 0; i--) {
        while (page_num + (1 << i) <= npages) {
            add(&free_areas[i].free_list, &(pages[page_num].pp_node));
            free_areas[i].nr_free++;
            set_page_order(pages + page_num, i);
            page_num += (1 << i);
        }
    }
}

struct PageInfo* malloc(uint32_t page_num) {
    uint32_t         now_order;
    uint32_t         aim_order;
    struct PageInfo* page;
    aim_order = log2(page_num);
    now_order = aim_order;
    if (aim_order >= MAX_ORDER) return NULL;
    while (now_order < MAX_ORDER) {
        if (free_areas[now_order].nr_free) {
            page = get_free_page(now_order);
            set_page_alloc(page, aim_order);
            spilt_page(page, aim_order, now_order);
            return page;
        }
        ++now_order;
    }
    return NULL;
}

void free(struct PageInfo* page) {
    uint32_t         aim_order = page->pp_order;
    uint32_t         now_order = aim_order;
    uint32_t         buddy_index;
    struct PageInfo* buddy;
    set_page_free(page, aim_order);
    while (now_order < MAX_ORDER - 1) {
        buddy_index = (page - pages) ^ (1 << now_order);
        buddy = pages + buddy_index;
        if (buddy->pp_order != now_order || buddy->pp_ref != 0) break;
        remove(&(buddy->pp_node));
        free_areas[now_order].nr_free--;
        if ((buddy_index & (1 << now_order)) == 0) {
            page = buddy;
        }
        now_order++;
    }
    add(&free_areas[now_order].free_list, &(page->pp_node));
    free_areas[now_order].nr_free++;
    if (aim_order != now_order) set_page_order(page, now_order);
}