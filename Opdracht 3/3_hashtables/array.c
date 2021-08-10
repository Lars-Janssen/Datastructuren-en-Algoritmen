/*
    Lars Janssen 12882712
    Deze struct maakt een array, kan dingen in de array zetten
    en uitlezen en vergroot de array indien nodig.
*/

#include <stdlib.h>

#include "array.h"

/*
    De struct zelf houdt 3 variabelen bij:
    size, dit is het aantal plekken in de array waar data in staat,
    allocated, dit is het aantal geheugen dat gealloceerd is voor de array,
    p, dit is een pointer naar het eerste element van de array.

*/
struct array
{
    size_t size;
    size_t allocated;
    int *p;
};

/*
    Dit initialiseerd de array en geeft de array "initial_capacity"
    geheugen.
*/
struct array *array_init(unsigned long initial_capacity)
{
    struct array *a = malloc(sizeof(struct array));
    if (a)
    {
        a->size = 0;
        a->allocated = initial_capacity;
        a->p = malloc(a->allocated * sizeof(int));
        if (a->p)
        {
            return a;
        }
    }
    return NULL;
}

/*
    Dit ruimt de array op door het geheugen van de pointer en
    het geheugen van de struct vrij te geven.
*/
void array_cleanup(struct array *a)
{
    if (a)
    {
        free(a->p);
        free(a);
    }
}

/*
    Dit returned de int op plek "index" in de array.
    Als de gegeven "index" niet goed is returned hij -1.
*/
int array_get(struct array *a, unsigned long index)
{
    if (!a || index > a->size)
    {
        return -1;
    }
    return a->p[index];
}

/*
    Dit voegt een element toe aan de array en verhoogt de size met 1.
    Als het element niet meer in de array past, dan realloceerd hij
    de array waarbij er meer ruimte wordt vrijgemaakt, omdat
    allocated 2 keer groter wordt.
*/
int array_append(struct array *a, int elem)
{
    if (a->size == a->allocated)
    {
        size_t newalloc = a->allocated * 2;
        int *np = (int *)realloc(a->p, newalloc * sizeof(int));
        if (np)
        {
            a->p = np;
            a->allocated = newalloc;
        }
        else
        {
            return 1;
        }
    }
    a->p[a->size] = elem;
    a->size = a->size + 1;
    return 0;
}

/*
    Dit returned het aantal elementen in de array.
*/
unsigned long array_size(struct array *a)
{
    if (!a)
    {
        return 0;
    }
    return a->size;
}
