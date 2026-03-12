#ifndef EMITTER_H
#define EMITTER_H

#include <stdio.h>

/* If 1, wraps output in a full standalone .tex file.
   If 0, emits only the circuitikz body. */
#define EMITTER_STANDALONE 1

void emit_circuitikz(FILE *out);

#endif /* EMITTER_H */
