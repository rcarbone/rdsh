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
#include "stdio.h"

/* Project headers */
#include "rdsh.h"
#include "try-helper.h"


int main (int argc, char * argv [])
{
  argv [0] = "connect";
  return rdsh_connect (argc, argv);
}
