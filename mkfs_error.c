#include "mkfs_error.h"
#include <stdio.h>
#include <stdlib.h>

char * errors_vector[] = {"Error: block size must be provided\n", "Error: invalid command\n", "Error: output file name must be provided\n", "Error: missing parameter for command\n", "Error: block size must be between 512B and 4KB\n"};

void parse_error(int error_type)
{
	fprintf(stderr, "%s", errors_vector[error_type-1]);
	exit(error_type);
}