#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include "component.h"

typedef void (*EmitterFn)(Component*);

EmitterFn find_handler(const char *type);

#endif