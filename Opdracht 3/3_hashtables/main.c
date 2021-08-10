/*
    Lars Janssen 12882712
    This makes a hash table with the words of a book as keys and the
    line numbers as values You can than search for a word and it returns
    the line numbers on which the word occurs.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

#define LINE_LENGTH 256

#define TABLE_START_SIZE 256
#define MAX_LOAD_FACTOR 0.6
#define HASH_FUNCTION hash_djb2

#define START_TESTS 2
#define MAX_TESTS 3
#define HASH_TESTS 3

/* Replace every non-ascii char with a space and lowercase every char. */
static void cleanup_string(char *line)
{
    for (char *c = line; *c != '\0'; c++)
    {
        *c = (char)tolower(*c);
        if (!isalpha(*c))
        {
            *c = ' ';
        }
    }
}

/* Return a pointer to a heap allocated string with all the characters we
 * use as word delimiters. Return NULL on failure. */
static char *calc_delim(void)
{
    const size_t ascii_table_size = 128;
    char *res = malloc(sizeof(char) * ascii_table_size);
    if (!res)
    {
        return NULL;
    }

    int res_index = 0;
    for (unsigned char c = 1; c < ascii_table_size; c++)
    { // exclude \0 char!
        if (!isalpha(c))
        { // if it's not an alpha it's a delim.
            res[res_index++] = (char)c;
        }
    }
    res[res_index] = '\0';
    return res;
}

/* Creates a hash table with a word index for the specified file and
 * parameters. Return a pointer to hash table or NULL if an error occured.
 * Cleans everyline by replacing non ascii characters with spaces and
 * lowercasing everything. It then splits the line up in words and adds
 * every word with its line_number to the hash_table.
 */
static struct table *create_from_file(char *filename,
                                      unsigned long start_size,
                                      double max_load,
                                      unsigned long (*hash_func)(unsigned char *))
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;

    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line)
    {
        fclose(fp);
        return NULL;
    }

    struct table *hash_table;
    hash_table = table_init(start_size, max_load, hash_func);

    if (!hash_table)
    {
        return NULL;
    }

    char *delim = calc_delim();
    int line_number = 1;

    while (fgets(line, LINE_LENGTH, fp))
    {
        cleanup_string(line);
        char *word = strtok(line, delim);

        while (word)
        {
            if (table_insert(hash_table, word, line_number) == 1)
            {
                return NULL;
            }

            word = strtok(NULL, delim);
        }

        line_number++;
    }

    free(delim);
    fclose(fp);
    free(line);

    return hash_table;
}

/* Reads words from stdin and prints line lookup results per word.
 * Return 0 if succesful and 1 on failure.
 * It gets the word to check with, looks up the array in the hash_table,
 * and gets and prints the line_numbers from the array.
 * */
static int stdin_lookup(struct table *hash_table)
{
    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line || !hash_table)
    {
        return 1;
    }

    char *delim = calc_delim();

    while (fgets(line, LINE_LENGTH, stdin))
    {
        char *check_word = strtok(line, delim);

        if (check_word)
        {
            cleanup_string(check_word);
            printf("%s\n", check_word);
            struct array *array = table_lookup(hash_table, check_word);
            if (array)
            {
                for (size_t i = 0; i < array_size(array); i++)
                {
                    if (array_get(array, i) != -1)
                    {
                        printf("* %d\n", array_get(array, i));
                    }
                }
            }
            else
            {
                free(line);
                free(delim);
                printf("\n");
                return 1;
            }
        }

        printf("\n");
    }

    free(line);
    free(delim);
    return 0;
}

static void timed_construction(char *filename)
{
    /* Here you can edit the hash table testing parameters: Starting size,
     * maximum load factor and hash function used, and see the the effect
     * on the time it takes to build the table.
     * You can edit the tested values in the 3 arrays below. If you change
     * the number of elements in the array, change the defined constants
     * at the top of the file too, to change the size of the arrays. */
    unsigned long start_sizes[START_TESTS] = {2, 65536};
    double max_loads[MAX_TESTS] = {0.2, 1.0, 2.0};
    unsigned long (*hash_funcs[HASH_TESTS])(unsigned char *) =
        {hash_java, hash_djb2, hash_lose_lose};

    for (int i = 0; i < START_TESTS; i++)
    {
        for (int j = 0; j < MAX_TESTS; j++)
        {
            for (int k = 0; k < HASH_TESTS; k++)
            {
                clock_t start = clock();
                struct table *hash_table =
                    create_from_file(filename, start_sizes[i], max_loads[j], hash_funcs[k]);
                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld "
                       "microsecs\n",
                       start_sizes[i], max_loads[j], k, end - start);
                table_cleanup(hash_table);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: %s text_file [-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && !strcmp(argv[2], "-t"))
    {
        timed_construction(argv[1]);
    }
    else
    {
        struct table *hash_table =
            create_from_file(argv[1], TABLE_START_SIZE, MAX_LOAD_FACTOR, HASH_FUNCTION);
        if (hash_table == NULL)
        {
            printf("An error occured creating the hash table, exiting..\n");
            return EXIT_FAILURE;
        }
        if (stdin_lookup(hash_table) != 0)
        {
            table_cleanup(hash_table);
            return EXIT_FAILURE;
        }
        table_cleanup(hash_table);
    }

    return EXIT_SUCCESS;
}
