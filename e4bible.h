#ifndef E4BIBLE
#define E4BIBLE

#define MAX_BIBLE_FILE_SIZE 255

#include <sys/random.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <ctype.h>
#include "config.h"
#include "sqlite/sqlite3.h"
#include <unistd.h>
#include <assert.h>
#include "dbg.h"

//Variables for the verses
#define MAX_VERSE_LINE_LENGTH 40

enum E4BIBLE_RETURN_CODE { E4_OK, E4_FAILURE };

int cFileExists(const char * filename);
char * getRandomVerse();
#endif
