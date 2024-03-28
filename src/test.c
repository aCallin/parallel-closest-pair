#include "test.h"

void correctness_tests() {
    printf("[CORRECTNESS TESTS]\n");
    int n[] = {2, 4, 8, 16, 2048};
    for (int i = 0; i < sizeof(n) / sizeof(n[0]); i++) {
        int current_n = n[i];
        printf("n: %d\n", current_n);

        // Generate points
        struct Point *points = malloc(current_n * sizeof(struct Point));
        generate_points(points, current_n);
        printf("Points:\n");
        if (current_n <= 16)
            print_points(points, current_n);

        // Solve with brute force approach
        struct Pair a = closest_pair_naive(points, current_n);
        printf("(Brute force) p1: {%7.2f, %7.2f}, p2: {%7.2f, %7.2f}, distance: %.5f\n", a.p1.x, a.p1.y, a.p2.x, a.p2.y, a.distance);

        // Solve with serial divide-and-conquer approach
        struct Pair b = closest_pair_dnc_serial(points, current_n);
        printf("(Serial divide-and-conquer) p1: {%7.2f, %7.2f}, p2: {%7.2f, %7.2f}, distance: %.5f\n", b.p1.x, b.p1.y, b.p2.x, b.p2.y, b.distance);

        // Solve with parallel divide-and-conquer approach
        struct Pair c = closest_pair_dnc_parallel(points, current_n);
        printf("(Parallel divide-and-conquer) p1: {%7.2f, %7.2f}, p2: {%7.2f, %7.2f}, distance: %.5f\n", c.p1.x, c.p1.y, c.p2.x, c.p2.y, c.distance);

        // Check that serial and parallel divide and conquer approaches give the same result as the naive approach. The points don't need to be the same as long as the distance is
        printf("Distances are equal: %s\n", (a.distance == b.distance && b.distance == c.distance) ? "true" : "false");

        free(points);
        printf("\n");
    }
}

void performance_tests() {
    // Brute force approach
    printf("[PERFORMANCE TESTS - BRUTE FORCE]\n");
    int n_naive[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384};
    for (int i = 0; i < sizeof(n_naive) / sizeof(n_naive[0]); i++) {
        int current_n = n_naive[i];
        printf("n: %d\n", current_n);
        
        // Generate points
        struct Point *points = malloc(current_n * sizeof(struct Point));
        generate_points(points, current_n);

        // Brute force
        ctimer_t timer_naive;
        ctimer_start(&timer_naive);
        closest_pair_naive(points, current_n);
        ctimer_stop(&timer_naive);
        ctimer_measure(&timer_naive);
        ctimer_print(timer_naive, "brute force");

        free(points);
        printf("\n");
    }

    printf("\n");

    // Divide-and-conquer approach
    printf("[PERFORMANCE TESTS - DIVIDE AND CONQUER]\n");
    int n_dnc[] = {65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216};
    for (int i = 0; i < sizeof(n_dnc) / sizeof(n_dnc[0]); i++) {
        int current_n = n_dnc[i];
        printf("n: %d\n", current_n);
        
        // Generate points
        struct Point *points = malloc(current_n * sizeof(struct Point));
        generate_points(points, current_n);

        // Serial
        ctimer_t timer_serial;
        ctimer_start(&timer_serial);
        closest_pair_dnc_serial(points, current_n);
        ctimer_stop(&timer_serial);
        ctimer_measure(&timer_serial);
        ctimer_print(timer_serial, "serial");

        // Parallel
        ctimer_t timer_parallel;
        ctimer_start(&timer_parallel);
        closest_pair_dnc_parallel(points, current_n);
        ctimer_stop(&timer_parallel);
        ctimer_measure(&timer_parallel);
        ctimer_print(timer_parallel, "parallel");

        free(points);
        printf("\n");
    }
}
