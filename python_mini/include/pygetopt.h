#pragma once

extern DL_IMPORT(int) _PyOS_opterr;
extern DL_IMPORT(int) _PyOS_optind;
extern DL_IMPORT(char *) _PyOS_optarg;

DL_IMPORT(int) _PyOS_GetOpt(int argc, char **argv, char *optstring);
