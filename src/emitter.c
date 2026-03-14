/* Emitter functions for generating TikZ code */

#include "emitter.h"
#include "component.h"
#include "component_registry.h"
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
/*  2-pin emitter                                                       */
/* ------------------------------------------------------------------ */
static void emit_two_pin(FILE *out, const Component *c, const char *tikz_type)
{
    const Pin *p0 = &c->pins[0];
    const Pin *p1 = &c->pins[1];
    fprintf(out, "  \\draw (%.4f,%.4f) to[%s, l=$%s$] (%.4f,%.4f);\n",
            p0->abs_pos.x, p0->abs_pos.y,
            tikz_type, c->name,
            p1->abs_pos.x, p1->abs_pos.y);
}


/* ------------------------------------------------------------------ */
/*  Multi-pin emitter                                                   */
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

    char transform[64] = "";
    if (apply_mirror && angle != 0)
        snprintf(transform, sizeof(transform), ", rotate=%d, xscale=-1", angle);
    else if (apply_mirror)
        snprintf(transform, sizeof(transform), ", xscale=-1");
    else if (angle != 0)
        snprintf(transform, sizeof(transform), ", rotate=%d", angle);

    fprintf(out, "  \\node [%s%s] at (%.4f,%.4f) (%s) {};\n",
            tikz_type, transform, node_cx, node_cy, c->name);

    if (c->value[0] != '\0')
        fprintf(out, "  \\node[right, font=\\small] at (%s.east) {$%s$};\n",
                c->name, c->value);

    /* Pin coordinates always at abs_pos — independent of node placement */
    for (int i = 0; i < c->pin_count; i++) {
        const Pin *p = &c->pins[i];
        fprintf(out, "  \\coordinate (%s_%s) at (%.4f,%.4f);\n",
                c->name, p->name, p->abs_pos.x, p->abs_pos.y);
        char nb[16];
        node_label(p->node, nb, sizeof(nb));
        fprintf(out, "  \\coordinate (%s) at (%.4f,%.4f);\n",
                nb, p->abs_pos.x, p->abs_pos.y);
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
/*  Manhattan wire router                                               */
/* ------------------------------------------------------------------ */
static void emit_manhattan(FILE *out,
                            double x1, double y1,
                            double x2, double y2,
                            int bend_horizontal_first)
{
    if (double_equal(y1, y2) || double_equal(x1, x2)) {
        fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                x1, y1, x2, y2);
        return;
    }

    double bx, by;
    if (bend_horizontal_first) {
        bx = x2; by = y1;
    } else {
        bx = x1; by = y2;
    }

    fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f) -- (%.4f,%.4f);\n",
            x1, y1, bx, by, x2, y2);
}

/* ------------------------------------------------------------------ */
/*  Heuristic: choose bend direction from pin exit direction           */
/* ------------------------------------------------------------------ */
static int choose_bend(const Component *c, const Pin *p)
{
    if (strcmp(p->name, "B")   == 0) return 0;
    if (strcmp(p->name, "G")   == 0) return 0;
    if (strcmp(p->name, "In+") == 0) return 0;
    if (strcmp(p->name, "In-") == 0) return 0;

    double dx = fabs(p->abs_pos.x - c->position.x);
    double dy = fabs(p->abs_pos.y - c->position.y);
    if (dx > dy) return 0;
    return 1;
}

/* ------------------------------------------------------------------ */
/*  Helper: is point i a component pin?                                */
/* ------------------------------------------------------------------ */
static int point_is_pin(int i)
{
    for (int ci = 0; ci < component_count; ci++) {
        for (int pi = 0; pi < components[ci].pin_count; pi++) {
            double dx = points[i].x - components[ci].pins[pi].abs_pos.x;
            double dy = points[i].y - components[ci].pins[pi].abs_pos.y;
            if (dx*dx + dy*dy < 1e-10) return 1;
        }
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Stub length — one LTspice grid unit in TikZ coords                 */
/* ------------------------------------------------------------------ */
#define STUB_LENGTH 0.8

static void pin_stub_endpoint(const Component *c, const Pin *p,
                               double *ex, double *ey)
{
    double dx = p->abs_pos.x - c->position.x;
    double dy = p->abs_pos.y - c->position.y;

    if (fabs(dx) >= fabs(dy)) {
        *ex = p->abs_pos.x + (dx >= 0 ? STUB_LENGTH : -STUB_LENGTH);
        *ey = p->abs_pos.y;
    } else {
        *ex = p->abs_pos.x;
        *ey = p->abs_pos.y + (dy >= 0 ? STUB_LENGTH : -STUB_LENGTH);
    }
}

/* ------------------------------------------------------------------ */
/*  Connection algorithm with Manhattan routing + stubs                 */
/* ------------------------------------------------------------------ */
static void emit_connections(FILE *out)
{
    fprintf(out, "\n  %% --- connections ---\n");

    /* ---- Part 1: component pin connections (Manhattan routed) ---- */
    for (int ci = 0; ci < component_count; ci++) {
        const Component *c = &components[ci];

        for (int pi = 0; pi < c->pin_count; pi++) {
            const Pin *p = &c->pins[pi];

            /* Floating pin — draw a stub */
            if (p->node < 0) {
                double ex, ey;
                pin_stub_endpoint(c, p, &ex, &ey);
                fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                        p->abs_pos.x, p->abs_pos.y, ex, ey);
                continue;
            }

            double px = p->abs_pos.x;
            double py = p->abs_pos.y;

            double best_dist = 1e18;
            double best_x    = 0, best_y = 0;
            int    found     = 0;

            for (int i = 0; i < point_count; i++) {
                if (node_id[uf_find(i)] != p->node) continue;

                double dx   = points[i].x - px;
                double dy   = points[i].y - py;
                double dist = dx*dx + dy*dy;

                if (dist < 1e-10) continue;

                if (c->pin_count == 2) {
                    int is_own_pin = 0;
                    for (int pj = 0; pj < c->pin_count; pj++) {
                        if (pj == pi) continue;
                        double ddx = points[i].x - c->pins[pj].abs_pos.x;
                        double ddy = points[i].y - c->pins[pj].abs_pos.y;
                        if (ddx*ddx + ddy*ddy < 1e-10) {
                            is_own_pin = 1;
                            break;
                        }
                    }
                    if (is_own_pin) continue;
                }

                if (dist < best_dist) {
                    best_dist = dist;
                    best_x    = points[i].x;
                    best_y    = points[i].y;
                    found     = 1;
                }
            }

            if (found && best_dist > 1e-8) {
                int bend = choose_bend(c, p);
                emit_manhattan(out, px, py, best_x, best_y, bend);
            } else if (!found) {
                double ex, ey;
                pin_stub_endpoint(c, p, &ex, &ey);
                fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                        p->abs_pos.x, p->abs_pos.y, ex, ey);
            }
        }
    }

    /* ---- Part 2: wire-only segments (no component pin on either end) ---- */
    fprintf(out, "\n  %% --- wire segments ---\n");
    for (int i = 0; i < edge_count; i++) {
        int a = edges[i].a;
        int b = edges[i].b;

        if (!point_is_pin(a) && !point_is_pin(b)) {
            fprintf(out, "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                    points[a].x, points[a].y,
                    points[b].x, points[b].y);
        }
    }
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

    emit_connections(out);
    emit_grounds(out);

    fprintf(out, "\n\\end{circuitikz}\n");

#if EMITTER_STANDALONE
    fprintf(out, "\\end{document}\n");
#endif
}