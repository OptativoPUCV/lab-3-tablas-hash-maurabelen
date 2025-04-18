#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if(searchMap(map, key) !=NULL) return;
    long index = hash(key, map->capacity);
    //long orgIndex = index;
    while(map->buckets[index] !=NULL && map->buckets[index]->key != NULL){
            index = (index + 1) % map-> capacity;
        }

    if(map->buckets[index] ==NULL){
        map->buckets[index] = createPair(strdup(key), value);
    }
    else{
        map->buckets[index]->key = strdup(key);
        map->buckets[index]->value = value;
         
    }
    map->size++;
}

void enlarge(HashMap * map) {
    //enlarge_called = 1; //no borrar (testing purposes)
    if(map == NULL) return;

    //guardar arreglo antiguo
    Pair **old_buckets = map ->buckets;
    long old_capacity = map->capacity;

    //duplicamos capacidad 
    map->capacity *= 2;

    //creamos nuevo arreglo de buckets
    map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
    if(map->buckets == NULL){
        map->buckets = old_buckets;
        map->capacity = old_capacity;
        return;
    }

    //reiniciamos tamaño
    map->size = 0;

    //reinsertamos elementos al nuevo arreglo
    long index = 0;
    while(index < old_capacity){
        Pair *pair = old_buckets[index];
        if(pair != NULL && pair->key != NULL){
            insertMap(map, pair->key, pair->value);
        }
        index++;
    }
}


HashMap * createMap(long capacity) {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    if(map == NULL) return NULL;
    
    map->buckets = (Pair**)calloc(capacity,sizeof(Pair*));
    if(map->buckets == NULL) return NULL;

    map->current = -1;
    map->capacity = capacity;
    map->size = 0;

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    if(map == NULL  || key == NULL) return;

    long index = hash(key, map-> capacity);
    long pos = 0;

    while(map->buckets[index] != NULL){
        Pair *pair = map->buckets[index];

        if(pair->key != NULL && strcmp(pair->key, key) == 0){
            pair->key = NULL;
            map->size--;
            return;
        }
        index = (index+1) % map-> capacity;
        pos++;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    if(map == NULL || key == NULL) return NULL;
    long index = hash(key, map->capacity);

    Pair *pair = map->buckets[index];

    while(pair != NULL){
        if(pair->key != NULL && strcmp(pair->key, key) == 0){
            map->current = index;
            return pair;
        }
        index = (index + 1) % map->capacity;
        pair = map->buckets[index];
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {
if(map == NULL) return NULL;
//long index = map->current + 1;

long index = 0;
while(index < map->capacity){
    Pair * pair = map->buckets[index];
    if(pair != NULL && pair->key != NULL){
        map->current = index;
        return pair;
    }
    index++;
}
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if(map == NULL) return NULL;
    long index = map->current + 1;
    while(index < map->capacity){
        Pair * pair= map->buckets[index];
        if(pair != NULL && pair->key != NULL){
            map->current = index;
            return pair;
        }
        index++;
    }
    return NULL;
}
