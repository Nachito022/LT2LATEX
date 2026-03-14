/* Graph functions */

#include "graph.h"
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>

Point points[MAX_POINTS];
int   point_count = 0;

Edge  edges[MAX_EDGES];
int   edge_count = 0;

int node_id[MAX_POINTS];
int node_count = 0;

int find_or_create_point(double x, double y)
{
    for (int i = 0; i < point_count; i++)
        if (double_equal(points[i].x, x) && double_equal(points[i].y, y))
            return i;

    if (point_count >= MAX_POINTS) {
        fprintf(stderr, "Too many points\n");
        exit(1);
    }

    points[point_count].x      = x;
    points[point_count].y      = y;
    points[point_count].parent = point_count;
    points[point_count].rank   = 0;

    return point_count++;
}

void add_edge(int a, int b)
{
    if (edge_count >= MAX_EDGES) {
        fprintf(stderr, "Too many edges\n");
        exit(1);
    }
    /* Avoid duplicate edges */
    for (int i = 0; i < edge_count; i++)
        if ((edges[i].a == a && edges[i].b == b) ||
            (edges[i].a == b && edges[i].b == a))
            return;

    edges[edge_count].a = a;
    edges[edge_count].b = b;
    edge_count++;
}

int uf_find(int i)
{
    if (points[i].parent != i)
        points[i].parent = uf_find(points[i].parent);
    return points[i].parent;
}

void uf_union(int a, int b)
{
    int rootA = uf_find(a);
    int rootB = uf_find(b);
    if (rootA == rootB) return;

    if      (points[rootA].rank < points[rootB].rank) points[rootA].parent = rootB;
    else if (points[rootA].rank > points[rootB].rank) points[rootB].parent = rootA;
    else { points[rootB].parent = rootA; points[rootA].rank++; }
}

void assign_nodes(void)
{
    for (int i = 0; i < point_count; i++) {
        int root  = uf_find(i);
        int found = -1;
        for (int j = 0; j < i; j++) {
            if (uf_find(j) == root) { found = node_id[j]; break; }
        }
        node_id[i] = (found == -1) ? node_count++ : found;
    }
}

int match_point(double x, double y)
{
    for (int i = 0; i < point_count; i++)
        if (double_equal(points[i].x, x) && double_equal(points[i].y, y))
            return i;
    return -1;
}