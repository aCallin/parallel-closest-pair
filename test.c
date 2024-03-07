#include "test.h"

void correctness_tests() {
    printf("[CORRECTNESS TESTS]\n");

    int n[3] = {2, 4, 8};
    for (int i = 0; i < 3; i++) {
        int current_n = n[i];
        printf("n: %d\n", current_n);

        struct Point *points = malloc(current_n * sizeof(struct Point));
        generate_points(points, current_n);
        printf("Points:\n");
        print_points(points, current_n);

        struct Pair closest = closest_pair_dnc_parallel(points, current_n);
        printf("Closest:\n");
        printf("{%7.2f, %7.2f} and {%7.2f, %7.2f}, distance: %.2f\n", closest.p1.x, closest.p1.y, closest.p2.x, closest.p2.y, closest.distance);

        free(points);
    }
}

void performance_tests() {
    printf("[PERFORMANCE TESTS]\n");

    int n[6] = {65536, 131072, 262144, 524288, 1048576, 2097152};
    for (int i = 0; i < 6; i++) {
        int current_n = n[i];
        printf("n: %d\n", current_n);
        
        // Randomly generate points to work with
        struct Point *points = malloc(current_n * sizeof(struct Point));
        generate_points(points, current_n);

        // Divide and conqure approach (serial)
        ctimer_t timer_serial;
        printf("Serial closest point... ");
        ctimer_start(&timer_serial);
        closest_pair_dnc_serial(points, current_n);
        ctimer_stop(&timer_serial);
        printf("done\n");
        ctimer_measure(&timer_serial);
        ctimer_print(timer_serial, "serial");

        // Divide and conqure approach (parallel)
        ctimer_t timer_parallel;
        printf("Parallel closest point... ");
        ctimer_start(&timer_parallel);
        closest_pair_dnc_parallel(points, current_n);
        ctimer_stop(&timer_parallel);
        printf("done\n");
        ctimer_measure(&timer_parallel);
        ctimer_print(timer_parallel, "parallel");

        free(points);
    }
}
