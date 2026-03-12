/* Emitter functions for generating TikZ code */

#include "emitter.h"
#include "component.h"
#include "component_registry.h"
#include "symbol_pin_map.h"
#include "graph.h"
#include "wire.h" 
#include <stdio.h>
#include <string.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Helpers                                                             */
/* ------------------------------------------------------------------ */

/* Returns the node label string for a given node id, e.g. "n3" */
static void node_label(int node_id_val, char *buf, int bufsz)
{
    if (node_id_val < 0)
        snprintf(buf, bufsz, "???");
    else
        snprintf(buf, bufsz, "n%d", node_id_val);
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
            tikz_type,
            c->name,
            p1->abs_pos.x, p1->abs_pos.y);
}

/* ------------------------------------------------------------------ */
/*  3+ pin emitter                                                      */
/*  \node [type] at (cx,cy) (name) {};                                 */
/*  then one coordinate node per pin so wires can attach               */
/* ------------------------------------------------------------------ */
static void emit_multi_pin(FILE *out, const Component *c, const char *tikz_type)
{
    /* Component centre in TikZ coords (already scaled, Y already flipped) */
    double cx = c->position.x;
    double cy = c->position.y;

    /* Place the component node */
    fprintf(out, "  \\node [%s] at (%.4f,%.4f) (%s) {};\n",
            tikz_type, cx, cy, c->name);

    /* Emit a named coordinate for each pin so wires can reach them */
    for (int i = 0; i < c->pin_count; i++) {
        const Pin *p = &c->pins[i];

        /* Try to get the circuitikz anchor name */
        const char *anchor = get_tikz_anchor(c->type, p->name);

        if (anchor != NULL) {
        fprintf(out, "  \\coordinate (%s_%s) at (%s.{%s});\n",c->name, p->name, c->name, anchor);
        }
        else {
            /* Fallback: place a coordinate at the absolute pin position */
            fprintf(out, "  \\coordinate (%s_%s) at (%.4f,%.4f); %% no anchor map\n",
                    c->name, p->name,
                    p->abs_pos.x, p->abs_pos.y);
        }

        /* Also emit a node-label tie so the graph node is reachable by name */
        char nb[16];
        node_label(p->node, nb, sizeof(nb));
        fprintf(out, "  \\coordinate (%s) at (%s_%s);\n",
                nb, c->name, p->name);
    }
}

/* ------------------------------------------------------------------ */
/*  Wire emitter                                                        */
/*  Emits \draw (nA) -- (nB); for every pair of points in the same    */
/*  node that are directly connected (i.e. share a union-find root     */
/*  but are distinct points). We walk the points array and for each    */
/*  point emit a segment to its direct parent to reconstruct the tree. */
/* ------------------------------------------------------------------ */
static void emit_wires(FILE *out)
{
    fprintf(out, "\n  %% --- wires ---\n");

    for (int i = 0; i < wire_count; i++) {
        fprintf(out,
                "  \\draw (%.4f,%.4f) -- (%.4f,%.4f);\n",
                wires[i].p1.x, wires[i].p1.y,
                wires[i].p2.x, wires[i].p2.y);
    }
}

static void emit_pin_stubs(FILE *out)
{
    fprintf(out, "\n  %% --- pin stubs ---\n");

    for (int ci = 0; ci < component_count; ci++) {
        const Component *c = &components[ci];
        if (c->pin_count <= 2) continue;

        for (int pi = 0; pi < c->pin_count; pi++) {
            const Pin *p = &c->pins[pi];
            if (p->node < 0) continue;

            /* Find the wire endpoint with the same node that is
               closest to this pin's absolute position */
            double best_dist = 1e18;
            double best_x = 0, best_y = 0;
            int found = 0;

            for (int wi = 0; wi < wire_count; wi++) {
                int wn1 = node_id[uf_find(wires[wi].point1_index)];
                int wn2 = node_id[uf_find(wires[wi].point2_index)];

                double wx, wy;
                int match = 0;

                if (wn1 == p->node) {
                    wx = wires[wi].p1.x;
                    wy = wires[wi].p1.y;
                    match = 1;
                } else if (wn2 == p->node) {
                    wx = wires[wi].p2.x;
                    wy = wires[wi].p2.y;
                    match = 1;
                }

                if (match) {
                    double dx = wx - p->abs_pos.x;
                    double dy = wy - p->abs_pos.y;
                    double dist = dx*dx + dy*dy;
                    if (dist < best_dist) {
                        best_dist = dist;
                        best_x = wx;
                        best_y = wy;
                        found = 1;
                    }
                }
            }

            if (found) {
                char anchor[128];
                snprintf(anchor, sizeof(anchor), "%s_%s", c->name, p->name);
                fprintf(out,"  \\draw (%s) -- (%.4f,%.4f);\n", anchor, best_x, best_y);
            }
        }
    }
}

static void emit_node_coordinates(FILE *out)
{
    fprintf(out, "\n  %% --- node coordinates ---\n");

    /* Mark nodes already claimed by multi-pin component pins */
    int emitted[MAX_POINTS];
    memset(emitted, 0, sizeof(emitted));

    for (int ci = 0; ci < component_count; ci++) {
        if (components[ci].pin_count > 2) {
            for (int pi = 0; pi < components[ci].pin_count; pi++) {
                int nid = components[ci].pins[pi].node;
                if (nid >= 0 && nid < MAX_POINTS)
                    emitted[nid] = 1;
            }
        }
    }

    /* Emit a coordinate at every wire endpoint that isn't already
       covered by a multi-pin component anchor */
    int seen[MAX_POINTS];
    memset(seen, 0, sizeof(seen));

    for (int i = 0; i < wire_count; i++) {
        /* p1 of wire */
        int idx1 = wires[i].point1_index;
        int nid1 = node_id[uf_find(idx1)];
        if (nid1 >= 0 && nid1 < MAX_POINTS && !emitted[nid1] && !seen[nid1]) {
            seen[nid1] = 1;
            fprintf(out, "  \\coordinate (n%d) at (%.4f,%.4f);\n",nid1, wires[i].p1.x, wires[i].p1.y);
        }

        /* p2 of wire */
        int idx2 = wires[i].point2_index;
        int nid2 = node_id[uf_find(idx2)];
        if (nid2 >= 0 && nid2 < MAX_POINTS && !emitted[nid2] && !seen[nid2]) {
            seen[nid2] = 1;
            fprintf(out, "  \\coordinate (n%d) at (%.4f,%.4f);\n",
                    nid2, wires[i].p2.x, wires[i].p2.y);
        }
    }

    /* Also catch any remaining unanchored component pin nodes
       not covered by wires (e.g. floating pins) */
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

    /* ---- node coordinates first so all names exist before use ---- */
    emit_node_coordinates(out);

    /* ---- components ---- */
    for (int i = 0; i < component_count; i++) {
        const Component *c = &components[i];

        const char *tikz_type = ltspice_to_circuitikz(c->type);
        if (tikz_type == NULL) {
            fprintf(out, "  %% UNKNOWN type '%s' for component '%s'\n",
                    c->type, c->name);
            continue;
        }

        fprintf(out, "\n  %% %s  (%s)\n", c->name, c->type);

        if (c->pin_count <= 2)
            emit_two_pin(out, c, tikz_type);
        else
            emit_multi_pin(out, c, tikz_type);
    }

    /* ---- wires ---- */
    emit_wires(out);
    emit_pin_stubs(out);

    fprintf(out, "\n\\end{circuitikz}\n");

#if EMITTER_STANDALONE
    fprintf(out, "\\end{document}\n");
#endif
}