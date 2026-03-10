#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include <string.h>

typedef struct {
    const char *ltspice_name;
    const char *circuitikz_name;
} ComponentMap;

static const ComponentMap COMPONENT_MAP[] = {
    /* Resistors / Capacitors / Inductors */
    { "res",            "resistor" },
    { "cap",            "capacitor" },
    { "ind",            "inductor" },
    { "ind2",           "cute inductor" },
    { "pot",            "pR" },

    /* Diodes */
    { "diode",          "diode" },
    { "zener",          "zener" },
    { "schottky",       "schottky" },
    { "led",            "led" },
    { "photo",          "photodiode" },

    /* Transistors - BJT */
    { "npn",            "npn" },
    { "pnp",            "pnp" },
    { "npn2",           "npn" },
    { "pnp2",           "pnp" },

    /* Transistors - MOSFET */
    { "nmos",           "nmos" },
    { "pmos",           "pmos" },
    { "nmos4",          "nmos" },
    { "pmos4",          "pmos" },

    /* Transistors - JFET */
    { "njf",            "njfet" },
    { "pjf",            "pjfet" },

    /* Sources */
    { "voltage",        "vsource" },
    { "current",        "isource" },
    { "bi",             "controlled current source" },
    { "bi2",            "controlled voltage source" },
    { "e",              "controlled voltage source" },
    { "f",              "controlled current source" },
    { "g",              "controlled current source" },
    { "h",              "controlled voltage source" },

    /* Opamps & Comparators */
    { "opamp",          "op amp" },
    { "opamp2",         "op amp" },
    { "comparator",     "op amp" },

    /* Switches */
    { "sw",             "nos" },
    { "csw",            "ncs" },

    /* Misc */
    { "xtal",           "crystal" },
    { "transformer",    "transformer" },
    { "transformer2",   "ideal transformer" },
    { "fuse",           "fuse" },
    { "antenna",        "antenna" },
};

static const int COMPONENT_MAP_SIZE =
    (int)(sizeof(COMPONENT_MAP) / sizeof(COMPONENT_MAP[0]));

/* Lookup function - returns NULL if not found */
static inline const char *ltspice_to_circuitikz(const char *ltspice_name)
{
    for (int i = 0; i < COMPONENT_MAP_SIZE; i++) {
        if (strcasecmp(COMPONENT_MAP[i].ltspice_name, ltspice_name) == 0)
            return COMPONENT_MAP[i].circuitikz_name;
    }
    return NULL;
}

#endif /* COMPONENT_REGISTRY_H */