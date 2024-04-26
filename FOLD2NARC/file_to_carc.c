#include "carc.h"
#include <windows.h>

short id = 0xF000;

short id_file = 0;

// int *first_id = NULL;
int first_id[Reasonable_max_len];
int first_id_size = 0;

// int *files_end = NULL;
int files_end[Reasonable_max_len];
int files_end_size = 0;

// int *files_start = NULL;
int files_start[Reasonable_max_len];
int files_start_size = 0;

narc_dict init_narcdict()
{
    narc_dict dic;

    dic.files = malloc(0);
    dic.folders = malloc(0);
    dic.size_file = 0;
    dic.size_folder = 0;
    return dic;
}

HANDLE get_dir_winHandle(char *path, WIN32_FIND_DATA *B)
{

    HANDLE hF = FindFirstFile((LPCSTR)path, B); //.

    FindNextFile(hF, B); //..

    // FindNextFile(hF, B); //(1st file)
    return hF;
}
narc_dict sort_dir(char *dir, char *path, int parent_id)
{
    WIN32_FIND_DATA B;

    narc_dict sorted_result = init_narcdict();
    sorted_result.parent_id = parent_id;
    strcpy(sorted_result.name, dir);
    sorted_result.id = id;

    id++;
    char *fullPath = malloc(strlen(dir) + strlen(path) + 3);
    char *fullerPath = malloc(strlen(dir) + strlen(path) + 5);
    strcpy(fullPath, path);
    strcat(fullPath, "\\");
    strcat(fullPath, dir);
    strcpy(fullerPath, fullPath);
    strcat(fullerPath, "\\*");

    // printf("%s\n", fullPath);

    HANDLE hF = get_dir_winHandle(fullerPath, &B);

    while (FindNextFile(hF, &B) != 0)
    {
        // printf("a\n");
        char *tempStr = calloc(strlen(B.cFileName) + 1, 1);

        strcpy(tempStr, B.cFileName);

        if (B.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            sorted_result.size_folder++;
            sorted_result.folders = realloc(sorted_result.folders, sorted_result.size_folder * sizeof(narc_dict));
            ((narc_dict *)sorted_result.folders)[sorted_result.size_folder - 1] = sort_dir(tempStr, fullPath, sorted_result.id);
        }
        else
        {
            sorted_result.size_file++;
            sorted_result.files = realloc(sorted_result.files, sorted_result.size_file * sizeof(char *));

            sorted_result.files[sorted_result.size_file - 1] = tempStr;
            // printf("%s   %s    %s\n--\n", tempStr, B.cFileName, sorted_result.files[sorted_result.size_file - 1]);
        }
        // free(tempStr);
    }

    free(fullPath);
    free(fullerPath);
    return sorted_result;
}

int index_file(narc_dict sorted)
{
    first_id_size++;
    // first_id = first_id ? realloc(first_id, first_id_size * sizeof(int)) : malloc(first_id_size * sizeof(int));
    if (/* !first_id */ first_id_size > Reasonable_max_len)
        return 0;
    first_id[first_id_size - 1] = id_file;

    for (int fileID = 0; fileID < sorted.size_file; fileID++)
    {
        id_file++;
    }
    for (int foldID = 0; foldID < sorted.size_folder; foldID++)
    {

        if (!index_file(((narc_dict *)sorted.folders)[foldID]))
            return 0;
    }
    return 1;
}
void freeing_narcs(narc_dict dict)
{
    for (int fileID = 0; fileID < dict.size_file; fileID++)
    {
        free(dict.files[fileID]);
    }
    free(dict.files);
    for (int foldID = 0; foldID < dict.size_folder; foldID++)
    {
        freeing_narcs(((narc_dict *)dict.folders)[foldID]);
    }
    free(dict.folders);
}

int load_all_files(narc_dict sorted, char *base_path, unsigned int *prec_size, FILE *WRITE)
{

    int size = *prec_size;
    char *tmpSTR = malloc(3 + strlen(base_path) + strlen(sorted.name));

    if (!tmpSTR)
    {
        printf("%d   %d 2\n", GetLastError(), 3 + strlen(base_path) + strlen(sorted.name));
        return 0;
    }

    strcpy(tmpSTR, base_path);
    strcat(tmpSTR, "\\");
    strcat(tmpSTR, sorted.name);
    // printf("%s\n", tmpSTR);
    for (int fileID = 0; fileID < sorted.size_file; fileID++)
    {
        char ff = 0xff;
        char *tmpSTR2 = malloc(3 + strlen(tmpSTR) + strlen(sorted.files[fileID]));

        if (!tmpSTR2)
        {
            printf("%d    %d\n", GetLastError(), 3 + strlen(tmpSTR) + strlen(sorted.files[fileID]));
            free(tmpSTR);
            return 0;
        }

        strcpy(tmpSTR2, tmpSTR);
        strcat(tmpSTR2, "\\");
        strcat(tmpSTR2, sorted.files[fileID]);

        // printf("%s\n", tmpSTR2);

        FILE *file = fopen(tmpSTR2, "rb"); //----
        // printf("a\n");

        fseek(file, 0, SEEK_END);
        unsigned int alloc = ftell(file);
        // printf("a\n");

        size += alloc;

        files_end_size++;
        // iles_end = !files_end ? malloc(files_end_size * sizeof(int) + 1) : realloc(files_end, files_end_size * sizeof(int) + 1);
        /* if (!files_end)
        {
            printf("%d    %d nd\n", GetLastError(), files_end_size * sizeof(int) + 1);
            free(tmpSTR);
            free(tmpSTR2);
            return 0;
        } */

        if (files_end_size > Reasonable_max_len)
            return 0;
        files_end[files_end_size - 1] = size;
        // printf("a\n");

        size += alloc % 4;

        files_start_size++;
        /* files_start = realloc(files_start, files_start_size * sizeof(int) + 1);
        if (!files_start)
        {
            printf("%d    %d st\n", GetLastError(), files_start_size * sizeof(int) + 1);
            free(tmpSTR);
            free(tmpSTR2);
            return 0;
        } */
        if (files_start_size > Reasonable_max_len)
            return 0;
        files_start[files_start_size - 1] = size;
        // printf("a\n");

        char *fileAlloc = malloc(alloc);
        if (!fileAlloc)
        {
            printf("%d         %d al\n", GetLastError(), alloc);
            free(tmpSTR);
            free(tmpSTR2);
            return 0;
        }
        // FILES = realloc(FILES, size);

        fseek(file, 0, SEEK_SET);
        fread(fileAlloc, alloc, 1, file);
        // printf("%x   %x    %x    %x  %x  %p \n", size, size, alloc, *prec_size, FILES + size, fileAlloc);
        // memcpy(FILES + *prec_size, fileAlloc, alloc);
        fwrite(fileAlloc, alloc, 1, WRITE);

        // printf("a\n");
        *prec_size = size;
        // printf("aaa\n");
        for (int four_alligner = 0; four_alligner < alloc % 4; four_alligner++)
        {
            // memcpy(FILES + size + four_alligner, &ff, 1);
            fwrite(&ff, 1, 1, WRITE);
        }
        // printf("aaa\n");
        fclose(file); //-------------------------

        // printf("aaa\n");
        free(tmpSTR2);
        // printf("aaa\n");
        free(fileAlloc);
        // printf("aaa\n");
    }
    for (int foldID = 0; foldID < sorted.size_folder; foldID++)
    {
        // printf("%s\n", tmpSTR);
        if (!load_all_files(((narc_dict *)sorted.folders)[foldID], tmpSTR, prec_size, WRITE))
        {
            free(tmpSTR);
            return 0;
        }
    }
    free(tmpSTR);
    return 1;
}
void transmit_file(FILE *dst, FILE *src)
{
    int buff;
    fseek(src, 0, SEEK_END);
    int size = ftell(src);
    fseek(src, 0, SEEK_SET);
    for (int it = 0; it < size / sizeof(int); it++)
    {
        fread(&buff, sizeof(int), 1, src);
        fwrite(&buff, sizeof(int), 1, dst);
    }
}

void split_strs(char *whole_path, char *result)
{
    char *temp = strrchr(whole_path, '\\') + 1;
    strcpy(result, temp);
    whole_path[(temp - 1) - whole_path] = '\0';
    // printf("%s   %s\n", result, whole_path);
}

int convert_folders_to_carc(char *path, char *name_of_carc)
{
    // REINIT
    size_list_len = 0;
    files_start_size = 0;
    id = 0xF000;
    id_file = 0;
    first_id_size = 0;
    files_end_size = 0;
    // END REINIT

    char last[255];

    split_strs(path, last);
    // printf("%s\n", path);

    narc_dict sorted = sort_dir(last, path, 0);

    // printf("a\n");
    if (!index_file(sorted))
        return 0;
    // printf("aa\n");

    unsigned int size = 0;
    FILE *file = fopen(name_of_carc, "wb"); //----

    size += write_temp_header(file);
    // printf("a\n");

    files_start_size++;
    // files_start = malloc(files_start_size * sizeof(int) + 1);
    files_start[files_start_size - 1] = 0;
    // char *FILES = malloc(0);

    // printf("aa\n");
    unsigned int prec_size = 0;
    FILE *WritableIMG = fopen("blank.bin", "wb+");
    if (!load_all_files(sorted, path, &prec_size, WritableIMG))
        return 0;

    size_list_len++;
    // list_of_len = malloc(size_list_len * sizeof(int));
    list_of_len[size_list_len - 1] = 0;

    // printf("a\n");

    size += write_fat(file);
    FILE *WritableFNT = fopen("blankFNT.bin", "wb+");
    size += write_fnt(sorted, file, WritableFNT);
    size += write_img(file, WritableIMG);

    fseek(file, 8, SEEK_SET);
    fwrite(&size, 4, 1, file);

    fclose(file);        //-------------------------
    fclose(WritableIMG); //--
    fclose(WritableFNT); //--
    remove("blankFNT.bin");
    remove("blank.bin");
    remove("entries.bin");
    freeing_narcs(sorted);
    if (list_of_len)
        free(list_of_len);

    return 1;
}
/* int main(int a, char *argcv[])
{
    char *path = malloc(strlen(argcv[1]) + 1);
    strcpy(path, argcv[1]);
    if (convert_folders_to_carc(path, "b.narc"))
        return 1;
    return 0;
} */
