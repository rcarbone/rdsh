![logo] rdsh, the Redis shell
=============================

```
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                       _     _
               _ __ __| |___| |__
              | '__/ _` / __| '_ \
              | | | (_| \__ \ | | |
              |_|  \__,_|___/_| |_|

             rdsh, the Redis shell

  R. Carbone (rocco@tecsiel.it)
    4Q 2018

  SPDX-License-Identifier: BSD-3-Clause

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
```

Table of contents:
 * **Hello world!**
 * **Screenshots**

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


* Hello world!
  ============

  Hello world, I am rdsh, the Redis shell.

  So, is rdsh a shell? No, no!
  rdsh is a Redis client. No, no again!

  Ok, let me explain.

  rdsh is a hack of the tcsh, mainly implemented to add Redis client
  functionalities into a shell.

  rdsh is several things at the same time: a shell, a Redis client,
  a Redis terminal-based program to create/read/administer Redis
  databases in character based terminals and finally a rendering
  engine for Redis data structures.

  So rdsh is surely a shell, because all of the existing native tcsh
  functionalities are left unchanged.

  But rdsh is also a Redis client because it has builtin extensions
  that allow you to manage one or more Redis server at the same time.

  All in a single program without leaving your native job enviroment!

  Have fun!

  /rocco


* Screenshots
  ===========

  #### A quick tour to rdsh available commands.

  ## Hello World!
  ![helloworld]

  ## Additional commands for Redis-Client
  ![help]

  ## Connecting/Disconnecting to/from a Redis Server ...
  ![connect]

  ## Establishing multiple connections to a Redis Server ...
  ![multiple]

  ## Populating a Hash with 1000 sequential keys-values ...
  ![hashing]


[logo]: images/rdsh.png
[helloworld]: images/helloworld.png
[help]: images/help.png
[connect]: images/connect.png
[multiple]: images/multiple.png
[hashing]: images/hashing.png
