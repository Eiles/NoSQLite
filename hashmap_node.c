//
//  hashmap_node.c
//  NoSQLite
//
//  Created by Elie Raffier on 10/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#include "hashmap_node.h"

hashmap_node_t* hashmap_node_create(uint32_t hash, list_value_t* lt)
{
    hashmap_node_t* node = (hashmap_node_t*)malloc(sizeof(hashmap_node_t));

    node->lt=lt;
    node->hash = hash;
    node->next=NULL;
    return node;
}

void hashmap_node_append(hashmap_node_t* node, uint32_t hash, list_value_t* lt){
    if (node == NULL)
        return;

    hashmap_node_t* tmp=node;
    while(tmp->next != NULL)
        tmp=tmp->next;
    tmp->next=hashmap_node_create(hash, lt);
}

void hashmap_node_append_node(hashmap_node_t* node, hashmap_node_t* newnode){
    if (node == NULL)
        return;
    
    hashmap_node_t* tmp=node;
    while(tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = newnode;
}

void hashmap_node_destroy_with_values(hashmap_node_t* node){
    hashmap_node_t* tmp=NULL;
    while(node!=NULL){
        tmp=node;
        node=node->next;
        list_value_destroy(tmp->lt);
        free(tmp);
    }
}

void hashmap_node_destroy(hashmap_node_t* node){
    hashmap_node_t* tmp=NULL;
    while(node!=NULL){
        tmp=node;
        node=node->next;
        free(tmp);
    }
}
