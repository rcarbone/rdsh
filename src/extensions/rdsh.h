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

/* The name of the game */
#define RDSH_VERSION   "0.1.0"
#define RDSH_AUTHOR    "R. Carbone (rocco@tecsiel.it)"
#define RDSH_RELEASED  __DATE__

/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <libgen.h>
#include <getopt.h>

/* Project headers */
#include "rctype.h"
#include "safe.h"
#include "vargv.h"
#include "sargv.h"
#include "support.h"
#include "rwall.h"
#include "interval.h"
#include "elapsed.h"
#include "roptions.h"

/* Redis headers */
#include "hiredis-wrap.h"

/*
 * Great Total: 131 commands implemented as tcsh builtin extensions
 *              131 = 5 + 6 + 24 + 24 + 15 + 17 + 15 + 25
 *   5 - rdsh sessions
 *   6 - Redis connections
 *  24 - Redis data structures
 *  24 - Redis strings
 *  15 - Redis hashes
 *  17 - Redis lists
 *  15 - Redis sets
 *  25 - Redis sorted sets
 */


/* GNU short options */
enum
{
  /* Admin (currently implemented #4 of #4 defined) */
  CMD_HELP        = 1,
  CMD_SESSIONS,
  CMD_CONNECT,
  CMD_DISCONNECT,
  CMD_CHANGE,

  /* Connections (currently implemented #6 of #6 defined) */
  CMD_AUTH        = 31,
  CMD_ECHO,
  CMD_PING,
  CMD_QUIT,
  CMD_SELECT,
  CMD_SWAPDB,

  /* Data Structures (currently implemented #24 of #24 defined) */
  CMD_DEL         = 61,
  CMD_DUMP,
  CMD_EXISTS,
  CMD_EXPIRE,
  CMD_EXPIREAT,
  CMD_KEYS,
  CMD_MIGRATE,
  CMD_MOVE,
  CMD_OBJECT,
  CMD_PERSIST,
  CMD_PEXPIRE,
  CMD_PEXPIREAT,
  CMD_PTTL,
  CMD_RANDOMKEY,
  CMD_RENAME,
  CMD_RENAMENX,
  CMD_RESTORE,
  CMD_SCAN,
  CMD_SORT,
  CMD_TOUCH,
  CMD_TTL,
  CMD_TYPE,
  CMD_UNLINK,
  CMD_WAIT,

  /* Strings (currently implemented #24 of #24 defined) */
  CMD_APPEND      = 91,
  CMD_BITCOUNT,
  CMD_BITFIELD,
  CMD_BITOP,
  CMD_BITPOS,
  CMD_DECR,
  CMD_DECRBY,
  CMD_GET,
  CMD_GETBIT,
  CMD_GETRANGE,
  CMD_GETSET,
  CMD_INCR,
  CMD_INCRBY,
  CMD_INCRBYFLOAT,
  CMD_MGET,
  CMD_MSET,
  CMD_MSETNX,
  CMD_PSETEX,
  CMD_SET,
  CMD_SETBIT,
  CMD_SETEX,
  CMD_SETRANGE,
  CMD_SETNX,
  CMD_STRLEN,

  /* Hashes (currently implemented #15 of #15 defined) */
  CMD_HDEL        = 121,
  CMD_HEXISTS,
  CMD_HGET,
  CMD_HGETALL,
  CMD_HINCRBY,
  CMD_HINCRBYFLOAT,
  CMD_HKEYS,
  CMD_HLEN,
  CMD_HMGET,
  CMD_HMSET,
  CMD_HSCAN,
  CMD_HSET,
  CMD_HSETNX,
  CMD_HSTRLEN,
  CMD_HVALS,

  /* Lists (currently implemented #17 of #17 defined) */
  CMD_BLPOP       = 151,
  CMD_BRPOP,
  CMD_BRPOPLPUSH,
  CMD_LINDEX,
  CMD_LINSERT,
  CMD_LLEN,
  CMD_LPOP,
  CMD_LPUSH,
  CMD_LPUSHX,
  CMD_LRANGE,
  CMD_LREM,
  CMD_LTRIM,
  CMD_LSET,
  CMD_RPOP,
  CMD_RPOPLPUSH,
  CMD_RPUSH,
  CMD_RPUSHX,

  /* Sets (currently implemented #15 of #15 defined) */
  CMD_SADD        = 181,
  CMD_SCARD,
  CMD_SDIFF,
  CMD_SDIFFSTORE,
  CMD_SINTER,
  CMD_SINTERSTORE,
  CMD_SISMEMBER,
  CMD_SMEMBERS,
  CMD_SMOVE,
  CMD_SPOP,
  CMD_SRANDMEMBER,
  CMD_SREM,
  CMD_SSCAN,
  CMD_SUNION,
  CMD_SUNIONSTORE,

  /* Sorted Sets (currently implemented #25 of #25 defined) */
  CMD_BZPOPMAX    = 201,
  CMD_BZPOPMIN,
  CMD_ZADD,
  CMD_ZCARD,
  CMD_ZCOUNT,
  CMD_ZINCRBY,
  CMD_ZINTERSTORE,
  CMD_ZLEXCOUNT,
  CMD_ZPOPMAX,
  CMD_ZPOPMIN,
  CMD_ZRANGE,
  CMD_ZRANGEBYLEX,
  CMD_ZRANGEBYSCORE,
  CMD_ZRANK,
  CMD_ZREM,
  CMD_ZREMRANGEBYLEX,
  CMD_ZREMRANGEBYRANK,
  CMD_ZREMRANGEBYSCORE,
  CMD_ZREVRANGE,
  CMD_ZREVRANGEBYLEX,
  CMD_ZREVRANGEBYSCORE,
  CMD_ZREVRANK,
  CMD_ZSCAN,
  CMD_ZSCORE,
  CMD_ZUNIONSTORE,

};



/* The structure contains information on the builtins the application can understand */
typedef struct
{
  unsigned id;        /* unique numeric builtin identifier  */
  char * name;        /* user printable name of the builtin */
  char * command;     /* Redis command name                 */

  unsigned args;      /* minimum # of mandatory arguments   */
  char * synopsis;    /* synopsis string                    */
  char * help;        /* help string                        */

} builtin_t;


/* Define a sorting function */
typedef int sf (const void * _a, const void * _b);


/* A global variable defined and initialized in init.c */
extern struct timeval boottime;

/* Public functions in file init.c */
void rdsh_init (char * progname);

/* Public functions in file prompt.c */
void rdsh_prompt (char * text);

/* Public functions in file help.c */
builtin_t * builtin_by_id (unsigned id);
builtin_t * builtin (char * name);

/* Public functions in file sessions.c */
rdsh_t * active_session (void);
rdsh_t * previous_session (void);
rdsh_t * lastest_session (void);
void setactive_session (rdsh_t * redis);
void resetactive_session (rdsh_t * redis);
char * getredisname (void);
unsigned sessionslen (void);
rdsh_t ** getsessions (void);
void moresession (rdsh_t * redis);
void lesssession (rdsh_t * redis);

/* Public functions in file commands.c */
int rdsh_command (int argc, char * argv []);

/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * Do not edit anything below, configure creates it.
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

/* Definitions for builtin extensions to the shell will be automatically inserted here by the configure script */
