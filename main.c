#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

#define MAX_PIECES 100

typedef struct {
    int length;
    int value;
} Piece;

// Global cache pointer
Cache *cache;

// Function to solve rod cutting with caching
int solveRodCutting(int rodLength, Piece *pieces, int pieceCount) {
    int maxValue;

    // Check cache first
    if (getCache(cache, rodLength, &maxValue)) {
        return maxValue;
    }

    // Initialize DP
    int dp[rodLength + 1];
    memset(dp, 0, sizeof(dp));

    for (int ii = 1; ii <= rodLength; ii++) {
        for (int jj = 0; jj < pieceCount; jj++) {
            if (pieces[jj].length <= ii) {
                int candidate = dp[ii - pieces[jj].length] + pieces[jj].value;
                if (candidate > dp[ii]) {
                    dp[ii] = candidate;
                }
            }
        }
    }

    // Store result in cache
    putCache(cache, rodLength, dp[rodLength]);
    return dp[rodLength];
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <cache_policy>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *policyStr = argv[2];
    
    // Check and set cache policy
    CachePolicy policy;
    if (strcmp(policyStr, "LRU") == 0) {
        policy = LRU;
    } else if (strcmp(policyStr, "LFU") == 0) {
        policy = LFU;
    } else {
        fprintf(stderr, "Invalid cache policy. Use 'LRU' or 'LFU'.\n");
        return 1;
    }

    // Initialize cache
    cache = createCache(policy);

    // Read (length, value) pairs from file
    Piece pieces[MAX_PIECES];
    int pieceCount = 0;

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    while (fscanf(file, "%d %d", &pieces[pieceCount].length, &pieces[pieceCount].value) == 2) {
        if (++pieceCount >= MAX_PIECES) break;
    }
    fclose(file);

    // Process input rod lengths and values
    int rodLength, rodValue;
    while (scanf("%d %d", &rodLength, &rodValue) == 2) {
        int maxValue = solveRodCutting(rodLength, pieces, pieceCount);
        printf("Rod Length: %d, Expected Value: %d, Computed Max Value: %d\n", rodLength, rodValue, maxValue);
    }

    freeCache(cache);
    return 0;
}
