/* Graph functions for handling point management and union-find operations */

#include "graph.h"
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>

Point points[MAX_POINTS];
int point_count = 0;

int node_id[MAX_POINTS];
int node_count = 0;

int find_or_create_point(double x, double y)
{
    //go through the current list of points, and check if it exists
    for (int i = 0; i < point_count; i++) {
        if (double_equal(points[i].x, x) && double_equal(points[i].y, y))
            return i;
    }

    //before adding a point, check if list is not full
    if (point_count >= MAX_POINTS) {
        fprintf(stderr, "Too many points\n");
        exit(1);
    }

    //Add point
    points[point_count].x = x;
    points[point_count].y = y;
    points[point_count].parent = point_count;
    points[point_count].rank = 0;

    return point_count++;
}

int uf_find(int i)
{
    //recursively finds the root
    //Makes every visited node directly point to the root, so points[i].parent always points at the root
    if (points[i].parent != i)
        points[i].parent = uf_find(points[i].parent);

    //Returns the root index
    return points[i].parent;
}

void uf_union(int a, int b)
{
    //Finds the root of each tree
    int rootA = uf_find(a);
    int rootB = uf_find(b);

    //If already in the same tree, do nothing
    if (rootA == rootB)
        return;

    //Union by rank:
    //Attach the shorter tree under the taller one
    if (points[rootA].rank < points[rootB].rank)
        points[rootA].parent = rootB;

    //If the other one is smaller
    else if (points[rootA].rank > points[rootB].rank)
        points[rootB].parent = rootA;

    //If equal rank, pick root A and increase its rank
    else {
        points[rootB].parent = rootA;
        points[rootA].rank++;
    }
}

void assign_nodes()
{
    //Assigns a unique node_id to each connected component
    for (int i = 0; i < point_count; i++) {
        //Find root
        int root = uf_find(i);

        int found = -1;
        for (int j = 0; j < i; j++) {
            //If another point with the same root already got a node_id
            if (uf_find(j) == root) {
                //Assigns the same node_id
                found = node_id[j];
                break;
            }
        }

        //If another point with the same root does not have a node_id,
        //it assigns a new node_id using node_count++

        if (found == -1) {
            node_id[i] = node_count++;
        } 
        else {
            node_id[i] = found;
        }
    }
}

int match_point(double x, double y)
{
    for (int i = 0; i < point_count; i++) {
        if (double_equal(points[i].x, x) && double_equal(points[i].y, y))
            return i;
    }
    return -1;
}
