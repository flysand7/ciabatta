
#pragma once

#define O_ACCMODE       03
#define O_RDONLY        00
#define O_WRONLY        01
#define O_RDWR          02
#define O_CREAT       0100
#define O_EXCL        0200
#define O_NOCTTY      0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_NDELAY      O_NONBLOCK
#define O_SYNC        04010000
#define O_FSYNC       O_SYNC
#define O_ASYNC       020000
#define O_CLOEXEC   02000000
#define O_DIRECT      040000