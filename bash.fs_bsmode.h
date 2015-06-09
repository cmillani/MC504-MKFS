#ifndef UFS_BASH_MODE
#define UFS_BASH_MODE
//TODO implementar reader do block
void enable_bash_mode(char fs_name[]);

void ls(FILE * file, char path[], char extra[]);
void chmod (FILE * file, int mode, char name[], char path[]);
void mkdir (FILE * file, char name[], char path[]);
void chdir (FILE * file, char name[]);
void rm (FILE * file, char name[], char path[]);
void echo (FILE * file, char data[], char target[], char path[]);
void cat (FILE * file, char target[], char path[]);
void exit_bs (void);

#endif