#include "parser.h"
#include "emitter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Replace the last extension of `src` with `new_ext`, write to `dst`. */
static void replace_extension(const char *src, const char *new_ext,
                               char *dst, size_t dstsz)
{
    strncpy(dst, src, dstsz - 1);
    dst[dstsz - 1] = '\0';

    /* Find the last '.' after the last path separator */
    char *last_dot   = strrchr(dst, '.');
    char *last_slash = strrchr(dst, '/');
#ifdef _WIN32
    char *last_bslash = strrchr(dst, '\\');
    if (last_bslash && (!last_slash || last_bslash > last_slash))
        last_slash = last_bslash;
#endif
    if (last_dot && (!last_slash || last_dot > last_slash))
        *last_dot = '\0';   /* truncate at the dot */

    strncat(dst, new_ext, dstsz - strlen(dst) - 1);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <circuit.asc>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_path = argv[1];

    /* Derive output path: circuit.asc → circuit.tex */
    char output_path[512];
    replace_extension(input_path, ".tex", output_path, sizeof(output_path));

    /* ---- Parse ---- */
    if (parse_asc(input_path) != 0) {
        fprintf(stderr, "ERROR: failed to parse '%s'\n", input_path);
        return EXIT_FAILURE;
    }

    /* ---- Emit ---- */
    FILE *out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "ERROR: cannot open output file '%s'\n", output_path);
        return EXIT_FAILURE;
    }

    emit_circuitikz(out);
    fclose(out);

    printf("Written: %s\n", output_path);
    return EXIT_SUCCESS;
}

/*
The full pipeline in order is:
```
parse_asc()
  ├── SYMBOL lines      → create_component()  +  add_pin()
  ├── WIRE lines        → find_or_create_point()  +  uf_union()
  └── FLAG lines        → find_or_create_point()
  │
  ├── transform_component_pins()   local coords → absolute coords
  ├── assign_nodes()               union-find   → integer node IDs
  └── attach_pins_to_nodes()       pins         → node IDs

emit_circuitikz()
  ├── 2-pin  → \draw (nA) to[resistor, l=$R1=10k$] (nB);
  ├── 3+ pin → \node [npn] at (x,y) (Q1) {};  + \coordinate per pin
  └── wires  → \draw (x1,y1) -- (x2,y2);
*/