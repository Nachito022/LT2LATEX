#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_PINS 16
#define MAX_COMPONENTS 512
#define MAX_POINTS 4096

#include "geometry.h"

typedef struct {
    char name[32];
    Vector local_pos;
    Vector abs_pos;
    int node;
} Pin;

typedef struct {
    char name[64];
    char type[64];
    Vector position;
    char orient[8];
    Pin pins[MAX_PINS];
    int pin_count;
} Component;

extern Component components[MAX_COMPONENTS];
extern int component_count;

Component* create_component(const char* name,
                            const char* type,
                            double x,
                            double y,
                            const char* orient);
void add_pin(Component* c,
             const char* pin_name,
             double local_x,
             double local_y);
             
void attach_pins_to_nodes(void);

#endif