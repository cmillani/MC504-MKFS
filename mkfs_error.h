#ifndef MKFS_ERROR
#define MKFS_ERROR

#define NO_BLOCK_SIZE_ERR 1
#define INVALID_ARGS_ERR 2
#define NO_NAME_ERR 3
#define MISSING_PAR_ERR 4
#define BLOCK_SZ_ERR 5

void parse_error(int error_type);

#endif