#define MAX_POINTS  4096
#include "geometry.h"

typedef struct {
    double x;
    double y;
    int parent;
    int rank;
} Point;


int find_or_create_point(double x, double y);
int uf_find(int i);
void uf_union(int a, int b);
void assign_nodes(void);
int match_point(double x, double y);

extern int node_id[MAX_POINTS];
extern int node_count;

