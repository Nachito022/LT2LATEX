#include "wire.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

Wire wires[MAX_WIRES];
int  wire_count = 0;

void add_wire(double x1, double y1, double x2, double y2)
{
    if (wire_count >= MAX_WIRES) {
        fprintf(stderr, "Too many wires\n");
        exit(1);
    }

    x1 *=  SCALE; y1 *= -SCALE;
    x2 *=  SCALE; y2 *= -SCALE;

    int p1 = find_or_create_point(x1, y1);
    int p2 = find_or_create_point(x2, y2);

    uf_union(p1, p2);
    add_edge(p1, p2);          /* <-- store the edge explicitly */

    wires[wire_count].p1.x          = x1;
    wires[wire_count].p1.y          = y1;
    wires[wire_count].p2.x          = x2;
    wires[wire_count].p2.y          = y2;
    wires[wire_count].point1_index  = p1;
    wires[wire_count].point2_index  = p2;
    wire_count++;
}