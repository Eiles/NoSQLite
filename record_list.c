//
//  record_list.c
//  NoSQLite
//
//  Created by Elie Raffier on 22/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#include "record_list.h"


record_list_t* record_list_create(list_value_t** record){
    record_list_t* new_list=(record_list_t*)malloc(sizeof(record_list_t));
    new_list->record=*record;
    new_list->next=NULL;
    return new_list;
}

void record_list_append(record_list_t* list,list_value_t** record){
    record_list_t* tmplist=list;
    while (tmplist->next!=NULL) {
        tmplist=tmplist->next;
    }
    tmplist->next=record_list_create(record);
}

record_list_t* record_list_contains(record_list_t* list, list_value_t** record){
    record_list_t* tmplist=list;
    while (tmplist->next!=NULL) {
        if(tmplist->record==*record){
            return NULL;
        }
        tmplist=tmplist->next;
    }
    if(tmplist->record==*record){
        return NULL;
    }
    else
        return tmplist;
}