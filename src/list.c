/*
 * NEATC. C implementation of NEAT: NeuroEvolution of Augmented Topologies.
 * Copyright (C) 2017  Lars P. Søndergaard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "list.h"

#include <stdio.h>
#include <stdlib.h>


List* new_list() {
    List* list = calloc(1, sizeof(List));
    list_init(list);
    return list;
}

void list_init(List* list) {
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    list->capacity = LIST_INITIAL_CAPACITY;
    list->items = calloc(1, sizeof(ListItem*) * list->capacity);
}

ListItem* new_listitem(void* data) {
    ListItem* item = calloc(1, sizeof(ListItem));
    listitem_init(item, data);
    return item;
}

void listitem_init(ListItem* item, void* data) {
    item->data = data;
    item->next = NULL;
    item->prev = NULL;
}

void listitem_set(ListItem* item, void* data) {
    item->data = data;
}

void list_append(List* list, ListItem* item) {
    list_grow_capacity(list);
    if (list->size == 0) {
        list->head = item;
        list->tail = item;
        item->next = NULL;
        item->prev = NULL;
    } else {
        item->next = NULL;
        item->prev = list->tail;
        item->prev->next = item;
        list->tail = item;
    }
    list->items[list->size] = item;
    list->size++;
}

void list_grow_capacity(List* list) {
    if (list->size < list->capacity) {
        return;
    }
    list->capacity += LIST_GROWING_VALUE;
    ListItem** newItems = realloc(list->items, sizeof(ListItem*) * list->capacity);
    if (!newItems) {
        fprintf(stderr, "Could not reallocate list items new capacity\n");
        exit(1);
    }
    list->items = newItems;
}

ListItem* list_get(List* list, ListIndex index) {
    if (index < 0 || index >= list->size) {
        return NULL;
    }
    return list->items[index];
}

ListItem* list_set(List* list, ListIndex index, ListItem* item) {
    if (index < 0 || index >= list->size) {
        fprintf(stderr, "Index out of bound.\n");
        return NULL;
    }
    ListItem* old_item = list->items[index];
    list->items[index] = item;

    if (index > 0) {
        item->prev = list->items[index-1];
        item->prev->next = item;
    } else {
        item->prev = NULL;
    }
    if (index < (list->size-1)) {
        item->next = list->items[index+1];
        item->next->prev = item;
    } else {
        item->next = NULL;
    }
    return old_item;
}

ListItem* list_remove(List* list, ListIndex index) {
    ListItem* item = list_get(list, index);
    ListItem* before = item->prev;
    ListItem* after = item->next;

    if (before != NULL) {
        before->next = after;
    }
    if (after != NULL) {
        after->prev = before;
    }
    if (list->head == item) {
        list->head = after;
    }
    if (list->tail == item) {
        list->tail = before;
    }
    item->next = NULL;
    item->prev = NULL;
    int i;
    for (i=index+1; i<list->size; i++) {
        list->items[i-1] = list->items[i];
    }
    list->items[list->size-1] = NULL;
    list->size--;
    return item;
}

void list_free(List* list) {
    int i;
    ListItem* item;
    for (i=0; i<list->size; i++) {
        item = list_get(list, i);
        free(item);
    }
    free(list->items);
}
