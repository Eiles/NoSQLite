//
//  hashmap.h
//  NoSQLite
//
//  Created by Elie Raffier on 03/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#ifndef __NoSQLite__hashmap__
#define __NoSQLite__hashmap__
#define BUCKET_NUMBER 50

#include <stdio.h>
#include "list_value.h"
#include "hashmap_node.h"

typedef int (*hashmap_compare)(void *a, void *b);

typedef struct hashmap_s {
    hashmap_node_t* map[BUCKET_NUMBER];
} hashmap_t;

uint32_t hashmap_hash(char* key);

hashmap_t* hashmap_create();

void hashmap_set(hashmap_t *map, hashmap_node_t* node);

hashmap_node_t** hashmap_get(hashmap_t* map, char *key);

hashmap_node_t* hashmap_find(hashmap_t* hashmap,char *key);

void hashmap_project(hashmap_node_t* result, char** argv, int argc);

void hashmap_destroy(hashmap_t* hashmap);

list_value_t* JSON_parse(char* string);

int JSON_extract_key(char* string, char** keypointer);

int JSON_extract_value(char* string, char** valuepointer);

hashmap_t* hashmap_load(char * db);

void hashmap_save(hashmap_t* hashmap,char* db);

void hashmap_where(hashmap_node_t** result, char *string);
#endif /* defined(__NoSQLite__hashmap__) */
