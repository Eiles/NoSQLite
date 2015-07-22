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

int main(int argc, const char * argv[]) {
    hashmap_t* hashmap=hashmap_load("base.db");
    hashmap_node_t* node=NULL;
    char* args[2];
    
    node=hashmap_find(hashmap, "grade");
    hashmap_where(&node,"{grade:13}");
    
    args[0]="grade";
    hashmap_project(node, args, 1);
    hashmap_destroy(hashmap);
    return 0;
}
