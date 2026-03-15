/* Emitter functions for generating TikZ code */

#include "emitter.h"
#include "component.h"
#include "component_registry.h"
#include "symbol_pin_map.h"
#include "graph.h"
#include "wire.h"
#include "ground.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Helpers                                                             */
/* ------------------------------------------------------------------ */

static void node_label(int node_id_val, char *buf, size_t bufsz)
{
    if (node_id_val < 0) snprintf(buf, bufsz, "???");
    else                 snprintf(buf, bufsz, "n%d", node_id_val);
}

/* ------------------------------------------------------------------ */
/*  Anchor calibration table                                            */
/*                                                                      */
/*  For each multi-pin type, stores the circuitikz anchor offset of   */
/*  one reference pin (from node center, at R0 no mirror).            */
/*  The node is shifted so the reference pin lands exactly at its     */
/*  computed abs_pos.                                                  */
/*                                                                      */
/*  Values measured from:                                               */
/*    \node [op amp] at (0,0) (A) {};  → A.out  = (1.18999, 0.0)     */
/*    \node [npn]    at (0,0) (Q) {};  → Q.center = (4.0, 0.0)       */
/*                                        Q.collector = (4.0, 0.76999)*/
/*                                        offset = (0.0, 0.76999)     */
/*    \node [nmos]   at (0,0) (M) {};  → M.center = (8.0, 0.0)       */
/*                                        M.drain = (8.0, 0.76999)    */
/*                                        offset = (0.0, 0.76999)     */
/* ------------------------------------------------------------------ */
typedef struct {
    const char *type;
    const char *ref_pin;   /* LTspice pin name                        */
    double      tikz_dx;   /* circuitikz: center → ref_pin at R0      */
    double      tikz_dy;
} AnchorCalib;

static const AnchorCalib ANCHOR_CALIB[] = {
    /* Op amps */
    { "UniversalOpAmp", "OUT",  1.18999,  0.0     },
    { "TL081",          "Out",  1.18999,  0.0     },
    { "opamp",          "OUT",  1.18999,  0.0     },
    { "opamp2",         "OUT",  1.18999,  0.0     },
    { "comparator",     "OUT",  1.18999,  0.0     },

    /* NPN BJTs — collector offset from center */
    { "npn",    "C",  0.0,  0.76999 },
    { "npn2",   "C",  0.0,  0.76999 },
    { "npn3",   "C",  0.0,  0.76999 },
    { "npn4",   "C",  0.0,  0.76999 },
    { "TIP41C", "C",  0.0,  0.76999 },

    /* PNP BJTs */
    { "pnp",    "C",  0.0,  0.76999 },
    { "pnp2",   "C",  0.0,  0.76999 },
    { "pnp4",   "C",  0.0,  0.76999 },
    { "lpnp",   "C",  0.0,  0.76999 },
    { "TIP42C", "C",  0.0,  0.76999 },

    /* MOSFETs */
    { "nmos",   "D",  0.0,  0.76999 },
    { "nmos4",  "D",  0.0,  0.76999 },
    { "pmos",   "D",  0.0,  0.76999 },
    { "pmos4",  "D",  0.0,  0.76999 },

    /* JFETs */
    { "njf",    "D",  0.0,  0.76999 },
    { "pjf",    "D",  0.0,  0.76999 },
};

#define ANCHOR_CALIB_SIZE \
    (int)(sizeof(ANCHOR_CALIB)/sizeof(ANCHOR_CALIB[0]))

static const AnchorCalib *get_calib(const char *type)
{
    for (int i = 0; i < ANCHOR_CALIB_SIZE; i++)
        if (strcasecmp(ANCHOR_CALIB[i].type, type) == 0)
            return &ANCHOR_CALIB[i];
    return NULL;
}

/* ------------------------------------------------------------------ */
/*  2-pin emitter                                                       */
/* ------------------------------------------------------------------ */
static void emit_two_pin(FILE *out, const Component *c, const char *tikz_type)
{
    const Pin *p0 = &c->pins[0];
    const Pin *p1 = &c->pins[1];

    char label[256];
    if (c->value[0] != '\0')
        snprintf(label, sizeof(label), "%s{=}%s", c->name, c->value);
    else
        snprintf(label, sizeof(label), "%s", c->name);

    fprintf(out, "  \\draw (%.4f,%.4f) to[%s, l=$%s$] (%.4f,%.4f);\n",
            p0->abs_pos.x, p0->abs_pos.y,
            tikz_type, label,
            p1->abs_pos.x, p1->abs_pos.y);
}

/* ------------------------------------------------------------------ */
/*  Multi-pin emitter                                                   */
/*                                                                      */
/*  1. Uses anchor calibration to place the node so that the          */
/*     reference circuitikz anchor lands exactly at abs_pos.          */
/*  2. For every pin, draws a short stub from the circuitikz anchor   */
/*     to abs_pos using -| or |- (Manhattan).                         */
/* ------------------------------------------------------------------ */
static void emit_multi_pin(FILE *out, const Component *c,
                            const char *tikz_type, int needs_mirror)
{
    double cx = c->position.x;
    double cy = c->position.y;

    int angle     = 0;
    int is_mirror = 0;
    if (c->orient[0] == 'R')
        angle = atoi(&c->orient[1]);
    else if (c->orient[0] == 'M') {
        is_mirror = 1;
        angle = atoi(&c->orient[1]);
    }

    int apply_mirror = is_mirror ^ needs_mirror;

    /* ---- Compute corrected node center from calibration ---- */
    double node_cx = cx;
    double node_cy = cy;

    const AnchorCalib *cal = get_calib(c->type);
    if (cal != NULL) {
        for (int i = 0; i < c->pin_count; i++) {
            if (strcmp(c->pins[i].name, cal->ref_pin) != 0) continue;

            double rdx = cal->tikz_dx;
            double rdy = cal->tikz_dy;

            /* Step 1: apply mirror */
            if (apply_mirror) rdx = -rdx;

            /* Step 2: rotate CCW by angle (TikZ convention) */
            double rad  = angle * PI / 180.0;
            double rdx2 = rdx * cos(rad) - rdy * sin(rad);
            double rdy2 = rdx * sin(rad) + rdy * cos(rad);

            node_cx = c->pins[i].abs_pos.x - rdx2;
            node_cy = c->pins[i].abs_pos.y - rdy2;
            break;
        }
    }

    /* ---- Emit the node ---- */
    char transform[64] = "";
    if (apply_mirror && angle != 0)
        snprintf(transform, sizeof(transform), ", rotate=%d, xscale=-1", angle);
    else if (apply_mirror)
        snprintf(transform, sizeof(transform), ", xscale=-1");
    else if (angle != 0)
        snprintf(transform, sizeof(transform), ", rotate=%d", angle);

    fprintf(out, "  \\node [%s%s] at (%.4f,%.4f) (%s) {};\n",
            tikz_type, transform, node_cx, node_cy, c->name);

    /* ---- Labels ---- */
    fprintf(out,
        "  \\node[font=\\small, above] at (%s.north) {$%s$};\n",
        c->name, c->name);
    if (c->value[0] != '\0')
        fprintf(out,
            "  \\node[font=\\small, below] at (%s.south) {$%s$};\n",
            c->name, c->value);

    /* ---- Pin coordinates + stubs ---- */
    for (int i = 0; i < c->pin_count; i++) {
        const Pin *p = &c->pins[i];

        const char *anchor = get_tikz_anchor(c->type, p->name);

        /* Coordinate at circuitikz anchor position */
        char anchor_coord[128];
        if (anchor != NULL) {
            fprintf(out, "  \\coordinate (%s_%s) at (%s.{%s});\n",
                    c->name, p->name, c->name, anchor);
            snprintf(anchor_coord, sizeof(anchor_coord),
                     "%s_%s", c->name, p->name);
        } else {
            /* No anchor map — place at abs_pos, stub is zero length */
            fprintf(out,
                "  \\coordinate (%s_%s) at (%.4f,%.4f); %% no anchor\n",
                c->name, p->name, p->abs_pos.x, p->abs_pos.y);
            snprintf(anchor_coord, sizeof(anchor_coord),
                     "%s_%s", c->name, p->name);
        }

        /* Coordinate at schematic connection point */
        char nb[16];
        node_label(p->node, nb, sizeof(nb));
        fprintf(out, "  \\coordinate (%s) at (%.4f,%.4f);\n",
                nb, p->abs_pos.x, p->abs_pos.y);

        /* Stub: circuitikz anchor → abs_pos                         */
        /* Use -| (horizontal first) or |- (vertical first) based on */
        /* dominant direction of offset                               */
        double dx = fabs(p->abs_pos.x - node_cx);
        double dy = fabs(p->abs_pos.y - node_cy);
        if (dx >= dy) {
            fprintf(out, "  \\draw (%s) -| (%.4f,%.4f);\n",
                    anchor_coord, p->abs_pos.x, p->abs_pos.y);
        } else {
            fprintf(out, "  \\draw (%s) |- (%.4f,%.4f);\n",
                    anchor_coord, p->abs_pos.x, p->abs_pos.y);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Node coordinates                                                    */
/* ------------------------------------------------------------------ */
static void emit_node_coordinates(FILE *out)
{
    fprintf(out, "\n  %% --- node coordinates ---\n");

    int emitted[MAX_POINTS];
    memset(emitted, 0, sizeof(emitted));
    for (int ci = 0; ci < component_count; ci++) {
        if (components[ci].pin_count > 2) {
            for (int pi = 0; pi < components[ci].pin_count; pi++) {
                int nid = components[ci].pins[pi].node;
                if (nid >= 0 && nid < MAX_POINTS) emitted[nid] = 1;
            }
        }
    }

    int seen[MAX_POINTS];
    memset(seen, 0, sizeof(seen));

    for (int i = 0; i < edge_count; i++) {
        int pa = edges[i].a;
        int pb = edges[i].b;
        int na = node_id[uf_find(pa)];
        int nb = node_id[uf_find(pb)];

        if (na >= 0 && na < MAX_POINTS && !emitted[na] && !seen[na]) {
            seen[na] = 1;
            fprintf(out, "  \\coordinate (n%d) at (%.4f,%.4f);\n",
                    na, points[pa].x, points[pa].y);
        }
        if (nb >= 0 && nb < MAX_POINTS && !emitted[nb] && !seen[nb]) {
            seen[nb] = 1;
            fprintf(out, "  \\coordinate (n%d) at (%.4f,%.4f);\n",
                    nb, points[pb].x, points[pb].y);
        }
    }

    for (int i = 0; i < point_count; i++) {
        int nid = node_id[uf_find(i)];
        if (nid < 0 || nid >= MAX_POINTS) continue;
        if (emitted[nid] || seen[nid]) continue;
        seen[nid] = 1;
        fprintf(out, "  \\coordinate (n%d) at (%.4f,%.4f);\n",
                nid, points[i].x, points[i].y);
    }
}

/* ------------------------------------------------------------------ */
/*  Wire emission — verbatim from LTspice edges                        */
/*                                                                      */
/*  Draws every wire segment exactly as LTspice has it.               */
/* ------------------------------------------------------------------ */
static void emit_wires(FILE *out)
{
    fprintf(out, "\n  %% --- wires ---\n");
    for (int i = 0; i < edge_count; i++) {
        int a = edges[i].a;
        int b = edges[i].b;
        fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                points[a].x, points[a].y,
                points[b].x, points[b].y);
    }
}

/* ------------------------------------------------------------------ */
/*  Junction dots                                                       */
/*                                                                      */
/*  A junction is any wire graph point with degree >= 3 (T or cross). */
/*  Filled dot emitted at these positions.                             */
/* ------------------------------------------------------------------ */
static void emit_junctions(FILE *out)
{
    fprintf(out, "\n  %% --- junctions ---\n");

    int degree[MAX_POINTS];
    memset(degree, 0, sizeof(degree));
    for (int i = 0; i < edge_count; i++) {
        degree[edges[i].a]++;
        degree[edges[i].b]++;
    }

    for (int i = 0; i < point_count; i++) {
        if (degree[i] < 3) continue;
        fprintf(out, "  \\fill (%.4f,%.4f) circle (1.5pt);\n",
                points[i].x, points[i].y);
    }
}

/* ------------------------------------------------------------------ */
/*  Floating pin stubs                                                  */
/*                                                                      */
/*  For pins with node < 0 (not connected to any wire),               */
/*  draw a short stub in the pin exit direction.                       */
/* ------------------------------------------------------------------ */
#define STUB_LENGTH 0.8

static void emit_floating_stubs(FILE *out)
{
    fprintf(out, "\n  %% --- floating pin stubs ---\n");

    for (int ci = 0; ci < component_count; ci++) {
        const Component *c = &components[ci];
        for (int pi = 0; pi < c->pin_count; pi++) {
            const Pin *p = &c->pins[pi];
            if (p->node >= 0) continue;

            /* Exit direction from component center */
            double dx = p->abs_pos.x - c->position.x;
            double dy = p->abs_pos.y - c->position.y;

            double ex, ey;
            if (fabs(dx) >= fabs(dy)) {
                ex = p->abs_pos.x + (dx >= 0 ? STUB_LENGTH : -STUB_LENGTH);
                ey = p->abs_pos.y;
            } else {
                ex = p->abs_pos.x;
                ey = p->abs_pos.y + (dy >= 0 ? STUB_LENGTH : -STUB_LENGTH);
            }

            fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                    p->abs_pos.x, p->abs_pos.y, ex, ey);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Net labels (FLAG names)                                             */
/*                                                                      */
/*  Emits a text label at each degree-1 wire endpoint that has a      */
/*  named FLAG (not "0" / ground).                                     */
/*  Requires netlabel.h/c — see earlier in the conversation.          */
/* ------------------------------------------------------------------ */
static void emit_net_labels(FILE *out)
{
    /* Placeholder — implement with netlabel.h/c to emit named flags  */
    (void)out;
}

/* ------------------------------------------------------------------ */
/*  Grounds                                                             */
/* ------------------------------------------------------------------ */
static void emit_grounds(FILE *out)
{
    fprintf(out, "\n  %% --- grounds ---\n");
    for (int i = 0; i < ground_count; i++)
        fprintf(out, "  \\node[ground] at (%.4f,%.4f) {};\n",
                grounds[i].x, grounds[i].y);
}

/* ------------------------------------------------------------------ */
/*  Top-level emitter                                                   */
/* ------------------------------------------------------------------ */
void emit_circuitikz(FILE *out)
{
#if EMITTER_STANDALONE
    fprintf(out,
        "\\documentclass[border=5pt]{standalone}\n"
        "\\usepackage{circuitikz}\n"
        "\\begin{document}\n");
#endif

    fprintf(out, "\\begin{circuitikz}\n");

    emit_node_coordinates(out);

    for (int i = 0; i < component_count; i++) {
        const Component *c = &components[i];

        const ComponentMap *map = ltspice_to_map(c->type);
        if (map == NULL) {
            fprintf(out, "  %% UNKNOWN type '%s' for '%s'\n",
                    c->type, c->name);
            continue;
        }

        fprintf(out, "\n  %% %s  (%s)\n", c->name, c->type);

        if (c->pin_count <= 2)
            emit_two_pin(out, c, map->circuitikz_name);
        else
            emit_multi_pin(out, c, map->circuitikz_name, map->needs_mirror);
    }

    emit_wires(out);
    emit_junctions(out);
    emit_floating_stubs(out);
    emit_net_labels(out);
    emit_grounds(out);

    fprintf(out, "\n\\end{circuitikz}\n");

#if EMITTER_STANDALONE
    fprintf(out, "\\end{document}\n");
#endif
}