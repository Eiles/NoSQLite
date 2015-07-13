//
//  hashmap_node.h
//  NoSQLite
//
//  Created by Elie Raffier on 10/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#ifndef __NoSQLite__hashmap_node__
#define __NoSQLite__hashmap_node__

#include <stdio.h>
#include "list_value.h"

typedef int (*hashmap_compare)(void *a, void *b);

typedef struct hashmap_node_s {
    list_value_t* lt;
    uint32_t hash;
    struct hashmap_node_s* next;
} hashmap_node_t;

hashmap_node_t* hashmap_node_create(uint32_t hash, list_value_t* lt);

void hashmap_node_append(hashmap_node_t* node, uint32_t hash, list_value_t* lt);

void hashmap_node_append_node(hashmap_node_t* node, hashmap_node_t* newnode);

void hashmap_node_destroy(hashmap_node_t* node);

void hashmap_node_destroy_with_values(hashmap_node_t* node);

#endif /* defined(__NoSQLite__hashmap_node__) */
