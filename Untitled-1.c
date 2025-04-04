#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

typedef struct {
    int cache_hits;
    int cache_misses;
    float hit_rate;
    float miss_rate;
    float avg_access_time;
    float cpu_utilization;
    float memory_utilization; 
} PerformanceMetrics;

typedef struct {
    unsigned int tag;
    int valid;
    int last_accessed;
} CacheLine;

typedef struct {
    CacheLine *lines;
    int size;
    int hits;
    int misses;
    int access_time;
}Cache;

Cache* initCache(int size) {
    Cache *cache = malloc(sizeof(Cache));
    cache->lines = malloc(size * sizeof(CacheLine));
    cache->size = size;
    cache->hits = 0;
    cache->misses = 0;
    cache->access_time = 0;

    for (int i = 0; i < size; i++) {
        cache->lines[i].valid = 0; 
    }
    return cache;
}

void accessCache(Cache *cache, unsigned int address) {
    unsigned int index = address % cache->size; 
    unsigned int tag = address / cache->size;
    cache->access_time++;

    if (cache->lines[index].valid && cache->lines[index].tag == tag) {
        cache->hits++;
        cache->lines[index].last_accessed = cache->access_time; 
    } else {
        cache->misses++;
        cache->lines[index].tag = tag; 
        cache->lines[index].valid = 1;  
        cache->lines[index].last_accessed = cache->access_time; 
    }
}

int searchCache(Cache *cache, unsigned int address) {
    unsigned int index = address % cache->size; 
    unsigned int tag = address / cache->size;

    if (cache->lines[index].valid && cache->lines[index].tag == tag) {
        return 1; 
    }
    return 0; 
}

void displayStats(Cache *cache) {
    printf("Hits: %d, Misses: %d, Hit Rate: %.2f%%\n", 
           cache->hits, cache->misses, 
           (float)cache->hits / (cache->hits + cache->misses) * 100);
    printf("Average Access Time: %.2f\n", (float)cache->access_time / (cache->hits + cache->misses));
}

void freeCache(Cache *cache) {
    free(cache->lines);
    free(cache);
}

float get_cpu_utilization() {
    FILETIME idleTime, kernelTime, userTime;
    float cpu_utilization = 0.0;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    ULONGLONG total_time = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
    total_time += ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;
    ULONGLONG idle_time = ((ULONGLONG)idleTime.dwHighDateTime << 32) | idleTime.dwLowDateTime;
    cpu_utilization = (float)(total_time - idle_time) / total_time;
    return cpu_utilization;
}

void writePerformanceMetrics(Cache *cache, PerformanceMetrics *metrics, int time) {
    FILE *fp = fopen("performance_data.csv", "a");
    if (fp != NULL) {
            fprintf(fp, "Time,Cache Hits,Cache Misses,Hit Rate,Miss Rate,Average Access Time,CPU Utilization,Memory Utilization\n");

        metrics->hit_rate = (float)cache->hits / (cache->hits + cache->misses);
        metrics->miss_rate = (float)cache->misses / (cache->hits + cache->misses);
        metrics->avg_access_time = (float)cache->access_time / (cache->hits + cache->misses);
        metrics->cpu_utilization = get_cpu_utilization();
        metrics->memory_utilization = 0.0; 

        fprintf(fp, "%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
                time, cache->hits, 
                cache->misses, metrics->hit_rate * 100, 
                metrics->miss_rate * 100, metrics->avg_access_time, 
                metrics->cpu_utilization * 100, metrics->memory_utilization);
        fclose(fp);
    } else {
        perror("Failed to open performance_data.csv");
    }
}

int main() {
    srand(time(NULL));
    int cache_size;
    printf("Enter cache size: ");
    scanf("%d", &cache_size); 
    Cache *cache = initCache(cache_size);
    PerformanceMetrics metrics = {0};

    for (int i = 0; i < 1000; i++) {
        unsigned int address = rand() % (int) cache_size*2; 
        accessCache(cache, address);
    }

    displayStats(cache);

    unsigned int search_address;
    printf("Enter a cache line address to search: ");
    scanf("%u", &search_address);
    
    if (searchCache(cache, search_address)) {
        printf("Cache hit for address: %u\n", search_address);
    } else {
        printf("Cache miss for address: %u\n", search_address);
    }

    writePerformanceMetrics(cache, &metrics, 1);
    freeCache(cache);
    return 0;
}