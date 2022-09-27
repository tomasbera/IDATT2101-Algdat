#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HashTable.h"

#define MAX_LINE_LENGTH (100)

static size_t hasher(const void *val, size_t val_size, size_t max)
{
    size_t key = 1;
    for (int i = 0; i < val_size; i++)
        key += ((int8_t*)val)[i] * (i + 1);

    return key % max;
}

void ht_insert_item_str(ht *ht, char *val, int *cols)
{
    *cols = ht_insert_item(ht, val, strlen(val) + 1, val, strlen(val) + 1, &hasher);
}

char *ht_find_item_str(ht *ht, char *key)
{
    return ht_find_item(ht, key, strlen(key) + 1, &hasher);
}

int main()
{
    FILE *textfile;
    char line[MAX_LINE_LENGTH], *res;
    int ch, lines = 1, collisions = 0, i = 0, l = 0;
    ht *ht = ht_malloc(113);

    textfile = fopen("names.txt", "r");
    if (textfile == NULL) 
    {
        fprintf(stderr, "The input file was not found.\n");
        return 1;
    }
    while ((ch=fgetc(textfile)) != EOF) 
    {
        if (ch == '\n') 
        {
            i = 0;
            ht_insert_item_str(ht, line, &l);
            if (l == 2) collisions++;
            lines++;
            memset(line, 0, MAX_LINE_LENGTH);
        }
        else
        {
            *(line + i) = ch;
            i++;
        }
    }
    ht_insert_item_str(ht, line, &l);
    if (l == 2) collisions++;
    memset(line, 0, MAX_LINE_LENGTH);
    fclose(textfile);

    textfile = fopen("names.txt", "r");
    if (textfile == NULL) 
    {
        fprintf(stderr, "The input file was not found.\n");
        return 1;
    }
    i = 0;
    while ((ch=fgetc(textfile)) != EOF) 
    {
        if (ch == '\n')
        {
            i = 0;
            res = ht_find_item_str(ht, line);
            if (strcmp(line, res) == 0) printf("%s = %s\n", line, res);
            memset(line, 0, MAX_LINE_LENGTH);
        }
        else
        {
            *(line + i) = ch;
            i++;
        }
    }
    res = ht_find_item_str(ht, line);
    if (strcmp(line, res) == 0) printf("%s = %s\n", line, res);
    memset(line, 0, MAX_LINE_LENGTH);
    fclose(textfile);

    printf("\n---------------------------------------------------------\n");
    printf("Total collisions: %d\n", collisions);
    printf("Load factor: %f\n", (float)(lines-collisions)/ht->max);
    printf("Collisions per person: %f\n", (float)collisions/lines);
    printf("%s\n", ht_find_item_str(ht, "Callum Gran"));
    ht_remove_item(ht, "Callum Gran", strlen("Callum Gran") + 1, hasher);
    res = ht_find_item_str(ht, "Callum Gran");
    if (res)
        printf("%s\n", res);
    else 
        printf("Deleted\n");
    
    ht_free(ht);
    
    return 0;
}