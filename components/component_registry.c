#include "component_registry.h"
#include <string.h>

typedef void (*EmitterFn)(Component*);

typedef struct {
    const char *type;
    EmitterFn emit;
} ComponentHandler;

extern void emit_resistor(Component*);
extern void emit_capacitor(Component*);
extern void emit_opamp(Component*);
extern void emit_bjt(Component*);

ComponentHandler registry[] = {
    {"R", emit_resistor},
    {"C", emit_capacitor},
    {"L", emit_inductor},
    {"V", emit_voltage},
    {"OPAMP", emit_opamp},
    {"NPN", emit_bjt},
    {NULL, NULL}
};

EmitterFn find_handler(const char *type)
{
    for (int i = 0; registry[i].type; i++)
        if (strcmp(registry[i].type, type) == 0)
            return registry[i].emit;

    return NULL;
}
