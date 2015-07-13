//
//  hashmap.c
//  NoSQLite
//
//  Created by Elie Raffier on 03/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#include "hashmap.h"
#include "hashmap_node.h"


uint32_t hashmap_hash(char* key)
{
    size_t len = strlen(key);
    uint32_t hash = 0;
    uint32_t i = 0;
    
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    
    return hash;
}

void hashmap_project(hashmap_node_t* result, char** argv, int argc){
    int i;
    int found=0;
    while(result!=NULL){
        for(i=0;i<argc;i++){
            list_value_t* record = result->lt->record;
            while (record!=NULL) {
                if(strcmp(argv[i],record->key)==0){
                    value_print_value(record);
                    found=1;
                }
                record=record->next;
            }
            printf("\t");
        }
        if(found){
            printf("\n");
            found=0;
        }
        result=result->next;
    }
}


hashmap_t* hashmap_create(){
    int i;
    hashmap_t* hashmap=(hashmap_t*)malloc(sizeof(hashmap_t));
    for(i=0;i<BUCKET_NUMBER;i++){
        hashmap->map[i]=NULL;
    }
    return hashmap;
}

hashmap_node_t** hashmap_get_bucket(hashmap_t* map, uint32_t hash){
    uint32_t bucket_n = hash % BUCKET_NUMBER;
    return &map->map[bucket_n];
}

void hashmap_set(hashmap_t *map, hashmap_node_t* node){
    hashmap_node_t** bucket = hashmap_get_bucket(map, node->hash);
    if(*bucket == NULL){
        *bucket = node;
    }else{
        hashmap_node_append_node(*bucket, node);
    }
}

hashmap_node_t* hashmap_find(hashmap_t* hashmap,char *key){
    int create=1;
    hashmap_node_t* result=NULL;
    if(strlen(key)==0){
        int i;
        for(i=0;i<BUCKET_NUMBER;i++){
            hashmap_node_t* tmp=hashmap->map[i];
            while (tmp!=NULL) {
                if(create){
                    result=hashmap_node_create(0, tmp->lt);
                    create=0;
                }else
                    hashmap_node_append_node(result, hashmap_node_create(0, tmp->lt));
                tmp=tmp->next;
            }
        }
        return result;
    }else{
        uint32_t hash=hashmap_hash(key);
        hashmap_node_t* bucket = *hashmap_get_bucket(hashmap, hash);
        if(bucket==NULL)
            return bucket;
        else{
            while(bucket!=NULL){
                if(bucket->hash==hash && strcmp(key,bucket->lt->key)==0){
                    if(create){
                        result=hashmap_node_create(hash, bucket->lt);
                        create=0;
                    }else
                        hashmap_node_append_node(result, hashmap_node_create(hash, bucket->lt));
                }
                bucket=bucket->next;
            }
        }
        return result;
    }
}

hashmap_node_t** hashmap_get(hashmap_t* map, char *key)
{
    hashmap_node_t** bucket = hashmap_get_bucket(map, hashmap_hash(key));
    return bucket;
}

void hashmap_destroy(hashmap_t* hashmap){
    int i;
    hashmap_node_t* node;
    for(i=0;i<BUCKET_NUMBER;i++){
        node=hashmap->map[i];
        hashmap_node_destroy_with_values(node);
    }
}