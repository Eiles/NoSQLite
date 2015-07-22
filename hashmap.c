//
//  hashmap.c
//  NoSQLite
//
//  Created by Elie Raffier on 03/07/2015.
//  Copyright (c) 2015 Elie Raffier. All rights reserved.
//

#include "hashmap.h"
#include "hashmap_node.h"
#include "record_list.h"

//int value_compare(list_value_t* a, list_value_t* b){
//    if(a->type==b->type)
//        return compare(a,b);
//    
//    if(a->type==STRING_TYPE)
//        return -1;
//    else
//        if (b->type==STRING_TYPE)
//            return 1;
//        else
//            return (compare(a,b));
//}

//int compare(list_value_t* avalue, list_value_t* bvalue){
//    if(avalue->type==STRING_TYPE)
//        return strcmp(avalue->value,bvalue->value);
//    
//    if(a<b)
//        return 1;
//    else{
//        if(a>b){
//            return -1;
//        }else{
//            return 0;
//        }
//    }
//}

void char_to_double(char* value,void *toconvert){
    double converted;
    double* todouble;
    memcpy(&converted, value, sizeof(double));
    todouble=&converted;
    toconvert=todouble;
}

//int char_to_int(){
//    
//}

uint32_t hashmap_hash(char* key){
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

void hashmap_where(hashmap_node_t** result, char *string){
    int found=0;
    hashmap_node_t* prev=NULL;
    hashmap_node_t* current=*result;
    list_value_t* tmprec;
    list_value_t* wheres=JSON_parse(string);
    list_value_t* tmpw;
    while (current!=NULL) {
        tmpw=wheres;
        while(tmpw!=NULL){
            tmprec=current->lt->record;
            while (tmprec!=NULL) {
                if(strcmp(tmpw->key,tmprec->key)==0){ //&& value_compare(tmpw, tmprec)==0){
                    found=1;
                }
                tmprec=tmprec->next;
            }
            tmpw=tmpw->next;
        }
        if(!found){
            if (prev) {
                prev->next=current->next;
                free(current);
                current=prev->next;
            }
            else{
                *result=current->next;
                free(current);
                current=*result;
                
            }
        }else{
            prev=current;
            current=current->next;
        }
        found=0;
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

hashmap_node_t** hashmap_get(hashmap_t* map, char *key){
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

list_value_t* JSON_parse(char *string){
    char* toParse=NULL;
    char** value=malloc(sizeof(char*));
    char** key=malloc(sizeof(char*));
    int type=0;
    int i,j;
    i=j=0;
    int keylen=0;
    int create=1;
    list_value_t* list=NULL;
    if(string[0] !='{' || string[strlen(string)-1] !='}'){
        printf("JSON can't be parsed\n");
        return NULL;
    }
    i++;
    j++;
    while(string[i]!='}'){
        if(string[i]==','){
            i++;
            toParse=malloc(sizeof(char)*(i-j)+1);
            strncpy(toParse,string+j,i-j-1);
            toParse[i-j]='\0';
            keylen=JSON_extract_key(toParse, key);
            type=JSON_extract_value(toParse, value);
            if(type==0){
                goto error;
            }else{
                if(create){
                    list=list_value_create(*key, *value, type);
                    create=0;
                }else{
                    list_value_append(list, *key, *value, type);
                }
            }
            j=i;
        }
        i++;
    }
    toParse=malloc(sizeof(char)*(strlen(string)-j)+1);
    strncpy(toParse,string+j,strlen(string+j)-1);
    toParse[strlen(string+j)]='\0';
    keylen=JSON_extract_key(toParse, key);
    type=JSON_extract_value(toParse, value);
    if(type==0 || keylen==0){
        goto error;
    }else{
        if(create){
            list=list_value_create(*key, *value, type);
            
            create=0;
        }else{
            list_value_append(list, *key, *value, type);
        }
    }
     return list;
    
error:
    if(!create)
        list_value_destroy_all(list);
    free(*key);
    free(*value);
    free(toParse);
    return NULL;
    
}

void hashmap_add_list(hashmap_t* hashmap, list_value_t* lt){
    hashmap_node_t* tmp_node;
    while(lt!=NULL){
        tmp_node = hashmap_node_create(hashmap_hash(lt->key), lt);
        hashmap_set(hashmap, tmp_node);
        lt=lt->next;
    }
    
}

int JSON_extract_key(char* string, char** keypointer){
    int i=0;
    while(string[i]!=':' && string[i]!='\0'){
        i++;
    }
    if(string[i]=='\0'){
        printf("JSON can't be parsed : Key not found\n");
        *keypointer=NULL;
        return -1;
        
    }
    (*keypointer)=malloc(sizeof(char)*(i+1));
    strncpy(*keypointer, string, i);
    (*keypointer)[i]='\0';
    return i;
}

int JSON_extract_value(char* string, char** valuepointer){
    int i,j,k;
    j=i=k=0;
    int isdecimal=0;
    int intvalue=0;
    double dvalue=0;
    while(string[i]!=':' && string[i]!='\0'){
        i++;
    }
    if(string[i]=='\0'){
        *valuepointer=NULL;
        return -1;
    }
    i++;
    j=i;
    while(string[j]!='\0' && string[j]!='}' && string[j]!=','){
        j++;
    }
    if(string[i] == '\'' || string[i] == '\"'){
        if(string[i]!=string[j-1]){
            printf("JSON can't be parsed : Incomplete string\n");
            *valuepointer=NULL;
            return -1;
        }else{
            (*valuepointer)=malloc(sizeof(char)*(j-i-1));
            strncpy(*valuepointer, string+i+1, j-i-2);
            (*valuepointer)[j]='\0';
            return STRING_TYPE;
        }
        
    }else{
        for(k=i;k<strlen(string);k++){
            if((string[k]<48 || string[k]>57)){
                if(string[k]=='.'){
                    isdecimal=1;
                }else{
                    printf("JSON can't be parsed : Stray char in number\n");
                    *valuepointer=NULL;
                    return -1;
                }
            }
        }
        if (isdecimal) {
            dvalue=strtof(string+i,NULL);
            (*valuepointer)=malloc(sizeof(dvalue));
            memcpy((*valuepointer), &dvalue, sizeof(dvalue));
            return DOUBLE_TYPE;
        } else {
            intvalue=atoi(string+i);
            *valuepointer=malloc(sizeof(int));
            memcpy(*valuepointer, &intvalue, sizeof(int));
            return INT_TYPE;
        }
    }
}

hashmap_t* hashmap_load(char * db){
    hashmap_t* hashmap=hashmap_create();
    FILE* fp;
    size_t len = 0;
    ssize_t read;
    char* line=NULL;
    fp = fopen(db, "r");
    if (fp == NULL)
        return NULL;
    
    while ((read = getline(&line, &len, fp)) != -1) {
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';
        list_value_t* addlist=JSON_parse(line);
        hashmap_add_list(hashmap,addlist);
    }
    fclose(fp);
    if (line)
        free(line);
    return hashmap;
}

void hashmap_save(hashmap_t* hashmap,char* db){
    hashmap_node_t* tmpnode;
    FILE* file=NULL;
    file=fopen(db, "w");
    int i;
    int create=1;
    record_list_t* recordlist=NULL;
    record_list_t* tmprecordlist=NULL;
    for(i=0;i<BUCKET_NUMBER;i++){
        tmpnode=hashmap->map[i];
        while(tmpnode!=NULL){
            if(create){
                recordlist=record_list_create(&(tmpnode->lt->record));
                create=0;
            }else{
                if((tmprecordlist=record_list_contains(recordlist, &tmpnode->lt->record))!=NULL){
                    record_list_append(tmprecordlist, &(tmpnode->lt->record));
                }
            }
            tmpnode=tmpnode->next;
        }
    }
    while (recordlist!=NULL) {
        writeDB(&file,recordlist->record);
        recordlist=recordlist->next;
    }
}