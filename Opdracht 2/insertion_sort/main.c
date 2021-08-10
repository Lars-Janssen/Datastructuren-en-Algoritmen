/*
    Lars Janssen 12882712
    Dit krijgt een de stdinput als input en print een gesorteerde lijst in
    de stdoutput.
    De lijst wordt gesorteerd door middel van insertion sort. Voor meer info:
    https://en.wikipedia.org/wiki/Insertion_sort

    Verder zijn er nog een paar opties om de lijst anders te sorteren:
    -u, dan worden alleen unieke waardes gesorteerd en geprint.
    -d, dan wordt de lijst in aflopende volgorde gesorteerd en geprint.
    -i, dan worden er nadat de lijst van n getallen gesorteerd is n-1
    gemiddeldes tussen aanliggene getallen berekend en tussen deze getallen
    toegevoegd en geprint.
    -z, dan wordt de lijst in het midden gesplitst in twee lijsten
    en wordt er een nieuwe lijst gemaakt door om de beurt een element uit beide
    lijsten toe te voegen. Daarna wordt deze nieuwe lijst geprint.
*/

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

/*
    Dit bekijkt of er opties gekozen zijn.
*/
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

/*
    Dit sorteert de lijst door middel van insertion sort.
    cn staat voor currentNode.
*/
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

/*
    Dit sorteert de lijst door middel van insertion sort
    in aflopende volgorde.
    cn staat voor currentNode.
*/
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

/*
    Dit bekijkt of de waarde al in de lijst staat door de lijst
    te doorlopen en steeds te kijken of de waarde van de pointer gelijk
    is aan de nieuwe waarde.
    cn staat voor currentNode.
*/
int checkUnique(int newItem, struct list *l)
{
    if (list_head(l) != NULL)
    {
        struct node *cn = list_head(l);

        while (cn)
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

/*
    Dit berekent de gemiddeldes van twee aanliggende nodes en
    voegt dit gemiddele toe aan de lijst.
    cn staat voor currentNode.
    an staat voor averageNode.
*/
void intermediate(struct list *l)
{
    struct node *cn = list_head(l);

    while (list_next(cn))
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

/*
    Dit splitst de lijst in twee gelijke delen,
    waarbij het eerste deel langer is in geval va oneven lengte.
    Hierna wordt een nieuwe lijst gemaakt waarbij om en om een
    node uit beide lijsten toegevoegd.
    De eerste node uit de eerste lijst wordt dus eerst toegevoegd, dan
    de eerste node uit de tweede lijst, dan
    de tweede node uit de eerste lijst, etc.
    cn staat voor currentNode.
    cnl staat voor currentNodeL.
    cnnew staat voor currentNodeNewList.
    nnnew staat voor newNodeNewList.

*/
void zip(struct list *l)
{
    size_t length = 0;
    struct node *cn = list_head(l);

    while (list_next(cn) != NULL)
    {
        length++;
        cn = list_next(cn);
    }

    if (length % 2 == 0)
    {
        length = length / 2;
    }
    else
    {
        length = length / 2 + 1;
    }

    struct list *newList = list_cut_after(l, list_get_ith(l, length));
    struct node *cnl = list_head(l);
    struct node *cnnew = list_head(newList);

    while (cnnew != NULL)
    {
        struct node *nnnew = list_next(cnnew);
        cnl = list_next(cnl);
        list_insert_before(l, cnnew, cnl);
        cnnew = nnnew;
    }
}

/*
    Dit leest de input token voor token uit.
    Elke token wordt meteen op de goede plek in de lijst gezet.
*/
void read(struct list *l, struct config cfg)
{
    while (fgets(buf, BUF_SIZE, stdin))
    {
        char *token;
        token = strtok(buf, " ");

        while (token)
        {
            int newItem = (int)strtol(token, NULL, 10);

            if (cfg.unique_values == 1)
            {
                /*
                    Dit bekijkt of de waarde al in de lijst zit als
                    de optie -u gebruikt is.
                */
                if (checkUnique(newItem, l) == 1)
                {
                    token = strtok(NULL, " ");
                    continue;
                }
            }
            
            if (newItem != 0)
            {
                /*
                    Dit bekijkt of de optie -d gebruikt is en voert
                    dan de juiste sort uit.
                */
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
}

/*
    Dit krijgt de argumenten en print de uiteindelijke lijst.
    Verder bekijkt dit of de opties -i en/of -z gebruikt zijn.
*/
int main(int argc, char *argv[])
{
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0)
    {
        return 1;
    }

    struct list *l = list_init();
    read(l, cfg);

    if (cfg.insert_intermediate == 1 && list_head(l) != NULL)
    {
        intermediate(l);
    }
    if (cfg.zip_alternating == 1 && list_head(l) != NULL)
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
