//
// Created by nitori on 18.03.17.
//

#ifndef NEAT_LIST_H
#define NEAT_LIST_H

#include <stdint.h>

#define LIST_INITIAL_CAPACITY 100
#define LIST_GROWING_VALUE 100

typedef struct List List;
typedef struct ListItem ListItem;
typedef int64_t ListIndex;

struct List {
    ListItem* head;
    ListItem* tail;
    size_t size;
    size_t capacity;
    ListItem** items;
};

struct ListItem {
    ListItem* next;
    ListItem* prev;
    void* data;
};

List* new_list();
void list_init(List* list);

ListItem* new_listitem(void* data);
void listitem_init(ListItem* item, void* data);
void listitem_set(ListItem* item, void* data);

void list_append(List* list, ListItem* item);
void list_grow_capacity(List* list);
ListItem* list_get(List* list, ListIndex index);
ListItem* list_set(List* list, ListIndex index, ListItem* item);
ListItem* list_remove(List* list, ListIndex index);
void list_free(List* list);

#endif //NEAT_LIST_H
