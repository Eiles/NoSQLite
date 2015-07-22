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
    hashmap_node_t* node=NULL;
    
    
//    node=hashmap_find(hashmap, "Elie");
    char* args[2];
//    args[0]="grade";
//    args[1]="name";
//    hashmap_project(node, args, 2);
    list_value_t* addlist=JSON_parse("{name:'Paul',machin:'chose',grade:2020}");
    hashmap_add_list(hashmap,addlist);
    addlist=JSON_parse("{Elie:10.5,grade:20,name:'Elie'}");
    hashmap_add_list(hashmap,addlist);
    addlist=JSON_parse("{Tom:13.5,grade:13,name:'Tom'}");
    hashmap_add_list(hashmap,addlist);
    node=hashmap_find(hashmap, "grade");
    
    hashmap_where(&node,"{machin:13}");
    
    args[0]="grade";
    hashmap_project(node, args, 1);
    hashmap_destroy(hashmap);
    return 0;
}
