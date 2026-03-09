#ifndef GEOMETRY_H
#define GEOMETRY_H

#define EPS   1e-6
#define SCALE 0.05
#define PI 3.14159265358979323846

typedef struct {
    double x;
    double y;
} Vector;

int double_equal(double a, double b);

Vector rotate(Vector p, double angle_deg);
Vector mirror(Vector p);
void apply_orientation(Vector *p, const char *orient);
Vector transform_pin(Vector local, Vector origin, const char *orient);
void transform_component_pins(void);

#endif

