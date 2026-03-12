#include "parser.h"
#include "component.h"
#include "geometry.h"
#include "graph.h"
#include "symbol_library.h"
#include "wire.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------ */
/*  Helpers                                                             */
/* ------------------------------------------------------------------ */

/* Case-insensitive symbol lookup in symbol_library */
static const Symbol *find_symbol(const char *type_name)
{


    const char *base = type_name;
    const char *p;

    if ((p = strrchr(base, '\\')) != NULL) base = p + 1;
    if ((p = strrchr(base, '/' )) != NULL) base = p + 1;

    for (int i = 0; i < SYMBOL_COUNT; i++) {
        if (strcasecmp(symbol_library[i]->name, base) == 0)
            return symbol_library[i];
    }
    return NULL;
}

/* Strip leading/trailing whitespace in-place */
static char *trim(char *s)
{
    while (isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

/* ------------------------------------------------------------------ */
/*  Parser state                                                        */
/* ------------------------------------------------------------------ */

typedef struct {
    char   type[64];      /* symbol type, e.g. "res", "npn"          */
    char   name[64];      /* instance name, e.g. "R1", "Q3"          */
    char   value[64];     /* value string, e.g. "10k", "2N2222"      */
    char   orient[8];     /* orientation, e.g. "R0", "M90"           */
    double x, y;          /* schematic position (raw LTspice units)   */
    int    valid;         /* 1 once we have type + position           */
} PendingComp;

static void reset_pending(PendingComp *p)
{
    memset(p, 0, sizeof(*p));
    strcpy(p->orient, "R0");   /* default orientation */
}

/* ------------------------------------------------------------------ */
/*  Commit a pending component into the component/graph arrays         */
/* ------------------------------------------------------------------ */
static void commit_component(PendingComp *p)
{
    if (!p->valid) return;

    const Symbol *sym = find_symbol(p->type);
    if (!sym) {
        /* Strip path for cleaner warning */
        const char *base = p->type;
        const char *q;
        if ((q = strrchr(base, '\\')) != NULL) base = q + 1;
        if ((q = strrchr(base, '/'))  != NULL) base = q + 1;

        fprintf(stderr,
                "WARNING: unknown symbol '%s' for instance '%s' — skipped\n",
                base, p->name);
        reset_pending(p);
        return;
    }

    /* Build label: "R1=10k" or just "R1" if no value */
    char label[128];
    if (p->value[0] != '\0')
        snprintf(label, sizeof(label), "%s=%s", p->name, p->value);
    else
        snprintf(label, sizeof(label), "%s", p->name);

    /* Strip subfolder prefix e.g. "OpAmps\\UniversalOpAmp" → "UniversalOpAmp" */
    const char *base_type = p->type;
    const char *q;
    if ((q = strrchr(base_type, '\\')) != NULL) base_type = q + 1;
    if ((q = strrchr(base_type, '/'))  != NULL) base_type = q + 1;

    /* Create the component (position scaling + Y-flip done inside) */
    Component *c = create_component(label, base_type, p->x, p->y, p->orient);

    /* Add each pin from the symbol library */
    for (int i = 0; i < sym->pin_count; i++) {
        const SymbolPin *sp = &sym->pins[i];
        add_pin(c, sp->name, (double)sp->x, (double)sp->y);
    }

    reset_pending(p);
}

/* ------------------------------------------------------------------ */
/*  Main parser                                                         */
/* ------------------------------------------------------------------ */
int parse_asc(const char *filepath)
{
    FILE *f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, "ERROR: cannot open '%s'\n", filepath);
        return -1;
    }

    char line[512];
    PendingComp pending;
    reset_pending(&pending);

    while (fgets(line, sizeof(line), f)) {
        char *l = trim(line);

        /* ---- SYMBOL line: starts a new component block ---- */
        /* Format: SYMBOL <type> <x> <y> <orient>              */
        if (strncmp(l, "SYMBOL ", 7) == 0) {
            /* Commit any previous pending component first */
            commit_component(&pending);
            reset_pending(&pending);

            char type[64];
            double x, y;
            char orient[8];

            if (sscanf(l + 7, "%63s %lf %lf %7s", type, &x, &y, orient) == 4) {
                strncpy(pending.type,   type,   sizeof(pending.type)   - 1);
                strncpy(pending.orient, orient, sizeof(pending.orient) - 1);
                pending.x     = x;
                pending.y     = y;
                pending.valid = 1;
            }
        }

        /* ---- SYMATTR: attributes attached to the current symbol ---- */
        /* Format: SYMATTR <key> <value>                                */
        else if (strncmp(l, "SYMATTR ", 8) == 0 && pending.valid) {
            char key[64], val[256];
            if (sscanf(l + 8, "%63s %255[^\n]", key, val) >= 1) {
                if (strcasecmp(key, "InstName") == 0)
                    strncpy(pending.name,  trim(val), sizeof(pending.name)  - 1);
                else if (strcasecmp(key, "Value") == 0)
                    strncpy(pending.value, trim(val), sizeof(pending.value) - 1);
            }
        }

        /* ---- WIRE line: connects two points ---- */
        /* Format: WIRE <x1> <y1> <x2> <y2>        */
        else if (strncmp(l, "WIRE ", 5) == 0) {
            double x1, y1, x2, y2;
        if (sscanf(l + 5, "%lf %lf %lf %lf", &x1, &y1, &x2, &y2) == 4)
            add_wire(x1, y1, x2, y2);   /* scaling + Y-flip done inside add_wire */
}

        /* ---- FLAG / IOPIN: ground and port markers ---- */
        /* Format: FLAG <x> <y> <net_name>                */
        else if (strncmp(l, "FLAG ", 5) == 0) {
            double x, y;
            char   net[64];
            if (sscanf(l + 5, "%lf %lf %63s", &x, &y, net) == 3) {
                x *=  SCALE;
                y  = -y * SCALE;
                find_or_create_point(x, y);   /* ensure it's in the graph */
            }
        }
    }

    /* Commit the last pending component */
    commit_component(&pending);

    fclose(f);

    /* ---- Post-processing ---- */
    transform_component_pins();   /* local → absolute coords            */
    assign_nodes();               /* union-find roots → integer node IDs */
    attach_pins_to_nodes();       /* match each pin to its node ID       */

    return 0;
}
