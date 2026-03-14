#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include <string.h>

typedef struct {
    const char *ltspice_name;
    const char *circuitikz_name;
    int needs_mirror;
} ComponentMap;

static const ComponentMap COMPONENT_MAP[] = {
    /* Resistors / Capacitors / Inductors */
    { "res",            "resistor",             0 },
    { "cap",            "capacitor",            0 },
    { "ind",            "inductor",             0 },
    { "ind2",           "cute inductor",        0 },
    { "pot",            "pR",                   0 },
    { "polcap",         "capacitor",            0 },

    /* Diodes */
    { "diode",          "diode",                0 },
    { "zener",          "zener",                0 },
    { "schottky",       "schottky",             0 },
    { "led",            "led",                  0 },
    { "photo",          "photodiode",           0 },
    { "varactor",       "varactor",             0 },
    { "TVSdiode",       "tvs diode",            0 },

    /* Transistors - BJT */
    { "npn",            "npn",                  0 },
    { "pnp",            "pnp",                  1 },
    { "npn2",           "npn",                  0 },
    { "pnp2",           "pnp",                  1 },
    { "lpnp",           "pnp",                  1 },
    { "npn3",           "npn",                  0 },
    { "npn4",           "npn",                  0 },
    { "pnp4",           "pnp",                  1 },

    /* Transistors - MOSFET */
    { "nmos",           "nmos",                 0 },
    { "pmos",           "pmos",                 0 },
    { "nmos4",          "nmos",                 0 },
    { "pmos4",          "pmos",                 0 },

    /* Transistors - JFET */
    { "njf",            "njfet",                0 },
    { "pjf",            "pjfet",                0 },

    /* Sources */
    { "voltage",        "vsource",              0 },
    { "current",        "isource",              0 },
    { "bi",             "controlled current source", 0 },
    { "bi2",            "controlled voltage source", 0 },
    { "e",              "vsource",              0 },
    { "e2",             "vsource",              0 },
    { "g",              "isource",              0 },
    { "g2",             "isource",              0 },
    { "f",              "controlled current source", 0 },
    { "h",              "controlled voltage source", 0 },
    { "bv",             "controlled voltage source", 0 },

    /* Opamps & Comparators */
    { "opamp",          "op amp",               0 },
    { "opamp2",         "op amp",               0 },
    { "comparator",     "op amp",               0 },
    { "UniversalOpAmp", "op amp",               0 },

    /* Switches */
    { "sw",             "nos",                  0 },
    { "csw",            "ncs",                  0 },

    /* Transmission lines */
    { "tline",          "transmission line",    0 },
    { "ltline",         "transmission line",    0 },

    /* Loads */
    { "load",           "load",                 0 },
    { "load2",          "load",                 0 },

    /* Thermal */
    { "SOAtherm_PCB",       "thermal",          0 },
    { "SOAtherm_HeatSink",  "thermal",          0 },
    { "SOAtherm_NMOS",      "thermal",          0 },

    /* Automotive */
    { "ISO16750_2",     "voltage source",       0 },
    { "ISO7637_2",      "voltage source",       0 },

    /* FRA */
    { "fra",            "fra",                  0 },
    { "fraprobe",       "fra probe",            0 },

    /* Misc */
    { "xtal",           "crystal",              0 },
    { "mesfet",         "mesfet",               0 },
    { "transformer",    "transformer",          0 },
    { "transformer2",   "ideal transformer",    0 },
    { "fuse",           "fuse",                 0 },
    { "antenna",        "antenna",              0 },
    { "FerriteBead",    "ferrite bead",         0 },
    { "FerriteBead2",   "ferrite bead",         0 },

    /* Specific models */
    { "TIP41C",         "npn",                  0 },
    { "TIP42C",         "pnp",                  1 },
    { "TL081",          "op amp",               0 },
};

static const int COMPONENT_MAP_SIZE =
    (int)(sizeof(COMPONENT_MAP) / sizeof(COMPONENT_MAP[0]));

static inline const ComponentMap *ltspice_to_map(const char *ltspice_name)
{
    for (int i = 0; i < COMPONENT_MAP_SIZE; i++) {
        if (strcasecmp(COMPONENT_MAP[i].ltspice_name, ltspice_name) == 0)
            return &COMPONENT_MAP[i];
    }
    return NULL;
}

#endif /* COMPONENT_REGISTRY_H */