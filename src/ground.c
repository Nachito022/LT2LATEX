/* ground.c */
#include "ground.h"
#include <stdio.h>
#include <stdlib.h>

Ground grounds[MAX_GROUNDS];
int ground_count = 0;

void add_ground(double x, double y)
{
    if (ground_count >= MAX_GROUNDS) {
        fprintf(stderr, "Too many grounds\n");
        exit(1);
    }
    grounds[ground_count].x = x;
    grounds[ground_count].y = y;
    ground_count++;
}