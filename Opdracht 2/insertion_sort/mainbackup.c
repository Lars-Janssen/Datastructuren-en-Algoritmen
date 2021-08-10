#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include "list.h"
#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config
{
    // You can ignore these options until you implement the
    // extra command-line arguments.

    // Set to 1 if -u is specified, 0 otherwise.
    int unique_values;

    // Set to 1 if -d is specified, 0 otherwise.
    int descending_order;

    // Set to 1 if -i is specified, 0 otherwise.
    int insert_intermediate;

    // Set to 1 if -z is specified, 0 otherwise.
    int zip_alternating;
};

int parse_options(struct config *cfg, int argc, char *argv[])
{
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "udiz")) != -1)
    {
        switch (c)
        {
        case 'u':
            cfg->unique_values = 1;
            break;
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'i':
            cfg->insert_intermediate = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

void sort(int newItem, struct list *l)
{
    struct node *newNode = list_new_node(newItem);
    if (list_head(l) == NULL ||
        list_node_value(list_head(l)) >= list_node_value(newNode))
    {
        list_add_front(l, newNode);
    }
    else
    {
        struct node *cn = list_head(l);
        while (list_next(cn) != NULL &&
               list_node_value(list_next(cn)) < list_node_value(newNode))
        {
            cn = list_next(cn);
        }
        list_insert_after(l, newNode, cn);
    }
}

void descendingSort(int newItem, struct list *l)
{
    struct node *newNode = list_new_node(newItem);
    if (list_head(l) == NULL ||
        list_node_value(list_head(l)) <= list_node_value(newNode))
    {
        list_add_front(l, newNode);
    }
    else
    {
        struct node *cn = list_head(l);
        while (list_next(cn) != NULL &&
               list_node_value(list_next(cn)) > list_node_value(newNode))
        {
            cn = list_next(cn);
        }
        list_insert_after(l, newNode, cn);
    }
}

int checkUnique(int newItem, struct list *l)
{
    if (list_head(l) != NULL)
    {
        struct node *cn = list_head(l);
        while (list_next(cn) != NULL)
        {
            if (list_node_value(cn) == newItem)
            {
                return 1;
            }
            cn = list_next(cn);
        }
        if (list_node_value(cn) == newItem)
        {
            return 1;
        }
    }
    return 0;
}

void alternate(struct list *l)
{
    struct node *cn = list_head(l);
    while (list_next(cn) != NULL)
    {
        int average = (list_node_value(cn) +
                       list_node_value(list_next(cn))) /
                      2;
        struct node *an = list_new_node(average);
        list_insert_after(l, an, cn);
        cn = list_next(cn);
        cn = list_next(cn);
    }
}

void zip(struct list *l)
{
    if(l == NULL){
        return;
    }
    int length = 0;
    struct node *cn = list_head(l);
    while(list_next(cn) != NULL){
        length++;
        cn = list_next(cn);
    }
    if(length % 2 == 0){
        length = length / 2;
    }
    else{
        length = length / 2 + 1;
    }
    struct list *new = list_cut_after(l, list_get_ith(l, length));
    struct node *cnl = list_head(l);
    struct node *cnnew = list_head(new);
    while(cnnew != NULL){
        struct node *nnnew = list_next(cnnew);
        cnl = list_next(cnl);
        list_insert_before(l, cnnew, cnl);
        cnnew = nnnew;
    }
}

int main(int argc, char *argv[])
{
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0)
    {
        return 1;
    }

    struct list *l = list_init();

    while (fgets(buf, BUF_SIZE, stdin))
    {
        char *token;
        token = strtok(buf, " ");
        while (token)
        {
            int newItem = (int)strtol(token, NULL, 10);
            if (cfg.unique_values == 1)
            {
                if (checkUnique(newItem, l) == 1)
                {
                    token = strtok(NULL, " ");
                    continue;
                }
            }
            if (newItem != 0)
            {
                if (cfg.descending_order == 0)
                {
                    sort(newItem, l);
                }
                else if (cfg.descending_order == 1)
                {
                    descendingSort(newItem, l);
                }
            }
            token = strtok(NULL, " ");
        }
    }

    if (cfg.insert_intermediate == 1)
    {
        alternate(l);
    }
    if(cfg.zip_alternating == 1)
    {
        zip(l);
    }

    size_t counter = 0;
    while (list_get_ith(l, counter) != NULL)
    {
        printf("%d\n", list_node_value(list_get_ith(l, counter)));
        counter++;
    }

    list_cleanup(l);
    return 0;
}
