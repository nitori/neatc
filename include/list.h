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

#ifndef NEAT_LIST_H
#define NEAT_LIST_H

#include <stdlib.h>
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
