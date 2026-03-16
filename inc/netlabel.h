#ifndef NETLABEL_H
#define NETLABEL_H

#define MAX_NETLABELS 128

typedef struct {
    double x, y;
    char   name[64];
} NetLabel;

extern NetLabel netlabels[MAX_NETLABELS];
extern int      netlabel_count;

void add_netlabel(double x, double y, const char *name);

#endif