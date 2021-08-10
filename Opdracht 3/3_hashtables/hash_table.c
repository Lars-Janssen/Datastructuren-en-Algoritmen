/*
    Lars Janssen 12882712
    Deze struct maakt een hashtable met linked lists.
    Vervolgens kunnen er een value en een key in de hashtable gezet worden
    en als de key al in de table zit, dan wordt de value toegevoegd aan
    de value array van de node met dezelfde key die al in de table staat.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"

struct table
{
    // The (simple) array used to index the table
    struct node **array;
    // The function used for computing the hash values in this table
    unsigned long (*hash_func)(unsigned char *);

    // Maximum load factor after which the table array should be resized
    double max_load;
    // Capacity of the array used to index the table
    unsigned long capacity;
    // Current number of elements stored in the table
    unsigned long load;
};

struct node
{
    // The string of characters that is the key for this node
    char *key;
    // A resizing array, containing the all the integer values for this key
    struct array *value;

    // Next pointer
    struct node *next;
};

size_t buf_size = 10;

/*
    Dit initialiseert de hashtable en maakt geeft geheugen vrij voor de
    array van nodes.
*/
struct table *table_init(unsigned long capacity,
                         double max_load,
                         unsigned long (*hash_func)(unsigned char *))
{

    struct table *t = malloc(sizeof(struct table));
    if (t)
    {
        t->max_load = max_load;
        t->capacity = capacity;
        t->hash_func = hash_func;
        t->array = calloc(t->capacity, sizeof(struct node));
        t->load = 0;
        return t;
    }
    return NULL;
}

/*
    Deze functie maakt een grotere array om de nodes in te doen.
    Daarna worden alle nodes gerehasht en daarin gezet.
    Als laatste wordt de pointer verandert om naar deze nieuwe array te wijzen
    en wordt de oude array gefreed.
*/
int rehash(struct table *t)
{
    struct node **newArray = calloc(t->capacity * 2, sizeof(struct node));

    if (!newArray)
    {
        return 1;
    }

    for (size_t i = 0; i < t->capacity; i++)
    {
        struct node *list = t->array[i];

        /*
            Dit gebruikt de hashfunctie op elke node om de nieuwe bucket
            te vinden. Daarna wordt de node in deze bucket geplaatst en wordt
            de node->next op NULL gezet, zodat hij niet meer naar de oude nodes
            wijst.
        */
        while (list)
        {
            struct node *listNext = list->next;
            unsigned long newBucket = t->hash_func((unsigned char *)list->key) % (t->capacity * 2);

            if (newArray[newBucket])
            {
                struct node *newList = newArray[newBucket];

                while (newList->next)
                {
                    newList = newList->next;
                }

                newList->next = list;
            }
            else
            {
                newArray[newBucket] = list;
            }

            list->next = NULL;
            list = listNext;
        }
    }

    free(t->array);
    t->array = newArray;
    t->capacity = t->capacity * 2;
    return 0;
}

/*
    Dit zet append de value array van de node met dezelfde key die al in de
    tabel staat.
*/
int keyInTable(struct table *t, char *key, int value, unsigned long bucket)
{
    struct node *list = t->array[bucket];

    while (strcmp(list->key, key) != 0)
    {
        list = list->next;
    }

    if (!list)
    {
        return 1;
    }

    array_append(list->value, value);
    return 0;
}

/*
    Dit zet de node in de bucket als er al iets in de bucket staat.
    Het doet het door naar het einde van de lijst te lopen en de node->next
    te veranderen.
*/
int bucketNotEmpty(struct table *t, char *key, int value, unsigned long bucket)
{
    struct node *list = t->array[bucket];

    while (list->next)
    {
        list = list->next;
    }

    struct node *newNode = malloc(sizeof(struct node));

    if (!newNode)
    {
        return 1;
    }

    newNode->key = key;

    if (!(newNode->value = array_init(1)))
    {
        return 1;
    }

    if (array_append(newNode->value, value) == 1)
    {
        return 1;
    }

    newNode->next = NULL;
    list->next = newNode;
    return 0;
}

/*
    Dit zet key en value in de array door een node te maken en deze
    in de juiste bucket te zetten
*/
int bucketEmpty(struct table *t, char *key, int value, unsigned long bucket)
{
    struct node *list = malloc(sizeof(struct node));

    if (!list)
    {
        return 1;
    }

    list->key = key;

    if (!(list->value = array_init(1)))
    {
        return 1;
    }

    if (array_append(list->value, value) == 1)
    {
        return 1;
    }

    list->next = NULL;
    t->array[bucket] = realloc(list, sizeof(struct node));

    if (!t->array[bucket])
    {
        return 1;
    }
    return 0;
}

/*
    Dit kopieert een key en insert deze en een value in de hashtable
    door een node te maken met de key en value erin,
    dan de key te hashen en de node in de juiste bucket te zetten.
    Als de key al in de hashtable zit, wordt de array
    van de value van de bijbehorende node geappend. Als de load_factor te
    groot wordt wordt de hashtable groter gemaakt en gerehashed.
*/
int table_insert(struct table *t, char *key, int value)
{
    if (!t)
    {
        return 1;
    }

    char *newKey = malloc(buf_size);

    if (!newKey)
    {
        return 1;
    }

    if (strlen(key) + 1 > buf_size)
    {
        buf_size = strlen(key) + 1;
        char *temp = realloc(newKey, buf_size);

        if (temp)
        {
            newKey = temp;
        }
        else
        {
            free(newKey);
            return 1;
        }
    }

    strncpy(newKey, key, buf_size);
    unsigned long bucket = t->hash_func((unsigned char *)newKey) % t->capacity;

    if (table_lookup(t, newKey) != NULL)
    {
        if (keyInTable(t, newKey, value, bucket) == 1)
        {
            return 1;
        }
        free(newKey);
    }
    else if (t->array[bucket])
    {
        if (bucketNotEmpty(t, newKey, value, bucket) == 1)
        {
            return 1;
        }
    }
    else
    {
        if (bucketEmpty(t, newKey, value, bucket) == 1)
        {
            return 1;
        }
    }

    t->load++;

    if (table_load_factor(t) > t->max_load)
    {
        if (rehash(t) == 1)
        {
            return 1;
        }
    }
    return 0;
}

/*
    Dit zoekt de node met de gegeven key op in de hashtable en als die er is,
    dan returned hij de value tabel.
*/
struct array *table_lookup(struct table *t, char *key)
{
    if (!t)
    {
        return NULL;
    }

    unsigned long bucket = t->hash_func((unsigned char *)key) % t->capacity;
    struct node *list = t->array[bucket];

    while (list)
    {
        if (strcmp(list->key, key) == 0)
        {
            return list->value;
        }

        list = list->next;
    }

    return NULL;
}

/*
    Dit returned de load van de hashtable gedeeld door de capacity van de
    hashtable. Als t nul is dan returned hij -1.
*/
double table_load_factor(struct table *t)
{
    if (t)
    {
        return (double)(t->load / t->capacity);
    }
    return -1;
}

/*
    Dit delete de node met de bijbehorende key uit de hashtable.
    Het doet dit door de bucket van de key te bekijken en vervolgens
    de lijst te doorlopen. Als de node gevonden wordt dan delete hij hem.
*/
int table_delete(struct table *t, char *key)
{
    if (!t || !table_lookup(t, key))
    {
        return 1;
    }
    else
    {
        unsigned long bucket = t->hash_func((unsigned char *)key) % t->capacity;
        struct node *list = t->array[bucket];
        struct node *previousNode = list;

        while (strcmp(list->key, key) != 0)
        {
            previousNode = list;
            list = list->next;
        }

        if (previousNode == list)
        {
            t->array[bucket] = list->next;
        }
        else
        {
            previousNode->next = list->next;
        }

        array_cleanup(list->value);
        free(list->key);
        free(list);
        return 0;
    }
}

/*
    Dit ruimt de hashtable op door elke lijst te doorlopen en de nodes
    en de value array te freeen.
*/
void table_cleanup(struct table *t)
{
    if (t)
    {
        for (size_t i = 0; i < t->capacity; i++)
        {
            struct node *list = t->array[i];
            if (list)
            {
                while (list)
                {
                    struct node *newNode = list->next;
                    free(list->key);
                    array_cleanup(list->value);
                    free(list);
                    list = newNode;
                }
            }
        }
        free(t->array);
        free(t);
    }
}
