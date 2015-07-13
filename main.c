//
//  main.c
//  NoSQLite
//
//  Created by Elie Raffier on 03/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#include <stdio.h>
#include "hashmap.h"
#include <inttypes.h>

int main(int argc, const char * argv[]) {
    hashmap_t* hashmap = hashmap_create();
    int j,k;
    char* val;
    hashmap_node_t* node=NULL;
    double d = 10.5;
    val=malloc(sizeof(d));
    memcpy(val, &d, sizeof(d));
    list_value_t* list= list_value_create("Elie", val, DOUBLE_TYPE);
    int i=20;
    val=malloc(sizeof(i));
    memcpy(val, &i, sizeof(i));
    list_value_append(list, "grade",val, INT_TYPE);
    list_value_append(list, "machin", "Bite", STRING_TYPE);
    list_value_t* tmp=list;
    hashmap_node_t* tmp_node = NULL;
    while(tmp!=NULL){
        tmp_node = hashmap_node_create(hashmap_hash(tmp->key), tmp);
        hashmap_set(hashmap, tmp_node);
        tmp=tmp->next;
    }
    
    d = 13.5;
    val=malloc(sizeof(d));
    memcpy(val, &d, sizeof(d));
    list = list_value_create("Tom", val, DOUBLE_TYPE);
    i=13;
    val=malloc(sizeof(i));
    memcpy(val, &i, sizeof(i));
    list_value_append(list, "grade",val, INT_TYPE);
    list_value_append(list, "truc", "Bite", STRING_TYPE);
    tmp=list;
    tmp_node = NULL;
    while(tmp!=NULL){
        tmp_node = hashmap_node_create(hashmap_hash(tmp->key), tmp);
        hashmap_set(hashmap, tmp_node);
        tmp=tmp->next;
    }
    
    for(i=0;i<BUCKET_NUMBER;i++){
        if(hashmap->map[i]!=NULL){
            printf("%"PRIu32"\n",hashmap->map[i]->hash);
        }
    }
    
    node=hashmap_find(hashmap, "grade");
    while(node!=NULL){
        value_print(node->lt);
        printf("\nFrom record : ");
        list_print(node->lt->record);
        printf("\n");
        if(node->lt->record)
        node=node->next;
    }
    hashmap_destroy(hashmap);
    return 0;
}
