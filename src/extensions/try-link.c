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


/*
 * Does nothing, but tries to link static library.
 *
 * The functions have been written only to test
 * if a binary program can be generated at compile time.
 *
 */


/* System headers */
#include "stdio.h"
#include "stdarg.h"

/* Project headers */
#include "rdsh.h"
#include "try-helper.h"


static void rdsh_server (int argc, char * argv [])
{
  rdsh_bgrewriteaof (argc, argv);
  rdsh_bgsave (argc, argv);
  rdsh_client_getname (argc, argv);
  rdsh_client_id (argc, argv);
  rdsh_client_kill (argc, argv);
  rdsh_client_list (argc, argv);
  rdsh_client_pause (argc, argv);
  rdsh_client_reply (argc, argv);
  rdsh_client_setname (argc, argv);
  rdsh_client_unblock (argc, argv);
  rdsh_command (argc, argv);
  rdsh_command_count (argc, argv);
  rdsh_command_getkeys (argc, argv);
  rdsh_command_info (argc, argv);
  rdsh_config_get (argc, argv);
  rdsh_config_resetstat (argc, argv);
  rdsh_config_rewrite (argc, argv);
  rdsh_config_set (argc, argv);
  rdsh_dbsize (argc, argv);
  rdsh_debug_object (argc, argv);
  rdsh_flushall (argc, argv);
  rdsh_flushdb (argc, argv);
  rdsh_info (argc, argv);
  rdsh_lastsave (argc, argv);
  rdsh_memory_doctor (argc, argv);
  rdsh_memory_help (argc, argv);
  rdsh_memory_malloc_stats (argc, argv);
  rdsh_memory_purge (argc, argv);
  rdsh_memory_stats (argc, argv);
  rdsh_memory_usage (argc, argv);
  rdsh_monitor (argc, argv);
  rdsh_role (argc, argv);
  rdsh_save (argc, argv);
  rdsh_sync (argc, argv);
  rdsh_time (argc, argv);
}


static void rdsh_connections (int argc, char * argv [])
{
  rdsh_auth (argc, argv);
  rdsh_echo (argc, argv);
  rdsh_ping (argc, argv);
  rdsh_quit (argc, argv);
  rdsh_select (argc, argv);
  rdsh_swapdb (argc, argv);
}


static void rdsh_data_structures (int argc, char * argv [])
{
  rdsh_del (argc, argv);
  rdsh_dump (argc, argv);
  rdsh_exists (argc, argv);
  rdsh_expire (argc, argv);
  rdsh_expireat (argc, argv);
  rdsh_keys (argc, argv);
  rdsh_migrate (argc, argv);
  rdsh_move (argc, argv);
  rdsh_object (argc, argv);
  rdsh_persist (argc, argv);
  rdsh_pexpire (argc, argv);
  rdsh_pexpireat (argc, argv);
  rdsh_pttl (argc, argv);
  rdsh_randomkey (argc, argv);
  rdsh_rename (argc, argv);
  rdsh_renamenx (argc, argv);
  rdsh_restore (argc, argv);
  rdsh_sort (argc, argv);
  rdsh_touch (argc, argv);
  rdsh_ttl (argc, argv);
  rdsh_type (argc, argv);
  rdsh_unlink (argc, argv);
  rdsh_wait (argc, argv);
  rdsh_scan (argc, argv);
}


static void rdsh_strings (int argc, char * argv [])
{
  rdsh_append (argc, argv);
  rdsh_bitcount (argc, argv);
  rdsh_bitfield (argc, argv);
  rdsh_bitop (argc, argv);
  rdsh_bitpos (argc, argv);
  rdsh_decr (argc, argv);
  rdsh_decrby (argc, argv);
  rdsh_get (argc, argv);
  rdsh_getbit (argc, argv);
  rdsh_getrange (argc, argv);
  rdsh_getset (argc, argv);
  rdsh_incr (argc, argv);
  rdsh_incrby (argc, argv);
  rdsh_incrbyfloat (argc, argv);
  rdsh_mget (argc, argv);
  rdsh_mset (argc, argv);
  rdsh_msetnx (argc, argv);
  rdsh_psetex (argc, argv);
  rdsh_set (argc, argv);
  rdsh_setbit (argc, argv);
  rdsh_setex (argc, argv);
  rdsh_setrange (argc, argv);
  rdsh_setnx (argc, argv);
  rdsh_strlen (argc, argv);
}


static void rdsh_hashes (int argc, char * argv [])
{
  rdsh_hdel (argc, argv);
  rdsh_hexists (argc, argv);
  rdsh_hget (argc, argv);
  rdsh_hgetall (argc, argv);
  rdsh_hincrby (argc, argv);
  rdsh_hincrbyfloat (argc, argv);
  rdsh_hkeys (argc, argv);
  rdsh_hlen (argc, argv);
  rdsh_hmget (argc, argv);
  rdsh_hmset (argc, argv);
  rdsh_hscan (argc, argv);
  rdsh_hset (argc, argv);
  rdsh_hsetnx (argc, argv);
  rdsh_hstrlen (argc, argv);
  rdsh_hvals (argc, argv);
}


static void rdsh_lists (int argc, char * argv [])
{
  rdsh_blpop (argc, argv);
  rdsh_brpop (argc, argv);
  rdsh_brpoplpush (argc, argv);
  rdsh_lindex (argc, argv);
  rdsh_linsert (argc, argv);
  rdsh_llen (argc, argv);
  rdsh_lpop (argc, argv);
  rdsh_lpush (argc, argv);
  rdsh_lpushx (argc, argv);
  rdsh_lrange (argc, argv);
  rdsh_lrem (argc, argv);
  rdsh_lset (argc, argv);
  rdsh_ltrim (argc, argv);
  rdsh_rpop (argc, argv);
  rdsh_rpoplpush (argc, argv);
  rdsh_rpush (argc, argv);
  rdsh_rpushx (argc, argv);
}


static void rdsh_sets (int argc, char * argv [])
{
  rdsh_sadd (argc, argv);
  rdsh_scard (argc, argv);
  rdsh_sdiff (argc, argv);
  rdsh_sdiffstore (argc, argv);
  rdsh_sinter (argc, argv);
  rdsh_sinterstore (argc, argv);
  rdsh_sismember (argc, argv);
  rdsh_smembers (argc, argv);
  rdsh_smove (argc, argv);
  rdsh_spop (argc, argv);
  rdsh_srandmember (argc, argv);
  rdsh_srem (argc, argv);
  rdsh_sscan (argc, argv);
  rdsh_sunion (argc, argv);
  rdsh_sunionstore (argc, argv);
}


static void rdsh_sorted_sets (int argc, char * argv [])
{
  rdsh_bzpopmax (argc, argv);
  rdsh_bzpopmin (argc, argv);
  rdsh_zadd (argc, argv);
  rdsh_zcard (argc, argv);
  rdsh_zcount (argc, argv);
  rdsh_zincrby (argc, argv);
  rdsh_zinterstore (argc, argv);
  rdsh_zlexcount (argc, argv);
  rdsh_zpopmax (argc, argv);
  rdsh_zpopmin (argc, argv);
  rdsh_zrange (argc, argv);
  rdsh_zrangebylex (argc, argv);
  rdsh_zrangebyscore (argc, argv);
  rdsh_zrank (argc, argv);
  rdsh_zrem (argc, argv);
  rdsh_zremrangebylex (argc, argv);
  rdsh_zremrangebyrank (argc, argv);
  rdsh_zremrangebyscore (argc, argv);
  rdsh_zrevrange (argc, argv);
  rdsh_zrevrangebylex (argc, argv);
  rdsh_zrevrangebyscore (argc, argv);
  rdsh_zrevrank (argc, argv);
  rdsh_zscan (argc, argv);
  rdsh_zscore (argc, argv);
  rdsh_zunionstore (argc, argv);
}


int main (int argc, char * argv [])
{
  printf ("This program does nothing, but it only tests if link works at compile time. Bye bye!\n");

  if (argc == 0)
    {
      /* Never executed, so no check is done about possible failures. */
      rdsh_server (argc, argv);
      rdsh_connections (argc, argv);
      rdsh_data_structures (argc, argv);
      rdsh_strings (argc, argv);
      rdsh_hashes (argc, argv);
      rdsh_lists (argc, argv);
      rdsh_sets (argc, argv);
      rdsh_sorted_sets (argc, argv);
    }

  return 0;
}
