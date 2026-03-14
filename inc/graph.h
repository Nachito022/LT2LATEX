#ifndef GRAPH_H
#define GRAPH_H

#define MAX_POINTS  4096
#define MAX_EDGES   8192

#include "geometry.h"

typedef struct {
    double x;
    double y;
    int parent;
    int rank;
} Point;

typedef struct {
    int a;      /* index of first point  */
    int b;      /* index of second point */
} Edge;

extern Point points[MAX_POINTS];
extern int   point_count;

extern Edge  edges[MAX_EDGES];
extern int   edge_count;

extern int node_id[MAX_POINTS];
extern int node_count;

int  find_or_create_point(double x, double y);
void add_edge(int a, int b);
int  uf_find(int i);
void uf_union(int a, int b);
void assign_nodes(void);
int  match_point(double x, double y);

#endif