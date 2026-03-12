/* ground.h */
#ifndef GROUND_H
#define GROUND_H

#define MAX_GROUNDS 64

typedef struct {
    double x, y;
} Ground;

extern Ground grounds[MAX_GROUNDS];
extern int ground_count;

void add_ground(double x, double y);

#endif