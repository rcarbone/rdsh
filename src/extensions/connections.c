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



/* Project headers */
#include "rdsh.h"


/* tcsh builtin extension [auth] */
int rdsh_auth (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [echo] */
int rdsh_echo (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [ping] */
int rdsh_ping (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [quit] */
int rdsh_quit (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [select] */
int rdsh_select (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [swapdb] */
int rdsh_swapdb (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

