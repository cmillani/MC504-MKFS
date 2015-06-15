#ifndef BASH_OUTSIDE_COMMANDS
#define BASH_OUTSIDE_COMMANDS

#include <stdbool.h>
#include <stdio.h>
#include "bash.fs_parser.h"
#include "mkfs_error.h"
#include "bash.fs_bsmode.h"
#include "mkfs_struct.h"
#include <string.h>
#include "bash_apps/package.h"
#include "Stack/stack.h"
#include "bash.fs_reader.h"

void minus_o(FILE * output, char path_inside[], FILE * ufs);
void minus_i(FILE * input, char path_inside[], FILE * ufs);
void minus_d(FILE * ufs);
	
#endif