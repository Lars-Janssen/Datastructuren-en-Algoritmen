/*
    Lars Janssen 12882712
    Dit programma leest een bestand uit.
    Het voegt een patient toe aan de priorityqueue als deze
    op de regel staat.
    Daarna kiest het de eerste patient, print die en verwijdert hem
    uit de priorityqueue. Na 10 iteraties wordt de prioritqueue
    leeggemaakt en worden de overgebleven namen geprint.
    Zie het pdf voor meer informatie.
*/

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prioq.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

struct config
{
    // You can ignore these options until/unless you implement the
    // bonus features.

    // Set to 1 if -y is specified, 0 otherwise.
    int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

/*
    Dit maakt een struct waar we de gegevens van een patient in opslaan.
*/
typedef struct
{
    char *name;
    int age;
} patient_t;

/*
    Dit is een manier om de patienten te vergelijken op basis van naam.
*/
static int compare_patient_name(const void *a, const void *b)
{
    return strcmp(((const patient_t *)a)->name, ((const patient_t *)b)->name);
}

/*
    Dit is een manier om de patienten te vergelijken op basis van leeftijd.
    Als de leeftijden gelijk zijn, dan worden de namen vergeleken.
*/
static int compare_patient_age(const void *a, const void *b)
{
    int difference = ((const patient_t *)a)->age - ((const patient_t *)b)->age;
    if (difference != 0)
    {
        return difference;
    }
    else
    {
        return compare_patient_name(a, b);
    }
}

/*
    Dit leest het bestand uit totdat er een punt staat.
    De nieuwe patienten worden opgeslagen in de heap.
*/
int read_out(prioq *queue)
{
    char *name;
    char *p;

    while (1)
    {
        char *s = fgets(buf, BUF_SIZE, stdin);

        if (strcmp(s, ".\n") == 0)
        {
            break;
        }
        else
        {
            patient_t *newPatient = malloc(sizeof(patient_t));
            newPatient->name = malloc(BUF_SIZE);

            if (!newPatient)
            {
                return EXIT_FAILURE;
            }

            if (!newPatient->name)
            {
                free(newPatient);
                return EXIT_FAILURE;
            }

            name = strtok(s, " ");
            newPatient->age = (int)strtol(strtok(NULL, " "), &p, 10);
            strncpy(newPatient->name, name, BUF_SIZE - 1);

            if (prioq_insert(queue, newPatient) != 0)
            {
                return EXIT_FAILURE;
            }
        }

        if (s == NULL)
        {
            fprintf(stderr, "Unexpected end of file. exiting\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/*
    Dit popt de patient die vooraan die priorityqueue staat en print de naam.
*/
void pop_patient(prioq *queue)
{
    patient_t *treatedPatient = prioq_pop(queue);
    printf("%s\n", treatedPatient->name);
    free(treatedPatient->name);
    free(treatedPatient);
}

/*
    Dit leest het bestand uit en beheert de priorityqueue.
*/
int main(int argc, char *argv[])
{
    prioq *queue;
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0)
    {
        return EXIT_FAILURE;
    }

    /*
        Als er -y is meegegeven wordt de compare functie verandert.
    */
    if (cfg.year)
    {
        queue = prioq_init(&compare_patient_age);
    }
    else
    {
        queue = prioq_init(&compare_patient_name);
    }

    if (!queue)
    {
        return EXIT_FAILURE;
    }

    for (int iterations = 0;;)
    {
        if (read_out(queue) == 1)
        {
            return EXIT_FAILURE;
        }

        if (prioq_size(queue) > 0)
        {
            pop_patient(queue);
        }

        printf(".\n"); // End turn.

        if (++iterations == 10)
        {
            while (prioq_size(queue) > 0)
            {
                pop_patient(queue);
            }

            break;
        }
    }

    prioq_cleanup(queue, free);
    return EXIT_SUCCESS;
}

int parse_options(struct config *cfg, int argc, char *argv[])
{
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "y")) != -1)
    {
        switch (c)
        {
        case 'y':
            cfg->year = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}
