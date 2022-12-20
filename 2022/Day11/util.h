#ifndef DEBUG
    #define DEBUG 1
#endif

#if DEBUG
    #define FILENAME "test.txt"
#else
    #define FILENAME "input.txt"
#endif

#include "file.h"
#include <stdlib.h>
#include "coords.h"