#ifndef PARSER_H
#define PARSER_H

/* Parses an LTspice .asc file and populates the component and graph structures.
   Returns 0 on success, -1 on error. */
int parse_asc(const char *filepath);

#endif /* PARSER_H */