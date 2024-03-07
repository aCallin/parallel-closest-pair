#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cilk/cilk.h>

struct Point {
    float x;
    float y;
};

struct Pair {
    struct Point p1;
    struct Point p2;
    float distance;
};

void print_point(struct Point point);
void print_points(struct Point *points, int n);
void generate_points(struct Point *points, int n);
float distance(struct Point p1, struct Point p2);
float min(float a, float b);
int sort_by_x(const void *a, const void *b);
int sort_by_y(const void *a, const void *b);
struct Pair closest_pair_dnc_serial(struct Point *points, int n);
struct Pair closest_pair_dnc_parallel(struct Point *points, int n);

#endif
