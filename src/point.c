#include "point.h"

void print_point(struct Point point) {
    printf("{%7.2f, %7.2f}\n", point.x, point.y);
}

void print_points(struct Point *points, int n) {
    for (int i = 0; i < n; i++) {
        print_point(points[i]);
    }
}

void generate_points(struct Point *points, int n) {
    for (int i = 0; i < n; i++) {
        float x = ((rand() % 10000) + 1) / 10.0f; // Values between 1/10 and 1000
        float y = ((rand() % 10000) + 1) / 10.0f;
        points[i].x = x;
        points[i].y = y;
    }
}

float distance(struct Point p1, struct Point p2) {
    return hypot((p2.x - p1.x), (p2.y - p1.y));
}

float min(float a, float b) {
    return (a < b) ? a : b;
}

int sort_by_x(const void *a, const void *b) {
    struct Point p1 = *((struct Point *)a);
    struct Point p2 = *((struct Point *)b);
    return (p1.x > p2.x) ? 1 : -1;
}

int sort_by_y(const void *a, const void *b) {
    struct Point p1 = *((struct Point *)a);
    struct Point p2 = *((struct Point *)b);
    return (p1.y > p2.y) ? 1 : -1;
}

struct Pair closest_pair_naive(struct Point *points, int n) {
    struct Pair closest;
    closest.p1 = points[0];
    closest.p2 = points[1];
    closest.distance = distance(points[0], points[1]);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j != i) {
                float current_distance = distance(points[i], points[j]);
                if (current_distance < closest.distance) {
                    closest.p1 = points[i];
                    closest.p2 = points[j];
                    closest.distance = current_distance;
                }
            }
        }
    }

    return closest;
}

struct Pair closest_pair_dnc_serial(struct Point *points, int n) {
    // Base case
    if (n == 2) {
        struct Pair closest;
        closest.p1 = points[0];
        closest.p2 = points[1];
        closest.distance = distance(points[0], points[1]);
        return closest;
    }

    // Recursive case

    // Sort the points by x and and recurse on left and right half
    struct Point *points_sorted = malloc(n * sizeof(struct Point));
    memcpy(points_sorted, points, n * sizeof(struct Point));
    qsort(points_sorted, n, sizeof(struct Point), sort_by_x);
    struct Point *points_left = malloc((n / 2) * sizeof(struct Point));
    struct Point *points_right = malloc((n / 2) * sizeof(struct Point));
    memcpy(points_left, points_sorted, (n / 2) * sizeof(struct Point));
    memcpy(points_right, (points_sorted + (n / 2)), (n / 2) * sizeof(struct Point));
    struct Pair closest_left = closest_pair_dnc_serial(points_left, n / 2);
    struct Pair closest_right = closest_pair_dnc_serial(points_right, n / 2);
    struct Pair closest;
    closest = (closest_left.distance < closest_right.distance) ? closest_left : closest_right;

    // Get the points split up the middle and sort them by y
    float middle_x = (points_sorted[(n / 2) - 1].x + points_sorted[n / 2].x) / 2;
    float middle_x_min = middle_x - closest.distance;
    float middle_x_max = middle_x + closest.distance;
    struct Point *points_split = malloc(n * sizeof(struct Point));
    int points_split_length = 0;
    for (int i = 0; i < (n / 2); i++) {
        if (points_left[i].x >= middle_x_min && points_left[i].x <= middle_x_max) {
            points_split[points_split_length] = points_left[i];
            points_split_length++;
        }
        if (points_right[i].x >= middle_x_min && points_right[i].x <= middle_x_max) {
            points_split[points_split_length] = points_right[i];
            points_split_length++;
        }
    }
    struct Point *points_split_sorted = malloc(points_split_length * sizeof(struct Point));
    memcpy(points_split_sorted, points_split, points_split_length * sizeof(struct Point));
    qsort(points_split_sorted, points_split_length, sizeof(struct Point), sort_by_y);

    // Compute distance to six subsequent points
    for (int i = 0; i < points_split_length; i++) {
        for (int j = i + 1; j < points_split_length && (points_split_sorted[j].y - points_split_sorted[i].y) < closest.distance; j++) {
            if (distance(points_split_sorted[i], points_split_sorted[j]) < closest.distance) {
                closest.p1 = points_split_sorted[i];
                closest.p2 = points_split_sorted[j];
                closest.distance = distance(points_split_sorted[i], points_split_sorted[j]);
            }
        }
    }

    // Clean up
    free(points_split_sorted);
    free(points_split);
    free(points_right);
    free(points_left);
    free(points_sorted);

    // Result
    return closest;
}

struct Pair closest_pair_dnc_parallel(struct Point *points, int n) {
    // Base case
    if (n == 2) {
        struct Pair closest;
        closest.p1 = points[0];
        closest.p2 = points[1];
        closest.distance = distance(points[0], points[1]);
        return closest;
    }

    // Recursive case

    // Sort the points by x and and recurse on left and right half
    struct Point *points_sorted = malloc(n * sizeof(struct Point));
    memcpy(points_sorted, points, n * sizeof(struct Point));
    qsort(points_sorted, n, sizeof(struct Point), sort_by_x);
    struct Point *points_left = malloc((n / 2) * sizeof(struct Point));
    struct Point *points_right = malloc((n / 2) * sizeof(struct Point));
    memcpy(points_left, points_sorted, (n / 2) * sizeof(struct Point));
    memcpy(points_right, (points_sorted + (n / 2)), (n / 2) * sizeof(struct Point));
    struct Pair closest_left;
    cilk_spawn closest_left = closest_pair_dnc_parallel(points_left, n / 2);
    struct Pair closest_right = closest_pair_dnc_parallel(points_right, n / 2);
    cilk_sync;
    struct Pair closest;
    closest = (closest_left.distance < closest_right.distance) ? closest_left : closest_right;

    // Get the points split up the middle and sort them by y
    float middle_x = (points_sorted[(n / 2) - 1].x + points_sorted[n / 2].x) / 2;
    float middle_x_min = middle_x - closest.distance;
    float middle_x_max = middle_x + closest.distance;
    struct Point *points_split = malloc(n * sizeof(struct Point));
    int points_split_length = 0;
    for (int i = 0; i < (n / 2); i++) {
        if (points_left[i].x >= middle_x_min && points_left[i].x <= middle_x_max) {
            points_split[points_split_length] = points_left[i];
            points_split_length++;
        }
        if (points_right[i].x >= middle_x_min && points_right[i].x <= middle_x_max) {
            points_split[points_split_length] = points_right[i];
            points_split_length++;
        }
    }
    struct Point *points_split_sorted = malloc(points_split_length * sizeof(struct Point));
    memcpy(points_split_sorted, points_split, points_split_length * sizeof(struct Point));
    qsort(points_split_sorted, points_split_length, sizeof(struct Point), sort_by_y);

    // Compute distance to six subsequent points
    for (int i = 0; i < points_split_length; i++) {
        for (int j = i + 1; j < points_split_length && (points_split_sorted[j].y - points_split_sorted[i].y) < closest.distance; j++) {
            if (distance(points_split_sorted[i], points_split_sorted[j]) < closest.distance) {
                closest.p1 = points_split_sorted[i];
                closest.p2 = points_split_sorted[j];
                closest.distance = distance(points_split_sorted[i], points_split_sorted[j]);
            }
        }
    }

    // Clean up
    free(points_split_sorted);
    free(points_split);
    free(points_right);
    free(points_left);
    free(points_sorted);

    // Result
    return closest;
}
