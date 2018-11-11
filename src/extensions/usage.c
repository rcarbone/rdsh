/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *                      _     _
 *              _ __ __| |___| |__
 *             | '__/ _` / __| '_ \
 *             | | | (_| \__ \ | | |
 *             |_|  \__,_|___/_| |_|
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


/* System headers */
#include <stdio.h>

/* Project headers */
#include "rdsh.h"


/* GNU short options */
enum
{
  /* Miscellanea */
  OPT_HELP    = 'h',
  OPT_VERBOSE = 'v',

} ropt_t;


/* Display the syntax for using a builtin */
void common_usage (unsigned n, char * name, char * synopsis, char * help, struct option * options)
{
  char * b = strchr (synopsis, ' ');
  if (b)
    b ++;

  printf ("Usage: %s [options] %s\n", name, b);
  printf ("\n");

  printf ("Miscellanea:\n");
  usage_item (options, n, OPT_HELP,    "show this help message");
  usage_item (options, n, OPT_VERBOSE, "run verbosely");
  printf ("\n");
}
