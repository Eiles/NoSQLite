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
#include "record_list.h"

int startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

int main(int argc, const char * argv[]) {
    if(argc<3){
        return 0;
    }
    char* collection=NULL;
    char* insert=NULL;
    char* find=NULL;
    char* set=NULL;
    char* where=NULL;
    char* project=NULL;
    char* remove=NULL;
    hashmap_t* hashmap=NULL;
    
    if (startsWith("-collection=",argv[1])) {
        char* c=strchr(argv[1], '=')+1;
        int len=strlen(c);
        if(len==0)
        return 0;
        collection=malloc(sizeof(char)*len+1);
        strncpy(collection, c, len);
        collection[len]='\0';
        printf("Collection is %s\n",collection);
    }else{
        printf("No collection name found\n");
        return 0;
    }
    
    if (startsWith("-insert=",argv[2])) {
        char* c=strchr(argv[2], '=')+1;
        int len=strlen(c);
        if(len==0)
            return 0;
        insert=malloc(sizeof(char)*len+1);
        strncpy(insert, c, len);
        insert[len]='\0';
        printf("Inserting : %s\n",insert);
        hashmap=hashmap_load(collection);
        if (hashmap==NULL) {
            printf("Couldn't load db, creating it\n");
        }else{
            hashmap=hashmap_create();
        }
        list_value_t* addlist=JSON_parse(insert);
        hashmap_add_list(hashmap,addlist);
        hashmap_save(hashmap, collection);
    }
    
    if(startsWith("-find=", argv[2])){
        if(!startsWith("-projection=", argv[3])){
            printf("No projection\n");
            return 0;
        }else{
            char* c=strchr(argv[3], '=')+1;
            int len=strlen(c);
            if(len==0)
                return 0;
            project=malloc(sizeof(char)*len+1);
            strncpy(project, c, len);
            project[len]='\0';
        }
        
        char* c=strchr(argv[2], '=')+1;
        int len=strlen(c);
        if(len==0)
            return 0;
        find=malloc(sizeof(char)*len+1);
        strncpy(find, c, len);
        find[len]='\0';
        hashmap=hashmap_load(collection);
        if (hashmap==NULL) {
            printf("Couldn't load db\n");
            return 0;
        }
        hashmap_node_t* node= hashmap_find(hashmap, find);
        list_value_t* list=JSON_parse(project);
        list_value_t* tmplist=list;
        int myargc=0;
        char** myargv;
        while(tmplist!=NULL){
            myargc++;
            tmplist=tmplist->next;
        }
        tmplist=list;
        myargv=malloc(sizeof(char*)*argc);
        int i=0;
        while (tmplist!=NULL) {
            myargv[i]=tmplist->key;
            i++;
            tmplist=tmplist->next;
        }
        
        hashmap_project(node, myargv, myargc);
        list_value_destroy_all(list);
        free(myargv);
        
        
    }
    hashmap_destroy(hashmap);
    return 0;
}
