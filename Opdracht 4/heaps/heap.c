/*
    Lars Janssen 12882712
    Deze struct is een heap datastructuur,
    waar dingen bij kunnen worden gezet, uitgehaald kunnen worden
    en de grootte van de heap teruggegeven kan worden.
    Voor meer informatie over de heap datastructuur bekijk:
    https://en.wikipedia.org/wiki/Heap_(data_structure)
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

/*
    Dit initialiseert de heap en maakt een array aan.
    Het returned een pointer naar de heap als het lukt.
*/
static struct heap *heap_init(int (*compare)(const void *, const void *))
{
    struct heap *h = malloc(sizeof(struct heap));
    if (h)
    {
        h->array = array_init(0);

        if (!h->array)
        {
            free(h);
            return NULL;
        }

        h->compare = compare;
        return h;
    }

    return NULL;
}

/*
    Dit initialiseert een prioq door een heap te initialiseren.
*/
struct heap *prioq_init(int (*compare)(const void *, const void *))
{
    return heap_init(compare);
}

/*
    Dit returned de grootte van de prioq
    door de grootte van de array te returnen.
*/
long int prioq_size(struct heap *h)
{
    return array_size(h->array);
}

/*
    Dit ruimt de heap op.
*/
static int heap_cleanup(struct heap *h, void free_func(void *))
{
    if (h)
    {
        array_cleanup(h->array, free_func);
        free(h);
        return 0;
    }

    return 1;
}

/*
    Dit ruimt de prioq op door de heap op te ruimen.
*/
int prioq_cleanup(prioq *h, void free_func(void *))
{
    return (heap_cleanup(h, free_func));
}

/*
    Dit zet een element in de heap en sorteert de heap.
    Het element wordt achter in de array gezet en steeds geswapt
    als het kleiner is dan zijn paren volgens de compare functie.
*/
static int heap_insert(struct heap *h, void *p)
{

    if (h && h->array && p)
    {

        if (array_append(h->array, p) == 0)
        {
            long cI = array_size(h->array) - 1;
            long pI = (cI - (cI + 1) % 2 - 1) / 2;

            while (cI > 0 && h->compare(array_get(h->array, pI), p) > 0)
            {
                array_set(h->array, cI, array_get(h->array, pI));
                array_set(h->array, pI, p);
                cI = pI;
                pI = (cI - (cI + 1) % 2 - 1) / 2;
            }

            return 0;
        }
    }

    return 1;
}

/*
    Dit insert een element in de prioq.
*/
int prioq_insert(prioq *h, void *p)
{
    return heap_insert(h, p);
}

/*
    Dit sorteert de heap na het poppen van het kleinste element door
    steeds te te kijken of het te sorteren element groter is dan
    een of meerdere van zijn kinderen. Als dat zo is dan worden
    ze verwisseld. Dit gaat zo door totdat het element kleiner is dan
    beiden kinderen of er geen kinderen zijn.
*/
static void sort_heap(struct heap *h, int pI, int lCI, int rCI,
                      void *p, void *lC, void *rC)
{
    while (lC)
    {
        if (rC)
        {
            if (h->compare(lC, p) <= 0 && h->compare(lC, rC) <= 0)
            {
                array_set(h->array, pI, lC);
                array_set(h->array, lCI, p);
                pI = lCI;
            }
            else if (h->compare(rC, p) <= 0)
            {
                array_set(h->array, pI, rC);
                array_set(h->array, rCI, p);
                pI = rCI;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (h->compare(lC, p) <= 0)
            {
                array_set(h->array, pI, lC);
                array_set(h->array, lCI, p);
                pI = lCI;
            }
            else
            {
                break;
            }
        }

        lCI = 2 * pI + 1;
        rCI = 2 * pI + 2;
        p = array_get(h->array, pI);
        lC = array_get(h->array, lCI);
        rC = array_get(h->array, rCI);
    }
}

/*
    Dit popt het kleinste element uit de heap en sorteert de heap dan weer.
*/
static void *heap_pop(struct heap *h)
{
    if (h && array_size(h->array) > 0)
    {
        void *root = array_get(h->array, 0);

        /*
            Dit verwisselt het eerste en laatste element.
        */
        array_set(h->array, 0, array_get(h->array, array_size(h->array) - 1));
        array_set(h->array, array_size(h->array) - 1, root);

        /*
            Dit sorteert de heap als er een element gepopt wordt.
        */
        if (array_pop(h->array))
        {
            int pI = 0;
            int lCI = 2 * pI + 1;
            int rCI = 2 * pI + 2;
            void *p = array_get(h->array, 0);
            void *lC = array_get(h->array, lCI);
            void *rC = array_get(h->array, rCI);
            sort_heap(h, pI, lCI, rCI, p, lC, rC);
        }

        return root;
    }

    return NULL;
}

/*
    Dit popt een element uit de prioq door een element uit de heap te poppen.
*/
void *prioq_pop(prioq *h)
{
    return heap_pop(h);
}
