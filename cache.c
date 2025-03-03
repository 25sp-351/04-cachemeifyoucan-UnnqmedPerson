#include "cache.h"

// Function to create a cache
Cache* createCache(CachePolicy policy) {
    Cache *cache = (Cache*)malloc(sizeof(Cache));
    cache->head = cache->tail = NULL;
    cache->size = 0;
    cache->policy = policy;
    return cache;
}

// Function to free the cache
void freeCache(Cache *cache) {
    CacheEntry *entry = cache->head;
    while (entry) {
        CacheEntry *temp = entry;
        entry = entry->next;
        free(temp);
    }
    free(cache);
}

// Function to find an entry in cache
bool getCache(Cache *cache, int rodLength, int *maxValue) {
    CacheEntry *entry = cache->head;
    while (entry) {
        if (entry->rodLength == rodLength) {
            *maxValue = entry->maxValue;
            
            // If LRU, move accessed entry to front
            if (cache->policy == LRU && entry != cache->head) {
                if (entry == cache->tail) cache->tail = entry->prev;
                if (entry->prev) entry->prev->next = entry->next;
                if (entry->next) entry->next->prev = entry->prev;
                
                entry->next = cache->head;
                entry->prev = NULL;
                cache->head->prev = entry;
                cache->head = entry;
            }
            
            // If LFU, increase frequency count
            if (cache->policy == LFU) {
                entry->frequency++;
            }
            
            return true;
        }
        entry = entry->next;
    }
    return false;
}

// Function to insert into cache
void putCache(Cache *cache, int rodLength, int maxValue) {
    if (cache->size >= CACHE_SIZE) {
        // Eviction: Remove LRU or LFU entry
        CacheEntry *evict = cache->tail;
        
        if (cache->policy == LFU) {
            // Find the least frequently used entry
            CacheEntry *curr = cache->head;
            evict = curr;
            while (curr) {
                if (curr->frequency < evict->frequency)
                    evict = curr;
                curr = curr->next;
            }
        }
        
        if (evict->prev) evict->prev->next = evict->next;
        if (evict->next) evict->next->prev = evict->prev;
        if (evict == cache->head) cache->head = evict->next;
        if (evict == cache->tail) cache->tail = evict->prev;
        free(evict);
        cache->size--;
    }
    
    // Insert new entry at head
    CacheEntry *newEntry = (CacheEntry*)malloc(sizeof(CacheEntry));
    newEntry->rodLength = rodLength;
    newEntry->maxValue = maxValue;
    newEntry->frequency = 1;
    newEntry->next = cache->head;
    newEntry->prev = NULL;
    
    if (cache->head) cache->head->prev = newEntry;
    cache->head = newEntry;
    if (!cache->tail) cache->tail = newEntry;
    
    cache->size++;
}
