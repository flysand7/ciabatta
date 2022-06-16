
#pragma once

#define EDOM   1
#define EILSEQ 2
#define ERANGE 3

// TODO: make it thread-local
extern _Thread_local int errno;
