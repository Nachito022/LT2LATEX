#ifndef WIRE_H
#define WIRE_H

#include "geometry.h"

#define MAX_WIRES   4096

typedef struct {
    Vector p1;
    Vector p2;
    int point1_index;
    int point2_index;
} Wire;

extern Wire wires[MAX_WIRES];
extern int wire_count;

void add_wire(double x1, double y1, double x2, double y2);

#endif