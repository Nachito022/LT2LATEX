#include "component_registry.h"
#include <string.h>

typedef void (*EmitterFn)(Component*);

typedef struct {
    const char *type;
    EmitterFn emit;
} ComponentHandler;




