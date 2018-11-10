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


/* Constants */
#define HELPINDENT 10
#define INCREASED 0
#define COLS      140


/* The unsorted table with all the builtins */
static builtin_t commands [] =
{
  /* sessions */
  {
    CMD_HELP,
    "help",
    NULL,
    0,
    NULL,
    "Help [builtin] If builtin is specified, print out help on it, otherwise print out the list of extensions"
  },

  {
    CMD_SESSIONS,
    "sessions",
    NULL,
    0,
    NULL,
    "List all active sessions"
  },

  {
    CMD_CONNECT,
    "connect",
    NULL,
    0,
    NULL,
    "Connect to a Redis server"
  },

  {
    CMD_DISCONNECT,
    "disconnect",
    NULL,
    0,
    NULL,
    "Disconnect from a Redis server"
  },

  {
    CMD_CHANGE,
    "change",
    NULL,
    0,
    NULL,
    "Focus on a given session"
  },

  /* Connections (currently implemented #6 of #6 defined) */
  {
    CMD_AUTH,
    "rdauth",
    "auth",
    1,
    "AUTH password",
    "Authenticate to the server"
  },

  {
    CMD_ECHO,
    "rdecho",
    "echo",
    1,
    "ECHO message",
    "Echo the given string"
  },

  {
    CMD_PING,
    "rdping",
    "ping",
    0,
    "PING [message]",
    "Ping the server"
  },

  {
    CMD_QUIT,
    "rdquit",
    "quit",
    0,
    "QUIT",
    "Close the connection"
  },

  {
    CMD_SELECT,
    "rdselect",
    "select",
    1,
    "SELECT index",
    "Change the selected database for the current connection"
  },

  {
    CMD_SWAPDB,
    "rdswapdb",
    "swapdb",
    2,
    "SWAPDB index index",
    "Swap two Redis databases"
  },

  /* Data Structures (currently implemented #24 of #24 defined) */
  {
    CMD_DEL,
    "rddel",
    "del",
    1,
    "DEL name",
    "Delete a named data structure"
  },

  {
    CMD_DUMP,
    "rddump",
    "dump",
    1,
    "DUMP name",
    "Return a serialized version of the value stored at the specified name"
  },

  {
    CMD_EXISTS,
    "rdexists",
    "exists",
    1,
    "EXISTS name [name ...]",
    "Determine if a named data structure exists"
  },

  {
    CMD_EXPIRE,
    "rdexpire",
    "expire",
    2,
    "EXPIRE name seconds",
    "Set a name's time to live in seconds"
  },

  {
    CMD_EXPIREAT,
    "rdexpireat",
    "expireat",
    2,
    "EXPIREAT name timestamp",
    "Set the expiration for a name as a UNIX timestamp"
  },

  {
    CMD_KEYS,
    "rdkeys",
    "keys",
    1,
    "KEYS pattern",
    "Find all data structures matching the given pattern"
  },

  {
    CMD_MIGRATE,
    "rdmigrate",
    "migrate",
    3,
    "MIGRATE host port name",
    "Atomically transfer a name from a Redis instance to another one"
  },

  {
    CMD_MOVE,
    "rdmove",
    "move",
    3,
    "MOVE name db",
    "Move a name to another database"
  },

  {
    CMD_OBJECT,
    "rdobject",
    "object",
    1,
    "OBJECT cmd [arguments ...]",
    "Inspect the internals of Redis objects"
  },

  {
    CMD_PERSIST,
    "rdpersist",
    "persist",
    1,
    "PERSIST name",
    "Remove the expiration from a name"
  },

  {
    CMD_PEXPIRE,
    "rdpexpire",
    "pexpire",
    2,
    "PEXPIRE name msecs",
    "Set a name's time to live in milliseconds"
  },

  {
    CMD_PEXPIREAT,
    "rdpexpireat",
    "pexpireat",
    2,
    "PEXPIREAT name msecs-timestamp",
    "Set a name's time to live in milliseconds"
  },

  {
    CMD_PTTL,
    "rdpttl",
    "pttl",
    1,
    "PTTL name",
    "Get the time to live for a name in milliseconds"
  },

  {
    CMD_RANDOMKEY,
    "rdrandomkey",
    "randomkey",
    0,
    "RANDOMKEY",
    "Return a random data structure from the namespace"
  },

  {
    CMD_RENAME,
    "rdrename",
    "rename",
    2,
    "RENAME name newname",
    "Rename a name"
  },

  {
    CMD_RENAMENX,
    "rdrenamenx",
    "renamenx",
    2,
    "RENAMENX name newname",
    "Rename a name, only if the new name does not exist"
  },

  {
    CMD_RESTORE,
    "rdrestore",
    "restore",
    3,
    "RESTORE name ttl serialized-value [REPLACE]",
    "Create a name using the provided serialized value, previously obtained using DUMP"
  },

  {
    CMD_SCAN,
    "rdscan",
    "scan",
    1,
    "SCAN cursor [MATCH pattern] [COUNT count]",
    "Incrementally iterate the named space"
  },

  {
    CMD_SORT,
    "rdsort",
    "sort",
    1,
    "SORT name [BY pattern] [LIMIT offset count]",
    "Sort the elements in a list, set or sorted set"
  },

  {
    CMD_TOUCH,
    "rdtouch",
    "touch",
    1,
    "TOUCH name [name ...]",
    "Alters the last access time of name(s)"
  },

  {
    CMD_TTL,
    "rdttl",
    "ttl",
    1,
    "TTL name",
    "Get the time to live for a data structure"
  },

  {
    CMD_TYPE,
    "rdtype",
    "type",
    1,
    "TYPE name",
    "Determine the type stored at data structure"
  },

  {
    CMD_UNLINK,
    "rdunlink",
    "unlink",
    1,
    "UNLINK name [name ...]",
    "Delete a name asynchronously in another thread"
  },

  {
    CMD_WAIT,
    "rdwait",
    "wait",
    2,
    "WAIT numreplicas timeout",
    "Wait for the synchronous replication of all the write commands sent in the context of the current connection"
  },

  /* Strings (currently implemented #24 of #24 defined) */
  {
    CMD_APPEND,
    "rdappend",
    "append",
    2,
    "APPEND key value",
    "Append a value to a key"
  },

  {
    CMD_BITCOUNT,
    "rdbitcount",
    "bitcount",
    1,
    "BITCOUNT key [start end]",
    "Count set bits in a string"
  },

  {
    CMD_BITFIELD,
    "rdbitfield",
    "BITFIELD",
    1,
    "BITFIELD key [GET type offset] [SET type offset value] [INCRBY type offset increment] [OVERFLOW WRAP|SAT|FAIL]",
    "Perform arbitrary bitfield integer operations on strings"
  },

  {
    CMD_BITOP,
    "rdbitop",
    "bitop",
    3,
    "BITOP operation destkey key [key ...]",
    "Perform bitwise operations between strings"
  },

  {
    CMD_BITPOS,
    "rdbitpos",
    "bitpos",
    2,
    "BITPOS key bit [start] [end]",
    "Find first bit set or clear in a string"
  },

  {
    CMD_DECR,
    "rddecr",
    "decr",
    1,
    "DECR key",
    "Decrement the integer value of a key by one"
  },

  {
    CMD_DECRBY,
    "rddecrby",
    "decrby",
    2,
    "DECRBY key decrement",
    "Decrement the integer value of a key by the given number"
  },

  {
    CMD_GET,
    "rdget",
    "get",
    1,
    "GET key",
    "Get the value of a key"
  },

  {
    CMD_GETBIT,
    "rdgetbit",
    "getbit",
    2,
    "GETBIT key offset",
    "Returns the bit value at offset in the string value stored at key"
  },

  {
    CMD_GETRANGE,
    "rdgetrange",
    "getrange",
    3,
    "GETRANGE key start end",
    "Get a substring of the string stored at a key"
  },

  {
    CMD_GETSET,
    "rdgetset",
    "getset",
    2,
    "GETSET key value",
    "Set the string value of a key and return its old value"
  },

  {
    CMD_INCR,
    "rdincr",
    "incr",
    1,
    "INCR key",
    "Increment the integer value of a key by one"
  },

  {
    CMD_INCRBY,
    "rdincrby",
    "incrby",
    2,
    "INCRBY key increment",
    "Increment the integer value of a key by the given amount"
  },

  {
    CMD_INCRBYFLOAT,
    "rdincrbyfloat",
    "incrbyfloat",
    2,
    "INCRBYFLOAT key increment",
    "Increment the float value of a key by the given amount"
  },

  {
    CMD_MGET,
    "rdmget",
    "mget",
    1,
    "MGET key [key ...]",
    "Get the values of all the given keys"
  },

  {
    CMD_MSET,
    "rdmset",
    "mset",
    1,
    "MSET key value [key value ...]",
    "Set multiple keys to multiple values"
  },

  {
    CMD_MSETNX,
    "rdmsetnx",
    "msetnx",
    2,
    "MSETNX key value [key value ...]",
    "Set multiple keys to multiple values, only if none of the keys exist"
  },

  {
    CMD_PSETEX,
    "rdpsetex",
    "PSETEX",
    3,
    "PSETEX key milliseconds value",
    "Set the value and expiration in milliseconds of a key"
  },

  {
    CMD_SET,
    "rdset",
    "set",
    1,
    "SET key value [expiration EX seconds|PX milliseconds] [NX|XX]",
    "Set the string value of a key"
  },

  {
    CMD_SETBIT,
    "rdsetbit",
    "setbit",
    3,
    "SETBIT key offset value",
    "Sets or clears the bit at offset in the string value stored at key"
  },

  {
    CMD_SETEX,
    "rdsetex",
    "setex",
    3,
    "SETEX key seconds value",
    "Set the value and expiration of a key"
  },

  {
    CMD_SETNX,
    "rdsetnx",
    "setnx",
    2,
    "SETNX key value",
    "Set the value of a key, only if the key does not exist"
  },

  {
    CMD_SETRANGE,
    "rdsetrange",
    "setrange",
    3,
    "SETRANGE key offset value",
    "Overwrite part of a string at key starting at the specified offset"
  },

  {
    CMD_STRLEN,
    "rdstrlen",
    "strlen",
    1,
    "STRLEN key",
    "Get the length of the value stored in a key"
  },

  /* Hashes (currently implemented #15 of #15 defined) */
  {
    CMD_HDEL,
    "rdhdel",
    "hdel",
    2,
    "HDEL name key [key ...]",
    "Delete one or more hash keys"
  },

  {
    CMD_HEXISTS,
    "rdhexists",
    "hexists",
    2,
    "HEXISTS name key",
    "Determine if a hash key exists"
  },

  {
    CMD_HGET,
    "rdhget",
    "hget",
    2,
    "HGET name key",
    "Get the value of a hash key"
  },

  {
    CMD_HGETALL,
    "rdhgetll",
    "hgetall",
    1,
    "HGETALL name",
    "Get all the keys and values in a hash"
  },

  {
    CMD_HINCRBY,
    "rdhincrby",
    "hincrby",
    3,
    "HINCRBY name key increment",
    "Increment the integer value of a hash key by the given number"
  },

  {
    CMD_HINCRBYFLOAT,
    "rdhincrbyfloat",
    "hincrbyfloat",
    3,
    "HINCRBYFLOAT name key increment",
    "Increment the float value of a hash key by the given amount"
  },

  {
    CMD_HKEYS,
    "rdhkeys",
    "hkeys",
    1,
    "HKEYS name",
    "Get all the keys in a hash"
  },

  {
    CMD_HLEN,
    "rdhlen",
    "hlen",
    1,
    "HLEN name",
    "Get the number of keys in a hash"
  },

  {
    CMD_HMGET,
    "rdhmget",
    "hmget",
    2,
    "HMGET name key [key ...]",
    "Get the values of all the given hash keys"
  },

  {
    CMD_HMSET,
    "rdhmset",
    "hmset",
    3,
    "HMSET name key value [key value ...]",
    "Set multiple hash keys to multiple values"
  },

  {
    CMD_HSCAN,
    "rdhscan",
    "hscan",
    2,
    "HSCAN name cursor [MATCH pattern] [COUNT count]",
    "Incrementally iterate hash keys and associated values"
  },

  {
    CMD_HSET,
    "rdhset",
    "hset",
    3,
    "HSET name key value",
    "Set a value of a hash key"
  },

  {
    CMD_HSETNX,
    "rdhsetnx",
    "hsetnx",
    3,
    "HSETNX name key value",
    "Set the value of a hash key, only if the key does not exist"
  },

  {
    CMD_HSTRLEN,
    "rdhstrlen",
    "hstrlen",
    2,
    "HSTRLEN name key",
    "Get the length of the value of a hash key"
  },

  {
    CMD_HVALS,
    "rdhvals",
    "hvals",
    1,
    "HVALS name",
    "Get all the values in a hash"
  },

  /* Lists (currently implemented #17 of #17 defined) */
  {
    CMD_BLPOP,
    "rdblpop",
    "blpop",
    2,
    "BLPOP name [key ...] timeout",
    "Remove and get the first element in a list, or block until one is available"
  },

  {
    CMD_BRPOP,
    "rdbrpop",
    "brpop",
    2,
    "BRPOP name [key ...] timeout",
    "Remove and get the last element in a list, or block until one is available"
  },

  {
    CMD_BRPOPLPUSH,
    "rdbrpoplpush",
    "brpoplpush",
    3,
    "BRPOPLPUSH source destination timeout",
    "Pop a value from a list, push it to another list and return it; or block until one is available"
  },

  {
    CMD_LINDEX,
    "rdlindex",
    "lindex",
    2,
    "LINDEX name index",
    "Get an element from a list by its index"
  },

  {
    CMD_LINSERT,
    "rdlinsert",
    "linsert",
    4,
    "LINSERT name BEFORE|AFTER pivot value",
    "Insert an element before or after another element in a list"
  },

  {
    CMD_LLEN,
    "rdllen",
    "llen",
    1,
    "LLEN name",
    "Get the length of a list"
  },

  {
    CMD_LPOP,
    "rdlpop",
    "lpop",
    1,
    "LPOP name",
    "Remove and get the first element in a list"
  },

  {
    CMD_LPUSH,
    "rdlpush",
    "lpush",
    2,
    "LPUSH name value [value ...]",
    "Prepend one or multiple values to a list"
  },

  {
    CMD_LPUSHX,
    "rdlpushx",
    "lpushx",
    2,
    "LPUSHX name value",
    "Prepend a value to a list, only if the list exists"
  },

  {
    CMD_LRANGE,
    "rdlrange",
    "lrange",
    3,
    "LRANGE name start stop",
    "Get a range of elements from a list"
  },

  {
    CMD_LREM,
    "rdlrem",
    "lrem",
    3,
    "LREM name count value",
    "Remove elements from a list"
  },

  {
    CMD_LSET,
    "rdlset",
    "lset",
    3,
    "LSET name index value",
    "Set the value of an element in a list by its index"
  },

  {
    CMD_LTRIM,
    "rdltrim",
    "ltrim",
    3,
    "LTRIM name start stop",
    "Trim a list to the specified range"
  },

  {
    CMD_RPOP,
    "rdrpop",
    "rpop",
    2,
    "RPOP name",
    "Remove and get the last element in a list"
  },

  {
    CMD_RPOPLPUSH,
    "rdrpoplpush",
    "rpoplpush",
    2,
    "RPOPLPUSH source destination",
    "Remove the last element in a list, prepend it to another list and return it"
  },

  {
    CMD_RPUSH,
    "rdrpush",
    "rpush",
    3,
    "RPUSH name value [value ...]",
    "Append one or multiple values to a list"
  },

  {
    CMD_RPUSHX,
    "rdrpushx",
    "rpushx",
    2,
    "RPUSHX name value",
    "Append a value to a list, only if the list exists"
  },

  /* Sets (currently implemented #15 of #15 defined) */
  {
    CMD_SADD,
    "rdsadd",
    "sadd",
    2,
    "SADD name member [member ...]",
    "Add one or more members to a set"
  },

  {
    CMD_SCARD,
    "rdscard",
    "scard",
    1,
    "SCARD name",
    "Get the number of members in a set"
  },

  {
    CMD_SDIFF,
    "rdsdiff",
    "sdiff",
    1,
    "SDIFF name [name ...]",         "Subtract multiple sets" },

  {
    CMD_SDIFFSTORE,
    "rdsdiffstore",
    "sdiffstore",
    2,
    "SDIFFSTORE dst name [name ...]",
    "Subtract multiple sets and store the resulting set in a name"
  },

  {
    CMD_SINTER,
    "rdsinter",
    "sinter",
    1,
    "SINTER name [name ...]",
    "Intersect multiple sets"
  },

  {
    CMD_SINTERSTORE,
    "rdsinterstore",
    "sinterstore",
    2,
    "SINTERSTORE dst name [name ...]",
    "Intersect multiple sets and store the resulting set in a name"
  },

  {
    CMD_SISMEMBER,
    "rdsismember",
    "sismember",
    2,
    "SISMEMBER name member",
    "Determine if a given value is a member of a set"
   },

  {
    CMD_SMEMBERS,
    "rdsmembers",
    "smembers",
    1,
    "SMEMBERS name",
    "Get all the members in a set"
  },

  {
    CMD_SMOVE,
    "rdsmove",
    "smove",
    3,
    "SMOVE src dst member",
    "Move a member from one set to another"
  },

  {
    CMD_SPOP,
    "rdspop",
    "spop",
    1,
    "SPOP name [count]",
    "Remove and return one or multiple random members from a set"
  },

  {
    CMD_SRANDMEMBER,
    "rdsrandmember",
    "srandmember",
    1,
    "SRANDMEMBER name [count]",
    "Get one or multiple random members from a set"
  },

  {
    CMD_SREM,
    "rdsrem",
    "srem",
    2,
    "SREM name member [member ...]",
    "Remove one or more members from a set"
  },

  {
    CMD_SSCAN,
    "rdsscan",
    "sscan",
    2,
    "SSCAN name cursor [MATCH pattern] [COUNT count]",
    "Incrementally iterate Set elements"
  },

  {
    CMD_SUNION,
    "rdsunion",
    "sunion",
    1,
    "SUNION name [name ...]",
    "Add multiple sets"
  },

  {
    CMD_SUNIONSTORE,
    "rdsunionstore",
    "sunionstore",
    2,
    "SUNIONSTORE dst name [name ...]",
    "Add multiple sets and store the resulting set in a name"
  },

  /* Sorted Sets (currently implemented #25 of #25 defined) */
  {
    CMD_BZPOPMAX,
    "rdbzpopmax",
    "bzpopmax",
    2,
    "BZPOPMAX name [name ...] timeout",
    "Remove and return the member with the highest score from one or more sorted sets, or block until one is available"
  },

  {
    CMD_BZPOPMIN,
    "rdbzpopmin",
    "bzpopmin",
    2,
    "BZPOPMIN name [name ...] timeout",
    "Remove and return the member with the lowest score from one or more sorted sets, or block until one is available"
  },

  {
    CMD_ZADD,
    "rdzadd",
    "zadd",
    3,
    "ZADD key [NX|XX] [CH] [INCR] score member [score member ...]",
    "Add one or more members to a sorted set, or update its score if it already exists"
  },

  {
    CMD_ZCARD,
    "rdzcard",
    "zcard",
    1,
    "ZCARD name",
    "Get the number of members in a sorted set"
  },

  {
    CMD_ZCOUNT,
    "rdzcount",
    "zcount",
    3,
    "ZCOUNT name min max",
    "Count the members in a sorted set with scores within the given values"
  },

  {
    CMD_ZINCRBY,
    "rdzincrby",
    "zincrby",
    3,
    "ZINCRBY name increment member",
    "Increment the score of a member in a sorted set"
  },

  {
    CMD_ZINTERSTORE,
    "rdzinterstore",
    "zinterstore",
    3,
    "ZINTERSTORE destination numkeys name [name ...] [WEIGHTS weight [weight ...]] [AGGREGATE SUM|MIN|MAX]",
    "Intersect multiple sorted sets and store the resulting sorted set in a new name"
  },

  {
    CMD_ZLEXCOUNT,
    "rdzlexcount",
    "zlexcount",
    3,
    "ZLEXCOUNT name min max",
    "Count the number of members in a sorted set between a given lexicographical range"
  },

  {
    CMD_ZPOPMAX,
    "rdzpopmax",
    "zpopmax",
    1,
    "ZPOPMAX name [count]",
    "Remove and return members with the highest scores in a sorted set"
  },

  {
    CMD_ZPOPMIN,
    "rdzpopmin",
    "zpopmin",
    1,
    "ZPOPMAX name [count]",
    "Remove and return members with the lowest scores in a sorted set"
  },

  {
    CMD_ZRANGE,
    "rdzrange",
    "zrange",
    3,
    "ZRANGE name start stop [WITHSCORES]",
    "Return a range of members in a sorted set, by index"
  },

  {
    CMD_ZRANGEBYLEX,
    "rdzrangebylex",
    "zrangebylex",
    3,
    "ZRANGEBYLEX name min max [LIMIT offset count]",
    "Return a range of members in a sorted set, by lexicographical range"
  },

  {
    CMD_ZRANGEBYSCORE,
    "rdzrangebyscore",
    "zrangebyscore",
    3,
    "ZRANGEBYSCORE name min max [WITHSCORES] [LIMIT offset count]",
    "Return a range of members in a sorted set, by score"
  },

  {
    CMD_ZRANK,
    "rdzrank",
    "zrank",
    2,
    "ZRANK name member",
    "Determine the index of a member in a sorted set"
  },

  {
    CMD_ZREM,
    "rdzrem",
    "zrem",
    2,
    "ZREM name member [member ...]",
    "Remove one or more members from a sorted set"
  },

  {
    CMD_ZREMRANGEBYLEX,
    "rdzremrangebylex",
    "zremrangebylex",
    3,
    "ZREMRANGEBYLEX name min max",
    "Remove all members in a sorted set between the given lexicographical range"
  },

  {
    CMD_ZREMRANGEBYRANK,
    "rdzremrangebyrank",
    "zremrangebyrank",
    3,
    "ZREMRANGEBYRANK name start stop",
    "Remove all members in a sorted set within the given indexes"
  },

  {
    CMD_ZREMRANGEBYSCORE,
    "rdzremrangebyscore",
    "zremrangebyscore",
    3,
    "ZREMRANGEBYSCORE name min max",
    "Remove all members in a sorted set within the given scores"
  },

  {
    CMD_ZREVRANGE,
    "rdzrevrange",
    "zrevrange",
    3,
    "ZREVRANGE name start stop [WITHSCORES]",
    "Return a range of members in a sorted set, by index, with scores ordered from high to low"
  },

  {
    CMD_ZREVRANGEBYLEX,
    "rdzrevrangebylex",
    "zrevrangebylex",
    3,
    "ZREVRANGEBYLEX name max min [LIMIT offset count]",
    "Return a range of members in a sorted set, by lexicographical range, ordered from higher to lower strings"
  },

  {
    CMD_ZREVRANGEBYSCORE,
    "rdzrevrangebyscore",
    "zrevrangebyscore",
    3,
    "ZREVRANGEBYSCORE name max min [WITHSCORES] [LIMIT offset count]",
    "Return a range of members in a sorted set, by score, with scores ordered from high to low"
  },

  {
    CMD_ZREVRANK,
    "rdzrevrank",
    "zrevrank",
    2,
    "ZREVRANK name member",
    "Determine the index of a member in a sorted set, with scores ordered from high to low"
  },

  {
    CMD_ZSCAN,
    "rdzscan",
    "zscan",
    2,
    "ZSCAN name cursor [MATCH pattern] [COUNT count]",
    "Incrementally iterate sorted sets elements and associated scores"
  },

  {
    CMD_ZSCORE,
    "rdzscore",
    "zscore",
    2,
    "ZSCORE name member", "Get the score associated with the given member in a sorted set" },

  {
    CMD_ZUNIONSTORE,
    "rdzunionstore",
    "zunionstore",
    3,
    "ZUNIONSTORE destination numkeys name [name ...] [WEIGHTS weight [weight ...]] [AGGREGATE SUM|MIN|MAX]",
    "Add multiple sorted sets and store the resulting sorted set in a new name"
  },

  { 0, NULL, NULL, 0, NULL, NULL }
};
#define CMDSIZE sizeof (commands) / sizeof (commands [0])


/* Functions to implement the builtins */

static unsigned longestcmd (void)
{
  unsigned longest = 0;
  builtin_t * cmd;

  for (cmd = commands; cmd < commands + CMDSIZE; cmd ++)
    {
      unsigned len = cmd -> name ? strlen (cmd -> name) : 0;
      if (longest < len)
	longest = len;
    }
  return longest;
}


/*
 * width is a multiple of 8
 * and it is increased of [INCREASED] to show builtins
 */
/* The [help list] builtin */
static void list_help_builtins (void)
{
  unsigned max   = longestcmd () + INCREASED;
  unsigned width = (max + INCREASED + 8) &~ 7;
  unsigned cols  = COLS / width;
  builtin_t * builtin;
  unsigned rows;
  unsigned i;
  unsigned j;

  if (cols == 0)
    cols = 1;
  rows = (CMDSIZE + cols - 1) / cols;

  printf ("The following builtins were implemented, as extensions to the native [tcsh] builtins:\n\n");

  for (i = 0; i < rows; i ++)
    {
      for (j = 0; j < cols; j ++)
	{
	  builtin = commands + i * cols + j;

	  if (builtin -> name && i * cols + j < CMDSIZE)
	    printf (" %-*.*s ", max, max, builtin -> name);
	  else
	    break;
	}
      printf ("\n");
    }
}


/*
 * Look up NAME as the name of a builtin, and return a pointer to that builtin.
 * Return a NULL pointer if NAME is not a builtin name.
 */
static builtin_t * lookup (char * name)
{
  char * p;
  char * q;
  builtin_t * builtin;
  builtin_t * found;

  int nmatches = 0;
  int longest = 0;

  found = NULL;
  for (builtin = commands; (p = builtin -> name); builtin ++)
    {
      for (q = name; * q == * p ++; q ++)
	if (* q == 0)		/* exact match ? */
	  return builtin;
      if (! * q)
	{			/* the name was a prefix */
	  if ((q - name) > longest)
	    {
	      longest = q - name;
	      nmatches = 1;
	      found = builtin;
	    }
	  else if (q - name == longest)
	    nmatches ++;
	}
    }

  switch (nmatches)
    {
    case 0:
      return NULL;
    case 1:
      return (builtin_t *) -1;
    default:
      return found;
    }
}


/* Lookup builtin by id */
builtin_t * builtin_by_id (unsigned id)
{
  builtin_t * builtin;
  for (builtin = commands; builtin -> name; builtin ++)
    if (builtin -> id == id)
      return builtin;
  return NULL;
}


/* Lookup builtin by name */
builtin_t * builtin (char * name)
{
  builtin_t * builtin;
  for (builtin = commands; builtin -> name; builtin ++)
    if (! strcmp (builtin -> name, name))
      return builtin;
  return NULL;
}


/* Builtin extension */
int rdsh_help (int argc, char * argv [])
{
  builtin_t * builtin;

  if (argc == 1)
    list_help_builtins ();
  else
    {
      while (-- argc)
	{
	  builtin = lookup (argv [argc]);
	  if (builtin == (builtin_t *) -1)
	    {
	      printf ("%s: ambiguous help\n", argv [argc]);
	      continue;
	    }
	  else if (! builtin)
	    {
	      printf ("%s: builtin not found.\n", argv [argc]);
	      continue;
	    }
	  else
	    printf ("%-*s\t%s\n", HELPINDENT, builtin -> name, builtin -> help);
	}
    }
  return 0;
}
