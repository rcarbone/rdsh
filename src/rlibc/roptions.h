/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *                   _ _ _
 *              _ __| (_) |__   ___
 *             | '__| | | '_ \ / __|
 *             | |  | | | |_) | (__
 *             |_|  |_|_|_.__/ \___|
 *
 *             rdsh, the Redis shell
 *
 * R. Carbone (rocco@tecsiel.it)
 *   4Q 2018
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */


#pragma once

/* System headers */
#include <getopt.h>

#if !defined(RMAX)
#define RMAX(a,b)  (a > b ? a : b)
#endif


#ifdef __cplusplus
extern "C" {

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

#endif /* __cplusplus */


unsigned optmax (struct option * options);
char * optlegitimate (struct option * options);
char * optname (struct option * options, unsigned n, int val);
void usage_item (struct option * options, unsigned n, unsigned val, char * description);
void _version_ (char * progname, char * version);
void _welcome_ (char * progname, char * version);


#ifdef __cplusplus
}
#endif /* __cplusplus */
