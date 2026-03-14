/* Component management functions */

#include "component.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Component components[MAX_COMPONENTS];
int component_count = 0;

Component* create_component(const char *name, const char *value,
                            const char *type, double x, double y,
                            const char *orient)
{
    if (component_count >= MAX_COMPONENTS) {
        fprintf(stderr, "Too many components\n");
        exit(1);
    }

    Component* c = &components[component_count++];

    strncpy(c->name,   name,   sizeof(c->name)   - 1);
    c->name[sizeof(c->name) - 1] = '\0';

    strncpy(c->value,  value,  sizeof(c->value)  - 1);
    c->value[sizeof(c->value) - 1] = '\0';

    strncpy(c->type,   type,   sizeof(c->type)   - 1);
    c->type[sizeof(c->type) - 1] = '\0';

    strncpy(c->orient, orient, sizeof(c->orient) - 1);
    c->orient[sizeof(c->orient) - 1] = '\0';

    c->position.x =  x * SCALE;
    c->position.y = -y * SCALE;

    c->pin_count = 0;

    return c;
}

void add_pin(Component* c, const char* pin_name, double local_x, double local_y)
{
    if (c->pin_count >= MAX_PINS) {
        fprintf(stderr, "Too many pins in %s\n", c->name);
        exit(1);
    }

    Pin* p = &c->pins[c->pin_count++];

    strncpy(p->name, pin_name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';

    p->local_pos.x = local_x * 16.0 * SCALE;
    p->local_pos.y = local_y * 16.0 * SCALE;   /* NO Y flip here */
    p->node = -1;
}

void attach_pins_to_nodes(void)
{
    for (int i = 0; i < component_count; i++) {
        Component* c = &components[i];

        for (int j = 0; j < c->pin_count; j++) {
            Pin* p = &c->pins[j];

            int point_index = match_point(p->abs_pos.x, p->abs_pos.y);
            if (point_index < 0) {
                p->node = -1;
                continue;
            }
            p->node = node_id[uf_find(point_index)];
        }
    }
}