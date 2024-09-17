#include <stdio.h>
#include <pthread.h>
#include <CommonCrypto/CommonDigest.h>
#include <time.h>
#include <unistd.h>  // For sysconf

#define DATA_SIZE 32  // Size of data to hash
#define NUM_HASHES (100000000)  // Total number of hashes to compute

// Structure to pass data to threads
typedef struct {
    unsigned char *data;
    long num_hashes;
} thread_data_t;

// Function to compute SHA-256 hashes
void* compute_hashes(void *arg) {
    thread_data_t *tdata = (thread_data_t *)arg;
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];

    for (long i = 0; i < tdata->num_hashes; i++) {
        CC_SHA256(tdata->data, DATA_SIZE, hash);  // Compute SHA-256 hash
    }

    return NULL;
}

// Function to get the number of available CPU cores
int get_cpu_cores() {
    return sysconf(_SC_NPROCESSORS_ONLN);  // Returns the number of available processors on macOS
}

// Function to get the current time
double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Use CLOCK_MONOTONIC to avoid time jumps
    return ts.tv_sec + ts.tv_nsec * 1e-9;  // Convert to seconds
}

int main() {
    unsigned char data[DATA_SIZE] = "Benchmarking hash performance";
    int num_threads = get_cpu_cores();
    pthread_t threads[num_threads];
    thread_data_t tdata[num_threads];
    long hashes_per_thread = NUM_HASHES / num_threads;

    printf("Running SHA-256 benchmark using %d threads...\n", num_threads);

    // Start time
    double start_time = get_time_in_seconds();

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        tdata[i].data = data;
        tdata[i].num_hashes = hashes_per_thread;
        pthread_create(&threads[i], NULL, compute_hashes, &tdata[i]);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // End time
    double end_time = get_time_in_seconds();
    double elapsed_time = end_time - start_time;

    // Calculate hashes per second
    double hashes_per_second = NUM_HASHES / elapsed_time;

    printf("Computed %ld hashes in %.2f seconds\n", NUM_HASHES, elapsed_time);
    printf("Hashes per second: %.2f\n", hashes_per_second);

    return 0;
}