#ifndef JOS_BUDDY
#define JOS_BUDDY

#include "inc/memlayout.h"

#define MAX_ORDER 11

struct FreeArea {
    struct ListNode free_list;
    uint32_t        nr_free;
};

/* free areas of different sizes */
extern struct FreeArea* free_areas;

void             buddy_init(void);
struct PageInfo* malloc(uint32_t);
void             free(struct PageInfo* page);

#endif
