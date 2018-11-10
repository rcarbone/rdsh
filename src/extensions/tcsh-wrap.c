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


/* tcsh header file */
#include "sh.h"

/* Project headers */
#include "rdsh.h"

/* Avoid warning: 'tcsh_xxx' defined but not used [-Wunused-function] */
#if defined(__GNUC__)
#pragma GCC diagnostic ignored   "-Wunused-function"
#else /* defined(__clang__) */
#pragma clang diagnostic ignored "-Wunused-function"
#endif


typedef int handler (int argc, char * argv []);


/* How to call the [rdsh] extension functions from tcsh */
static void tcsh_xxx (Char ** v, handler * func)
{
  Char ** vv = v;                      /* an interator in the 'v' array */

  /* insert command name */
  char ** argv = argsmore (NULL, short2str (* vv ++));

  while (* vv)
    argv = argsmore (argv, short2str (* vv ++));

  /* It's time to execute the function */
  if ((* func) (argslen (argv), argv))
    setcopy (STRstatus, Strsave (STR1), VAR_READWRITE);  /* set the $status variable */
}


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * Do not edit anything below, configure creates it.
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* Definitions for builtin extensions to the shell will be automatically inserted here by the configure script */
