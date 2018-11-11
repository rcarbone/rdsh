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

/* Public functions in file server.c */
int rdsh_bgrewriteaof (int argc, char * argv []);
int rdsh_bgsave (int argc, char * argv []);
int rdsh_client_getname (int argc, char * argv []);
int rdsh_client_id (int argc, char * argv []);
int rdsh_client_kill (int argc, char * argv []);
int rdsh_client_list (int argc, char * argv []);
int rdsh_client_pause (int argc, char * argv []);
int rdsh_client_reply (int argc, char * argv []);
int rdsh_client_setname (int argc, char * argv []);
int rdsh_client_unblock (int argc, char * argv []);
int rdsh_cmd (int argc, char * argv []);
int rdsh_command_count (int argc, char * argv []);
int rdsh_command_getkeys (int argc, char * argv []);
int rdsh_command_info (int argc, char * argv []);
int rdsh_config_get (int argc, char * argv []);
int rdsh_config_resetstat (int argc, char * argv []);
int rdsh_config_rewrite (int argc, char * argv []);
int rdsh_config_set (int argc, char * argv []);
int rdsh_dbsize (int argc, char * argv []);
int rdsh_debug_object (int argc, char * argv []);
int rdsh_flushall (int argc, char * argv []);
int rdsh_flushdb (int argc, char * argv []);
int rdsh_info (int argc, char * argv []);
int rdsh_lastsave (int argc, char * argv []);
int rdsh_memory_doctor (int argc, char * argv []);
int rdsh_memory_help (int argc, char * argv []);
int rdsh_memory_malloc_stats (int argc, char * argv []);
int rdsh_memory_purge (int argc, char * argv []);
int rdsh_memory_stats (int argc, char * argv []);
int rdsh_memory_usage (int argc, char * argv []);
int rdsh_monitor (int argc, char * argv []);
int rdsh_role (int argc, char * argv []);
int rdsh_save (int argc, char * argv []);
int rdsh_sync (int argc, char * argv []);
int rdsh_time (int argc, char * argv []);

/* Public functions in file connections.c */
int rdsh_auth (int argc, char * argv []);
int rdsh_echo (int argc, char * argv []);
int rdsh_ping (int argc, char * argv []);
int rdsh_quit (int argc, char * argv []);
int rdsh_select (int argc, char * argv []);
int rdsh_swapdb (int argc, char * argv []);

/* Public functions in file data-structures.c */
int rdsh_del (int argc, char * argv []);
int rdsh_dump (int argc, char * argv []);
int rdsh_exists (int argc, char * argv []);
int rdsh_expire (int argc, char * argv []);
int rdsh_expireat (int argc, char * argv []);
int rdsh_keys (int argc, char * argv []);
int rdsh_migrate (int argc, char * argv []);
int rdsh_move (int argc, char * argv []);
int rdsh_object (int argc, char * argv []);
int rdsh_persist (int argc, char * argv []);
int rdsh_pexpire (int argc, char * argv []);
int rdsh_pexpireat (int argc, char * argv []);
int rdsh_pttl (int argc, char * argv []);
int rdsh_randomkey (int argc, char * argv []);
int rdsh_rename (int argc, char * argv []);
int rdsh_renamenx (int argc, char * argv []);
int rdsh_restore (int argc, char * argv []);
int rdsh_sort (int argc, char * argv []);
int rdsh_touch (int argc, char * argv []);
int rdsh_ttl (int argc, char * argv []);
int rdsh_type (int argc, char * argv []);
int rdsh_unlink (int argc, char * argv []);
int rdsh_wait (int argc, char * argv []);
int rdsh_scan (int argc, char * argv []);

/* Public functions in file strings.c */
int rdsh_append (int argc, char * argv []);
int rdsh_bitcount (int argc, char * argv []);
int rdsh_bitfield (int argc, char * argv []);
int rdsh_bitop (int argc, char * argv []);
int rdsh_bitpos (int argc, char * argv []);
int rdsh_decr (int argc, char * argv []);
int rdsh_decrby (int argc, char * argv []);
int rdsh_get (int argc, char * argv []);
int rdsh_getbit (int argc, char * argv []);
int rdsh_getrange (int argc, char * argv []);
int rdsh_getset (int argc, char * argv []);
int rdsh_incr (int argc, char * argv []);
int rdsh_incrby (int argc, char * argv []);
int rdsh_incrbyfloat (int argc, char * argv []);
int rdsh_mget (int argc, char * argv []);
int rdsh_mset (int argc, char * argv []);
int rdsh_msetnx (int argc, char * argv []);
int rdsh_psetex (int argc, char * argv []);
int rdsh_set (int argc, char * argv []);
int rdsh_setbit (int argc, char * argv []);
int rdsh_setex (int argc, char * argv []);
int rdsh_setrange (int argc, char * argv []);
int rdsh_setnx (int argc, char * argv []);
int rdsh_strlen (int argc, char * argv []);

/* Public functions in file hashes.c */
int rdsh_hdel (int argc, char * argv []);
int rdsh_hexists (int argc, char * argv []);
int rdsh_hget (int argc, char * argv []);
int rdsh_hgetall (int argc, char * argv []);
int rdsh_hincrby (int argc, char * argv []);
int rdsh_hincrbyfloat (int argc, char * argv []);
int rdsh_hkeys (int argc, char * argv []);
int rdsh_hlen (int argc, char * argv []);
int rdsh_hmget (int argc, char * argv []);
int rdsh_hmset (int argc, char * argv []);
int rdsh_hscan (int argc, char * argv []);
int rdsh_hset (int argc, char * argv []);
int rdsh_hsetnx (int argc, char * argv []);
int rdsh_hstrlen (int argc, char * argv []);
int rdsh_hvals (int argc, char * argv []);

/* Public functions in file lists.c */
int rdsh_blpop (int argc, char * argv []);
int rdsh_brpop (int argc, char * argv []);
int rdsh_brpoplpush (int argc, char * argv []);
int rdsh_lindex (int argc, char * argv []);
int rdsh_linsert (int argc, char * argv []);
int rdsh_llen (int argc, char * argv []);
int rdsh_lpop (int argc, char * argv []);
int rdsh_lpush (int argc, char * argv []);
int rdsh_lpushx (int argc, char * argv []);
int rdsh_lrange (int argc, char * argv []);
int rdsh_lrem (int argc, char * argv []);
int rdsh_lset (int argc, char * argv []);
int rdsh_ltrim (int argc, char * argv []);
int rdsh_rpop (int argc, char * argv []);
int rdsh_rpoplpush (int argc, char * argv []);
int rdsh_rpush (int argc, char * argv []);
int rdsh_rpushx (int argc, char * argv []);

/* Public functions in file sets.c */
int rdsh_sadd (int argc, char * argv []);
int rdsh_scard (int argc, char * argv []);
int rdsh_sdiff (int argc, char * argv []);
int rdsh_sdiffstore (int argc, char * argv []);
int rdsh_sinter (int argc, char * argv []);
int rdsh_sinterstore (int argc, char * argv []);
int rdsh_sismember (int argc, char * argv []);
int rdsh_smembers (int argc, char * argv []);
int rdsh_smove (int argc, char * argv []);
int rdsh_spop (int argc, char * argv []);
int rdsh_srandmember (int argc, char * argv []);
int rdsh_srem (int argc, char * argv []);
int rdsh_sscan (int argc, char * argv []);
int rdsh_sunion (int argc, char * argv []);
int rdsh_sunionstore (int argc, char * argv []);

/* Public functions in file sorted-sets.c */
int rdsh_bzpopmax (int argc, char * argv []);
int rdsh_bzpopmin (int argc, char * argv []);
int rdsh_zadd (int argc, char * argv []);
int rdsh_zcard (int argc, char * argv []);
int rdsh_zcount (int argc, char * argv []);
int rdsh_zincrby (int argc, char * argv []);
int rdsh_zinterstore (int argc, char * argv []);
int rdsh_zlexcount (int argc, char * argv []);
int rdsh_zpopmax (int argc, char * argv []);
int rdsh_zpopmin (int argc, char * argv []);
int rdsh_zrange (int argc, char * argv []);
int rdsh_zrangebylex (int argc, char * argv []);
int rdsh_zrangebyscore (int argc, char * argv []);
int rdsh_zrank (int argc, char * argv []);
int rdsh_zrem (int argc, char * argv []);
int rdsh_zremrangebylex (int argc, char * argv []);
int rdsh_zremrangebyrank (int argc, char * argv []);
int rdsh_zremrangebyscore (int argc, char * argv []);
int rdsh_zrevrange (int argc, char * argv []);
int rdsh_zrevrangebylex (int argc, char * argv []);
int rdsh_zrevrangebyscore (int argc, char * argv []);
int rdsh_zrevrank (int argc, char * argv []);
int rdsh_zscan (int argc, char * argv []);
int rdsh_zscore (int argc, char * argv []);
int rdsh_zunionstore (int argc, char * argv []);
