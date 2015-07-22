//
//  list_value.h
//  NoSQLite
//
//  Created by Elie Raffier on 03/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#ifndef __NoSQLite__list_value__
#define __NoSQLite__list_value__
#define INT_TYPE 1
#define DOUBLE_TYPE 2
#define STRING_TYPE 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct list_value_s{
    int type;
    char* key;
    char* value;
    struct list_value_s* record;
    struct list_value_s* next;
} list_value_t;

list_value_t* list_value_create(char* key,char* value, int type);

void list_value_append(list_value_t* list,char* key, char* value, int type);

void value_print(list_value_t* list);

void list_print(list_value_t* list);

void list_value_destroy(list_value_t* list);

void value_print_value(list_value_t* list);

void list_value_destroy_all(list_value_t* list);

void writeDB(FILE** filepointer,list_value_t* record);

void value_write(FILE** filepointer,list_value_t* record);
#endif /* defined(__NoSQLite__list_value__) */
