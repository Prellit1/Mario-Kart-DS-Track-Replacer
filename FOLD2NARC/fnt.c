#include "carc.h"

unsigned int list_of_len[Reasonable_max_len];
int size_list_len = 0;

int write_fnt(narc_dict sorted, FILE *f, FILE *Write)
{
    // char *fnt_subt = malloc(0);
    int size_subt = 0;
    make_all_subtable(sorted, Write, &size_subt);

    FILE *fnt_entry = fopen("entries.bin", "wb+");
    unsigned int size_entries = write_fnt_base(sorted, fnt_entry);

    // unsigned int size = list_of_len[size_list_len - 1] + 8;
    // unsigned int tempS = size + size % 4;
    fseek(fnt_entry, 0, SEEK_END);
    fseek(Write, 0, SEEK_END);
    unsigned int size = ftell(fnt_entry) + ftell(Write) + 8;
    unsigned int tempS = size + size % 4;
    fseek(fnt_entry, 0, SEEK_SET);
    fseek(Write, 0, SEEK_SET);

    fwrite("BTNF", 4, 1, f);
    fwrite(&tempS, 4, 1, f);
    // fwrite(fnt_entry, size_entries, 1, f);
    transmit_file(f, fnt_entry);
    // fwrite(fnt_subt, size_subt, 1, f);
    transmit_file(f, Write);
    // tempS = -1; //

    fclose(fnt_entry);
    return size + size % 4;
}

void make_all_subtable(narc_dict sorted, FILE *sub_str, int *size)
{
    unsigned int tempS = -1; //
    make_subtable_string(sorted, sub_str, size);
    recurs_subt(sorted, sub_str, size);
    fseek(sub_str, 0, SEEK_END);
    for (int misalign = 0; misalign < ftell(sub_str) % 4; misalign++)
    {
        fwrite(&tempS, 1, 1, sub_str);
    }
}

void recurs_subt(narc_dict sorted, FILE *sub_str, int *size)
{
    for (int foldID = 0; foldID < sorted.size_folder; foldID++)
    {
        make_subtable_string(((narc_dict *)sorted.folders)[foldID], sub_str, size);
        recurs_subt(((narc_dict *)sorted.folders)[foldID], sub_str, size);
    }
}

void make_subtable_string(narc_dict sorted, FILE *sub_str, int *size)
{
    unsigned char tmp;
    for (int foldID = 0; foldID < sorted.size_folder; foldID++)
    {
        tmp = strlen(((narc_dict *)sorted.folders)[foldID].name);
        tmp |= 0x80;
        // int prec_size = *size;

        *size += tmp + 3;

        // sub_str = realloc(sub_str, *size + 1);
        fwrite(&tmp, 1, 1, sub_str);
        // memcpy(sub_str + prec_size, &tmp, 1);
        fwrite(((narc_dict *)sorted.folders)[foldID].name, tmp & ~0x80, 1, sub_str);
        // memcpy(sub_str + prec_size + 1, ((narc_dict *)sorted.folders)[foldID].name, tmp & ~0x80);
        fwrite(&((narc_dict *)sorted.folders)[foldID].id, 2, 1, sub_str);
        // memcpy(sub_str + *size - 2, &((narc_dict *)sorted.folders)[foldID].id, 2);
    }
    for (int fileID = 0; fileID < sorted.size_file; fileID++)
    {
        tmp = strlen(sorted.files[fileID]);
        // int prec_size = *size;

        *size += tmp + 1;
        // sub_str = realloc(sub_str, *size + 1);
        // memcpy(sub_str + prec_size, &tmp, 1);
        fwrite(&tmp, 1, 1, sub_str);
        fwrite(sorted.files[fileID], tmp, 1, sub_str);

        // memcpy(sub_str + prec_size + 1, sorted.files[fileID], tmp);
    }
    // sub_str[*size] = '\0';
    fwrite("\0", 1, 1, sub_str);

    size_list_len++;
    // list_of_len = realloc(list_of_len, size_list_len * sizeof(int));
    if (size_list_len > Reasonable_max_len)
        return;
    list_of_len[size_list_len - 1] = ftell(sub_str);
    // tmp + list_of_len[size_list_len - 2];
    // printf("a");
}
void write_fnt_entry(narc_dict sorted, FILE *write, int num_entry)
{
    int int_offs = list_of_len[(unsigned short)(sorted.id & ~0xF000)];
    short int_id = (short)first_id[(unsigned short)(sorted.id & ~0xF000)];
    short id_parent_ordir;
    if (!sorted.parent_id)
        id_parent_ordir = num_entry;
    else
        id_parent_ordir = sorted.parent_id;

    fwrite(&int_offs, 4, 1, write);
    fwrite(&int_id, 2, 1, write);
    fwrite(&id_parent_ordir, 2, 1, write);
    /* strncpy(base_str + *iterated, &int_offs, 4);
    *iterated += 4;
    strncpy(base_str + *iterated, &int_id, 2);
    *iterated += 2;
    strncpy(base_str + *iterated, &id_parent_ordir, 2);
    *iterated += 2; */
    for (int foldID = 0; foldID < sorted.size_folder; foldID++)
    {
        write_fnt_entry(((narc_dict *)sorted.folders)[foldID], write, 0);
    }
}
int write_fnt_base(narc_dict sorted, FILE *write)
{
    int size_sub_entry = 8;
    int num_entry = 1 + find_all_subdirs(sorted);
    size_sub_entry *= num_entry;
    for (int id = 0; id < size_list_len; id++)
    {
        list_of_len[id] += size_sub_entry;
    }
    int iterated = 0;

    // printf("a\n");
    write_fnt_entry(sorted, write, num_entry);
    return size_sub_entry;
}

int find_all_subdirs(narc_dict sorted)
{
    int num = 0;
    for (int foldID = 0; foldID < sorted.size_folder; foldID++)
    {
        num++;
        num += find_all_subdirs(((narc_dict *)sorted.folders)[foldID]);
    }
    return num;
}