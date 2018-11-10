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


#pragma once


/* hiredis headers */
#include "hiredis.h"


#define DEFAULT_HOST     "localhost"
#define DEFAULT_PORT     6379
#define DEFAULT_AUTH     NULL
#define DEFAULT_TIMEOUT  1500


/* Redis server descriptor */
typedef struct
{
  rtime_t updated;         /* last updated at nsec resolution */

  char * host;             /* both hostname or IP address     */
  unsigned port;           /* IPv4 port                       */
  unsigned timeout;        /* connection/reply timeout        */

  char * name;
  char * auth;
  char * error;            /* a buffer where to store underlaying errors */

  rtime_t connected;       /* time connection has established at nsec resolution */
  char * version;          /* server version */

  redisContext * rds;      /* pointer to underlaying connection handler */

} rdsh_t;


/* Prototypes */

char     * redis_host (rdsh_t * redis);
unsigned   redis_port (rdsh_t * redis);
char     * redis_name (rdsh_t * redis);
rtime_t    redis_connected (rdsh_t * redis);
char     * redis_version (rdsh_t * redis);
bool       redis_status (rdsh_t * redis);

rdsh_t *   redis_connect (char * host, unsigned port, unsigned timeout);
void       redis_disconnect (rdsh_t * redis);
void       redis_run (redisContext * rds, char * prog, unsigned argc, char * argv []);
