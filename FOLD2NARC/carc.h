#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct narc_dict
{

    short parent_id;
    char name[255];
    short id;
    char **files;
    int size_file;
    void *folders;
    int size_folder;
} narc_dict;

int write_temp_header(FILE *);

int write_fat(FILE *);

int write_fnt(narc_dict, FILE *, FILE *);
void transmit_file(FILE *, FILE *);
int write_img(FILE *, FILE *);
int convert_folders_to_carc(char *path, char *name_of_carc);

extern short id_file;
extern int *files_end;
extern int files_end_size;

extern int *files_start;
extern int files_start_size;
extern unsigned int *list_of_len;
extern int size_list_len;

extern int *first_id;
extern int first_id_size;