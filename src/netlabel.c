#include "netlabel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NetLabel netlabels[MAX_NETLABELS];
int      netlabel_count = 0;

void add_netlabel(double x, double y, const char *name)
{
    if (netlabel_count >= MAX_NETLABELS) {
        fprintf(stderr, "Too many net labels\n");
        exit(1);
    }
    netlabels[netlabel_count].x = x;
    netlabels[netlabel_count].y = y;
    strncpy(netlabels[netlabel_count].name, name,
            sizeof(netlabels[netlabel_count].name) - 1);
    netlabels[netlabel_count].name[
        sizeof(netlabels[netlabel_count].name) - 1] = '\0';
    netlabel_count++;
}