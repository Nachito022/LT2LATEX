#include "geometry.h"
#include "component.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

Vector rotate(Vector p, double angle_deg)
{
    double rad = angle_deg * PI / 180.0;

    Vector r;
    r.x = p.x * cos(rad) - p.y * sin(rad);
    r.y = p.x * sin(rad) + p.y * cos(rad);

    return r;
}

Vector mirror(Vector p)
{
    p.x = -p.x;
    return p;
}

void apply_orientation(Vector *p, const char *orient)
{
    int angle = 0;
    int mirror_flag = 0;

    if (orient[0] == 'R') {
        angle = atoi(&orient[1]);
    }
    else if (orient[0] == 'M') {
        mirror_flag = 1;
        angle = atoi(&orient[1]);
    }

    if (mirror_flag)
        *p = mirror(*p);

    *p = rotate(*p, angle);
}

void transform_component_pins()
{
    for (int i = 0; i < component_count; i++) {

        Component* c = &components[i];

        for (int j = 0; j < c->pin_count; j++) {

            Vector p = c->pins[j].local_pos;

            apply_orientation(&p, c->orient);

            c->pins[j].abs_pos.x = p.x + c->position.x;
            c->pins[j].abs_pos.y = p.y + c->position.y;
        }
    }
}

int double_equal(double a, double b)
{
    return fabs(a - b) < EPS;
}
