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
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

/* Project headers */
#include "rdsh.h"

extern int xprintf (const char *, ...);


/* Global variable here.  Sorry! */
struct timeval boottime;


/* Identifiers */
static char __version__ []  = RDSH_VERSION;
static char __authors__ []  = RDSH_AUTHOR;
static char __released__ [] = RDSH_RELEASED;
static char __id__ []       = "A hack of the popular 'tcsh' with extensions for Redis-client.\n";


/* You are welcome! */
static void helloworld (char * program)
{
  static bool once = false;

  if (! once)
    {
      xprintf ("\n");
      xprintf ("-- %s %s (%s) -- %s\n", program, __version__, __released__, __authors__);
      xprintf ("%s\n", __id__);

      once = true;
    }
}


/* Just few initialization steps */
void rdsh_init (char * progname)
{
  /* Ignore writes to connections that have been closed at the other end */
  signal (SIGPIPE, SIG_IGN);

  /* Set time the shell boots */
  gettimeofday (& boottime, NULL);

  /* Hello world! this is [rdsh] speaking */
  helloworld (progname);

  xprintf ("Type 'help' for the list of builtins implemented by this shell.\n\n");

  /* Set the prompt */
  rdsh_prompt (NULL);
}
