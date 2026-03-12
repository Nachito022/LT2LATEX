#ifndef SYMBOL_PIN_MAP_H
#define SYMBOL_PIN_MAP_H

#include <string.h>
#include "symbol_library.h"

typedef struct {
    const char *ltspice_pin;
    const char *tikz_anchor;
} PinMap;

typedef struct {
    const char *symbol_name;
    const PinMap *pin_map;
    int pin_map_count;
} SymbolPinMap;


/* ── BJTs (npn, pnp, npn2, npn3, pnp2, TIP41C, TIP42C) ── */
static const PinMap bjt_pin_map[] = {
    { "C", "collector" },
    { "B", "base" },
    { "E", "emitter" },
};

/* ── BJT 4-pin substrate (npn4, pnp4, lpnp) ── */
static const PinMap bjt4_pin_map[] = {
    { "C", "collector" },
    { "B", "base" },
    { "E", "emitter" },
    { "S", "substrate" },   /* no circuitikz anchor — draw manually */
};

/* ── MOSFETs 3-pin (nmos, pmos, mesfet) ── */
static const PinMap mosfet_pin_map[] = {
    { "D", "drain" },
    { "G", "gate" },
    { "S", "source" },
};

/* ── MOSFETs 4-pin substrate (nmos4, pmos4) ── */
static const PinMap mosfet4_pin_map[] = {
    { "D", "drain" },
    { "G", "gate" },
    { "S", "source" },
    { "B", "bulk" },        /* no circuitikz anchor — draw manually */
};

/* ── JFETs (njf, pjf) ── */
static const PinMap jfet_pin_map[] = {
    { "D", "drain" },
    { "G", "gate" },
    { "S", "source" },
};

/* ── Op-amps (UniversalOpAmp) ── */
static const PinMap opamp_pin_map[] = {
    { "In+", "+" },
    { "In-", "-" },
    { "OUT", "out" },
    { "V+",  "up" },
    { "V-",  "down" },
};

/* ── Op-amps specific model (TL081) ── */
static const PinMap tl081_pin_map[] = {
    { "+",   "+" },
    { "-",   "-" },
    { "Out", "out" },
    { "V+",  "up" },
    { "V-",  "down" },
};

/* ── Voltage-controlled voltage source (e, e2) ── */
static const PinMap vcvs_pin_map[] = {
    { "+", "north" },
    { "-", "south" },
};

/* ── Voltage-controlled current source (g, g2) ── */
static const PinMap vccs_pin_map[] = {
    { "+", "north" },
    { "-", "south" },
};

/* ── Voltage-controlled switch (sw) ── */
static const PinMap sw_pin_map[] = {
    { "A",   "in" },
    { "B",   "out" },
    { "NC+", "control+" },
    { "NC-", "control-" },
};

/* ── Transmission lines (tline, ltline) ── */
static const PinMap tline_pin_map[] = {
    { "I1", "in1+" },
    { "R1", "in1-" },
    { "I2", "in2+" },
    { "R2", "in2-" },
};

/* ── SOAtherm HeatSink ── */
static const PinMap soatherm_heatsink_pin_map[] = {
    { "Tc",         "Tc" },
    { "Tinterface", "Tinterface" },
    { "Tfin",       "Tfin" },
};

/* ── SOAtherm NMOS (behavioural — no circuitikz equivalent) ── */
static const PinMap soatherm_nmos_pin_map[] = {
    { "D",  "drain" },
    { "G",  "gate" },
    { "S",  "source" },
    { "D2", "drain2" },
    { "G2", "gate2" },
    { "S2", "source2" },
    { "Tj", "Tj" },
    { "Tc", "Tc" },
};


/* ── Master lookup ── */
static const SymbolPinMap SYMBOL_PIN_MAP[] = {
    { "npn",              bjt_pin_map,             3 },
    { "npn2",             bjt_pin_map,             3 },
    { "npn3",             bjt_pin_map,             3 },
    { "npn4",             bjt4_pin_map,            4 },
    { "pnp",              bjt_pin_map,             3 },
    { "pnp2",             bjt_pin_map,             3 },
    { "pnp4",             bjt4_pin_map,            4 },
    { "lpnp",             bjt4_pin_map,            4 },
    { "TIP41C",           bjt_pin_map,             3 },
    { "TIP42C",           bjt_pin_map,             3 },
    { "nmos",             mosfet_pin_map,          3 },
    { "pmos",             mosfet_pin_map,          3 },
    { "mesfet",           mosfet_pin_map,          3 },
    { "nmos4",            mosfet4_pin_map,         4 },
    { "pmos4",            mosfet4_pin_map,         4 },
    { "njf",              jfet_pin_map,            3 },
    { "pjf",              jfet_pin_map,            3 },
    { "UniversalOpAmp",   opamp_pin_map,           5 },
    { "TL081",            tl081_pin_map,           5 },
    { "e",                vcvs_pin_map,            2 },
    { "e2",               vcvs_pin_map,            2 },
    { "g",                vccs_pin_map,            2 },
    { "g2",               vccs_pin_map,            2 },
    { "sw",               sw_pin_map,              4 },
    { "tline",            tline_pin_map,           4 },
    { "ltline",           tline_pin_map,           4 },
    { "SOAtherm-HeatSink",soatherm_heatsink_pin_map, 3 },
    { "SOAtherm-NMOS",    soatherm_nmos_pin_map,   8 },
};

#define SYMBOL_PIN_MAP_COUNT (sizeof(SYMBOL_PIN_MAP) / sizeof(SYMBOL_PIN_MAP[0]))

static const char *get_tikz_anchor(const char *symbol_name, const char *ltspice_pin) {
    for (size_t i = 0; i < SYMBOL_PIN_MAP_COUNT; i++) {
        if (strcmp(SYMBOL_PIN_MAP[i].symbol_name, symbol_name) == 0) {
            for (int j = 0; j < SYMBOL_PIN_MAP[i].pin_map_count; j++) {
                if (strcmp(SYMBOL_PIN_MAP[i].pin_map[j].ltspice_pin, ltspice_pin) == 0) {
                    return SYMBOL_PIN_MAP[i].pin_map[j].tikz_anchor;
                }
            }
        }
    }
    return NULL; // not found
}

#endif // SYMBOL_PIN_MAP_H