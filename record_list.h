//
//  record_list.h
//  NoSQLite
//
//  Created by Elie Raffier on 22/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#ifndef __NoSQLite__record_list__
#define __NoSQLite__record_list__

#include <stdio.h>
#include "list_value.h"

typedef struct record_list_s {
    list_value_t* record;
    struct record_list_s* next;
} record_list_t;

record_list_t* record_list_create(list_value_t** record);

void record_list_append(record_list_t* list,list_value_t** record);

record_list_t* record_list_contains(record_list_t* list,list_value_t** record);

#endif /* defined(__NoSQLite__record_list__) */
