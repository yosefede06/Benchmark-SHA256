#include <stdio.h>
#include <CommonCrypto/CommonDigest.h>
#include <time.h>

#define DATA_SIZE 32  // Size of data to hash
#define NUM_HASHES (100000000)  // Total number of hashes to compute

// Function to compute SHA-256 hashes
void compute_hashes(long num_hashes, unsigned char *data) {
    unsigned char hash[CC_SHA256_DIGEST_LENGTH];

    for (long i = 0; i < num_hashes; i++) {
        CC_SHA256(data, DATA_SIZE, hash);  // Compute SHA-256 hash
    }
}

// Function to get the current time
double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Use CLOCK_MONOTONIC to avoid time jumps
    return ts.tv_sec + ts.tv_nsec * 1e-9;  // Convert to seconds
}

int main() {
    unsigned char data[DATA_SIZE] = "Benchmarking hash performance";

    printf("Running SHA-256 benchmark on a single core...\n");

    // Start time
    double start_time = get_time_in_seconds();

    // Compute hashes on a single core
    compute_hashes(NUM_HASHES, data);

    // End time
    double end_time = get_time_in_seconds();
    double elapsed_time = end_time - start_time;

    // Calculate hashes per second
    double hashes_per_second = NUM_HASHES / elapsed_time;

    printf("Computed %ld hashes in %.2f seconds\n", NUM_HASHES, elapsed_time);
    printf("Hashes per second: %.2f\n", hashes_per_second);

    return 0;
}