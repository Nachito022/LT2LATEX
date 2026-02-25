#include "../inc/component.h"
#include "../inc/emitter.h"

void emit_resistor(Component *c)
{
    Pin *a = &c->pins[0];
    Pin *b = &c->pins[1];

    printf("\\draw (%.3f,%.3f) to[R,l=$%s$] (%.3f,%.3f);\n",
        a->abs_pos.x * SCALE,
        a->abs_pos.y * SCALE,
        c->name,
        b->abs_pos.x * SCALE,
        b->abs_pos.y * SCALE
    );
}
