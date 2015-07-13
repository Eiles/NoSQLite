//
//  list_value.c
//  NoSQLite
//
//  Created by Elie Raffier on 03/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#include "list_value.h"

list_value_t* list_value_create(char* key,char* value, int type){
    if(value == NULL)
        return NULL;
    int size;
    switch (type) {
        case INT_TYPE:
            size = sizeof(int);
            break;
        case DOUBLE_TYPE:
            size = sizeof(double);
            break;
        case STRING_TYPE:
            size = (int)strlen(value)+1;
            break;
        default:
            return NULL;
            break;
    }
    list_value_t* list;
    if((list = (list_value_t*)malloc(sizeof(list_value_t)))==NULL)
        return NULL;
    list->type = type;
    list->key = (char*)malloc(strlen(key)+1);
    strcpy(list->key, key);
    list->value = (char*)malloc(size);
    memcpy(list->value, value, size);
    list->next = NULL;
    list->record = list;
    return list;
}

void list_value_append(list_value_t* list,char* key, char* value, int type){
    if(list == NULL || value == NULL)
        return;
    list_value_t* tmp=list;
    while(tmp->next != NULL)
        tmp=tmp->next;
    tmp->next=list_value_create(key,value,type);
    tmp->next->record = list;
}


void value_print(list_value_t* list){
    if (list == NULL)
        return;
    
    int intval;
    double doubleval;
    switch (list->type) {
        case INT_TYPE:
            memcpy(&intval,list->value, sizeof(int));
            list->key?printf("%s : %d",list->key, intval):printf("%d", intval);
            break;
        case DOUBLE_TYPE:
            memcpy(&doubleval,list->value, sizeof(double));
            list->key?printf("%s : %lf",list->key, doubleval):printf("%lf", doubleval);
            break;
        case STRING_TYPE:
            list->key?printf("%s : %s",list->key, list->value):printf("%s",list->value);
            break;
    }
}

void list_print(list_value_t* list){
    printf("{ ");
    while(list != NULL){
        value_print(list);
        list=list->next;
        printf(", ");
    }
     printf("}");
}

void list_value_destroy(list_value_t* list){
    list_value_t* tmp=NULL;
        tmp=list;
        free(tmp->key);
        tmp->key=NULL;
        free(tmp->value);
        tmp->value=NULL;
        free(tmp);
        tmp=NULL;
}