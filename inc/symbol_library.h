#ifndef SYMBOL_LIBRARY_H
#define SYMBOL_LIBRARY_H

typedef struct {
    const char *name;
    int x;
    int y;
} SymbolPin;

typedef struct {
    const char *name;
    const char *prefix;
    int pin_count;
    const SymbolPin *pins;
} Symbol;

static const SymbolPin e2_pins[] = { {"+", 0, 1}, {"-", 0, 6}, {"P", -3, 5}, {"N", -3, 2} };
static const Symbol e2 = {
    .name      = "e2",
    .prefix    = "E",
    .pin_count = 4,
    .pins      = e2_pins,
};

static const SymbolPin SOAtherm_PCB_pins[] = { {"Tcenter", 0, 0} };
static const Symbol SOAtherm_PCB = {
    .name      = "SOAtherm-PCB",
    .prefix    = "X",
    .pin_count = 1,
    .pins      = SOAtherm_PCB_pins,
};

static const SymbolPin LED_pins[] = { {"+", 1, 0}, {"-", 1, 4} };
static const Symbol LED = {
    .name      = "LED",
    .prefix    = "D",
    .pin_count = 2,
    .pins      = LED_pins,
};

static const SymbolPin load_pins[] = { {"A", 1, 0}, {"B", 1, 4} };
static const Symbol load = {
    .name      = "load",
    .prefix    = "I",
    .pin_count = 2,
    .pins      = load_pins,
};

static const SymbolPin UniversalOpAmp_pins[] = { {"In+", -2, 1}, {"In-", -2, -1}, {"V+", 0, -2}, {"V-", 0, 2}, {"OUT", 2, 0} };
static const Symbol UniversalOpAmp = {
    .name      = "UniversalOpAmp",
    .prefix    = "X",
    .pin_count = 5,
    .pins      = UniversalOpAmp_pins,
};

static const SymbolPin pnp2_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol pnp2 = {
    .name      = "pnp2",
    .prefix    = "QP",
    .pin_count = 3,
    .pins      = pnp2_pins,
};

static const SymbolPin njf_pins[] = { {"D", 3, 0}, {"G", 0, 4}, {"S", 3, 6} };
static const Symbol njf = {
    .name      = "njf",
    .prefix    = "JN",
    .pin_count = 3,
    .pins      = njf_pins,
};

static const SymbolPin csw_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol csw = {
    .name      = "csw",
    .prefix    = "W",
    .pin_count = 2,
    .pins      = csw_pins,
};

static const SymbolPin tline_pins[] = { {"I1", -3, -1}, {"R1", -3, 1}, {"I2", 3, -1}, {"R2", 3, 1} };
static const Symbol tline = {
    .name      = "tline",
    .prefix    = "T",
    .pin_count = 4,
    .pins      = tline_pins,
};

static const SymbolPin FerriteBead2_pins[] = { {"A", 0, -2}, {"B", 0, 2} };
static const Symbol FerriteBead2 = {
    .name      = "FerriteBead2",
    .prefix    = "L_Ferrite_Bead",
    .pin_count = 2,
    .pins      = FerriteBead2_pins,
};

static const SymbolPin pmos_pins[] = { {"D", 3, 0}, {"G", 0, 5}, {"S", 3, 6} };
static const Symbol pmos = {
    .name      = "pmos",
    .prefix    = "MP",
    .pin_count = 3,
    .pins      = pmos_pins,
};

static const SymbolPin ltline_pins[] = { {"I1", -3, -1}, {"R1", -3, 1}, {"I2", 3, -1}, {"R2", 3, 1} };
static const Symbol ltline = {
    .name      = "ltline",
    .prefix    = "O",
    .pin_count = 4,
    .pins      = ltline_pins,
};

static const SymbolPin schottky_pins[] = { {"+", 1, 0}, {"-", 1, 4} };
static const Symbol schottky = {
    .name      = "schottky",
    .prefix    = "D",
    .pin_count = 2,
    .pins      = schottky_pins,
};

static const SymbolPin ISO16750_2_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol ISO16750_2 = {
    .name      = "ISO16750-2",
    .prefix    = "X",
    .pin_count = 2,
    .pins      = ISO16750_2_pins,
};

static const SymbolPin pnp_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol pnp = {
    .name      = "pnp",
    .prefix    = "QP",
    .pin_count = 3,
    .pins      = pnp_pins,
};

static const SymbolPin pnp4_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6}, {"S", 4, 3} };
static const Symbol pnp4 = {
    .name      = "pnp4",
    .prefix    = "QP",
    .pin_count = 4,
    .pins      = pnp4_pins,
};

static const SymbolPin res_pins[] = { {"A", 1, 1}, {"B", 1, 6} };
static const Symbol res = {
    .name      = "res",
    .prefix    = "R",
    .pin_count = 2,
    .pins      = res_pins,
};

static const SymbolPin varactor_pins[] = { {"+", 1, 0}, {"-", 1, 4} };
static const Symbol varactor = {
    .name      = "varactor",
    .prefix    = "D",
    .pin_count = 2,
    .pins      = varactor_pins,
};

static const SymbolPin sw_pins[] = { {"A", 0, 1}, {"B", 0, 6}, {"NC+", -3, 5}, {"NC-", -3, 2} };
static const Symbol sw = {
    .name      = "sw",
    .prefix    = "S",
    .pin_count = 4,
    .pins      = sw_pins,
};

static const SymbolPin mesfet_pins[] = { {"D", 3, 0}, {"G", 0, 5}, {"S", 3, 6} };
static const Symbol mesfet = {
    .name      = "mesfet",
    .prefix    = "Z",
    .pin_count = 3,
    .pins      = mesfet_pins,
};

static const SymbolPin f_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol f = {
    .name      = "f",
    .prefix    = "F",
    .pin_count = 2,
    .pins      = f_pins,
};

static const SymbolPin voltage_pins[] = { {"+", 0, 1}, {"-", 0, 6} };
static const Symbol voltage = {
    .name      = "voltage",
    .prefix    = "V",
    .pin_count = 2,
    .pins      = voltage_pins,
};

static const SymbolPin lpnp_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6}, {"S", 4, 3} };
static const Symbol lpnp = {
    .name      = "lpnp",
    .prefix    = "QP",
    .pin_count = 4,
    .pins      = lpnp_pins,
};

static const SymbolPin ISO7637_2_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol ISO7637_2 = {
    .name      = "ISO7637-2",
    .prefix    = "X",
    .pin_count = 2,
    .pins      = ISO7637_2_pins,
};

static const SymbolPin TIP41C_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol TIP41C = {
    .name      = "TIP41C",
    .prefix    = "Q",
    .pin_count = 3,
    .pins      = TIP41C_pins,
};

static const SymbolPin zener_pins[] = { {"+", 1, 0}, {"-", 1, 4} };
static const Symbol zener = {
    .name      = "zener",
    .prefix    = "D",
    .pin_count = 2,
    .pins      = zener_pins,
};

static const SymbolPin ind2_pins[] = { {"A", 1, 1}, {"B", 1, 6} };
static const Symbol ind2 = {
    .name      = "ind2",
    .prefix    = "L",
    .pin_count = 2,
    .pins      = ind2_pins,
};

static const SymbolPin pjf_pins[] = { {"D", 3, 0}, {"G", 0, 4}, {"S", 3, 6} };
static const Symbol pjf = {
    .name      = "pjf",
    .prefix    = "JP",
    .pin_count = 3,
    .pins      = pjf_pins,
};

static const SymbolPin nmos_pins[] = { {"D", 3, 0}, {"G", 0, 5}, {"S", 3, 6} };
static const Symbol nmos = {
    .name      = "nmos",
    .prefix    = "MN",
    .pin_count = 3,
    .pins      = nmos_pins,
};

static const SymbolPin g2_pins[] = { {"+", 0, 6}, {"-", 0, 1}, {"NC+", -3, 5}, {"NC-", -3, 2} };
static const Symbol g2 = {
    .name      = "g2",
    .prefix    = "G",
    .pin_count = 4,
    .pins      = g2_pins,
};

static const SymbolPin g_pins[] = { {"+", 0, 6}, {"-", 0, 1}, {"NC+", -3, 2}, {"NC-", -3, 5} };
static const Symbol g = {
    .name      = "g",
    .prefix    = "G",
    .pin_count = 4,
    .pins      = g_pins,
};

static const SymbolPin load2_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol load2 = {
    .name      = "load2",
    .prefix    = "I",
    .pin_count = 2,
    .pins      = load2_pins,
};

static const SymbolPin diode_pins[] = { {"+", 1, 0}, {"-", 1, 4} };
static const Symbol diode = {
    .name      = "diode",
    .prefix    = "D",
    .pin_count = 2,
    .pins      = diode_pins,
};

static const SymbolPin cap_pins[] = { {"A", 1, 0}, {"B", 1, 4} };
static const Symbol cap = {
    .name      = "cap",
    .prefix    = "C",
    .pin_count = 2,
    .pins      = cap_pins,
};

static const SymbolPin pmos4_pins[] = { {"D", 3, 0}, {"G", 0, 5}, {"S", 3, 6}, {"B", 3, 3} };
static const Symbol pmos4 = {
    .name      = "pmos4",
    .prefix    = "MP",
    .pin_count = 4,
    .pins      = pmos4_pins,
};

static const SymbolPin res2_pins[] = { {"A", 1, 0}, {"B", 1, 4} };
static const Symbol res2 = {
    .name      = "res2",
    .prefix    = "R",
    .pin_count = 2,
    .pins      = res2_pins,
};

static const SymbolPin npn_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol npn = {
    .name      = "npn",
    .prefix    = "QN",
    .pin_count = 3,
    .pins      = npn_pins,
};

static const SymbolPin npn4_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6}, {"S", 4, 3} };
static const Symbol npn4 = {
    .name      = "npn4",
    .prefix    = "QN",
    .pin_count = 4,
    .pins      = npn4_pins,
};

static const SymbolPin bi_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol bi = {
    .name      = "bi",
    .prefix    = "B",
    .pin_count = 2,
    .pins      = bi_pins,
};

static const SymbolPin TVSdiode_pins[] = { {"+", 1, 0}, {"-", 1, 4} };
static const Symbol TVSdiode = {
    .name      = "TVSdiode",
    .prefix    = "D",
    .pin_count = 2,
    .pins      = TVSdiode_pins,
};

static const SymbolPin bi2_pins[] = { {"+", 0, 5}, {"-", 0, 0} };
static const Symbol bi2 = {
    .name      = "bi2",
    .prefix    = "B",
    .pin_count = 2,
    .pins      = bi2_pins,
};

static const SymbolPin polcap_pins[] = { {"A", 1, 0}, {"B", 1, 4} };
static const Symbol polcap = {
    .name      = "polcap",
    .prefix    = "C",
    .pin_count = 2,
    .pins      = polcap_pins,
};

static const SymbolPin TL081_pins[] = { {"+", 0, 1}, {"-", 0, -1}, {"V+", 2, -3}, {"V-", 2, 3}, {"Out", 4, 0} };
static const Symbol TL081 = {
    .name      = "TL081",
    .prefix    = "X",
    .pin_count = 5,
    .pins      = TL081_pins,
};

static const SymbolPin current_pins[] = { {"+", 0, 0}, {"-", 0, 5} };
static const Symbol current = {
    .name      = "current",
    .prefix    = "I",
    .pin_count = 2,
    .pins      = current_pins,
};

static const SymbolPin npn3_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol npn3 = {
    .name      = "npn3",
    .prefix    = "QN",
    .pin_count = 3,
    .pins      = npn3_pins,
};

static const SymbolPin e_pins[] = { {"+", 0, 1}, {"-", 0, 6}, {"P", -3, 2}, {"N", -3, 5} };
static const Symbol e = {
    .name      = "e",
    .prefix    = "E",
    .pin_count = 4,
    .pins      = e_pins,
};

static const SymbolPin bv_pins[] = { {"+", 0, 1}, {"-", 0, 6} };
static const Symbol bv = {
    .name      = "bv",
    .prefix    = "B",
    .pin_count = 2,
    .pins      = bv_pins,
};

static const SymbolPin nmos4_pins[] = { {"D", 3, 0}, {"G", 0, 5}, {"S", 3, 6}, {"B", 3, 3} };
static const Symbol nmos4 = {
    .name      = "nmos4",
    .prefix    = "MN",
    .pin_count = 4,
    .pins      = nmos4_pins,
};

static const SymbolPin TIP42C_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol TIP42C = {
    .name      = "TIP42C",
    .prefix    = "Q",
    .pin_count = 3,
    .pins      = TIP42C_pins,
};

static const SymbolPin SOAtherm_HeatSink_pins[] = { {"Tc", -5, 0}, {"Tinterface", -3, 0}, {"Tfin", 3, 0} };
static const Symbol SOAtherm_HeatSink = {
    .name      = "SOAtherm-HeatSink",
    .prefix    = "X",
    .pin_count = 3,
    .pins      = SOAtherm_HeatSink_pins,
};

static const SymbolPin ind_pins[] = { {"A", 1, 1}, {"B", 1, 6} };
static const Symbol ind = {
    .name      = "ind",
    .prefix    = "L",
    .pin_count = 2,
    .pins      = ind_pins,
};

static const SymbolPin fra_pins[] = { {"OUT", 0, -4}, {"IN", 0, 0} };
static const Symbol fra = {
    .name      = "fra",
    .prefix    = "@",
    .pin_count = 2,
    .pins      = fra_pins,
};

static const SymbolPin SOAtherm_NMOS_pins[] = { {"D", 0, -7}, {"G", -4, 2}, {"S", 0, 4}, {"D2", 0, -3}, {"G2", -3, 2}, {"S2", 0, 3}, {"Tj", 4, -4}, {"Tc", 4, -6} };
static const Symbol SOAtherm_NMOS = {
    .name      = "SOAtherm-NMOS",
    .prefix    = "X",
    .pin_count = 8,
    .pins      = SOAtherm_NMOS_pins,
};

static const SymbolPin FerriteBead_pins[] = { {"A", 0, -2}, {"B", 0, 2} };
static const Symbol FerriteBead = {
    .name      = "FerriteBead",
    .prefix    = "L_Ferrite_Bead",
    .pin_count = 2,
    .pins      = FerriteBead_pins,
};

static const SymbolPin h_pins[] = { {"+", 0, 1}, {"-", 0, 6} };
static const Symbol h = {
    .name      = "h",
    .prefix    = "H",
    .pin_count = 2,
    .pins      = h_pins,
};

static const SymbolPin npn2_pins[] = { {"C", 4, 0}, {"B", 0, 3}, {"E", 4, 6} };
static const Symbol npn2 = {
    .name      = "npn2",
    .prefix    = "QN",
    .pin_count = 3,
    .pins      = npn2_pins,
};

static const SymbolPin fraprobe_pins[] = {  };
static const Symbol fraprobe = {
    .name      = "fraprobe",
    .prefix    = "?",
    .pin_count = 0,
    .pins      = fraprobe_pins,
};

#define SYMBOL_COUNT 57

static const Symbol *symbol_library[] = {
    &e2,
    &SOAtherm_PCB,
    &LED,
    &load,
    &UniversalOpAmp,
    &pnp2,
    &njf,
    &csw,
    &tline,
    &FerriteBead2,
    &pmos,
    &ltline,
    &schottky,
    &ISO16750_2,
    &pnp,
    &pnp4,
    &res,
    &varactor,
    &sw,
    &mesfet,
    &f,
    &voltage,
    &lpnp,
    &ISO7637_2,
    &TIP41C,
    &zener,
    &ind2,
    &pjf,
    &nmos,
    &g2,
    &g,
    &load2,
    &diode,
    &cap,
    &pmos4,
    &res2,
    &npn,
    &npn4,
    &bi,
    &TVSdiode,
    &bi2,
    &polcap,
    &TL081,
    &current,
    &npn3,
    &e,
    &bv,
    &nmos4,
    &TIP42C,
    &SOAtherm_HeatSink,
    &ind,
    &fra,
    &SOAtherm_NMOS,
    &FerriteBead,
    &h,
    &npn2,
    &fraprobe,
};

static inline const Symbol *symbol_library_get(int i)
{
    return symbol_library[i];
}

#endif // SYMBOL_LIBRARY_H