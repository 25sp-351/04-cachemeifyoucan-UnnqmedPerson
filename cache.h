#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define cache size
#define CACHE_SIZE 10  

// Structure for cache entry
typedef struct CacheEntry {
    int rodLength;   // Key: Rod length
    int maxValue;    // Computed max value for this rod length
    int frequency;   // Frequency counter (used for LFU)
    struct CacheEntry *prev, *next;  // Linked list pointers (for LRU)
} CacheEntry;

// Cache type to define LRU or LFU mode
typedef enum { LRU, LFU } CachePolicy;

// Cache structure
typedef struct {
    CacheEntry *head, *tail;  // Pointers to the linked list (for LRU)
    int size;
    CachePolicy policy;
} Cache;

// Function prototypes
Cache* createCache(CachePolicy policy);
void freeCache(Cache *cache);
bool getCache(Cache *cache, int rodLength, int *maxValue);
void putCache(Cache *cache, int rodLength, int maxValue);

#endif // CACHE_H
