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
    { "polcap",         "capacitor" },

    /* Diodes */
    { "diode",          "diode" },
    { "zener",          "zener" },
    { "schottky",       "schottky" },
    { "led",            "led" },
    { "photo",          "photodiode" },
    { "varactor",       "varactor" },
    { "TVSdiode",       "tvs diode" },

    /* Transistors - BJT */
    { "npn",            "npn" },
    { "pnp",            "pnp" },
    { "npn2",           "npn" },
    { "pnp2",           "pnp" },
    { "lpnp",           "pnp" },
    { "npn3",           "npn" },
    { "npn4",           "npn" },
    { "pnp4",           "pnp" },

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
    { "e2",             "controlled voltage source" },
    { "g2",             "controlled current source" },
    { "bv",             "controlled voltage source" },

    /* Opamps & Comparators */
    { "opamp",          "op amp" },
    { "opamp2",         "op amp" },
    { "comparator",     "op amp" },
    { "UniversalOpAmp", "op amp" },
    

    /* Switches */
    { "sw",             "nos" },
    { "csw",            "ncs" },

    /* Transmission lines */
    { "tline",          "transmission line" },
    { "ltline",         "transmission line" },

    /* Loads */
    { "load",           "load" },
    { "load2",          "load" },

    /* Thermal */
    { "SOAtherm_PCB",       "thermal" },
    { "SOAtherm_HeatSink",  "thermal" },
    { "SOAtherm_NMOS",      "thermal" },

    /* Automotive */
    { "ISO16750_2",     "voltage source" },
    { "ISO7637_2",      "voltage source" },

    /* FRA */
    { "fra",            "fra" },
    { "fraprobe",       "fra probe" },

    /* Misc */
    { "xtal",           "crystal" },
    { "mesfet",         "mesfet" },
    { "transformer",    "transformer" },
    { "transformer2",   "ideal transformer" },
    { "fuse",           "fuse" },
    { "antenna",        "antenna" },
    { "FerriteBead",    "ferrite bead" },
    { "FerriteBead2",   "ferrite bead" },


    /* Specific models - map to their generic type */
    { "TIP41C",         "npn" },
    { "TIP42C",         "pnp" },
    { "TL081",          "op amp" },

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