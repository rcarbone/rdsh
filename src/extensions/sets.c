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


/* tcsh builtin extension [sadd] */
int rdsh_sadd (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [scard] */
int rdsh_scard (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sdiff] */
int rdsh_sdiff (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sdiffstore] */
int rdsh_sdiffstore (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sinter] */
int rdsh_sinter (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sinterstore] */
int rdsh_sinterstore (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sismember] */
int rdsh_sismember (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [smembers] */
int rdsh_smembers (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [smove] */
int rdsh_smove (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [spop] */
int rdsh_spop (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [srandmember] */
int rdsh_srandmember (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [srem] */
int rdsh_srem (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sscan] */
int rdsh_sscan (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sunion] */
int rdsh_sunion (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

/* tcsh builtin extension [sunionstore] */
int rdsh_sunionstore (int argc, char * argv [])
{
  return rdsh_redis (argc, argv);
}

