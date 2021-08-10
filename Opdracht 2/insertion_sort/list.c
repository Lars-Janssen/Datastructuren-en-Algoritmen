/*
    Lars Janssen 12882712
    Dit bestand beschrijft 2 structs, een lijst en een node.
    Deze structs worden gebruikt om een singly linked list te kunnen maken.
    Voor meer informatie over singly linked lists:
    https://en.wikipedia.org/wiki/Linked_list#Singly_linked_list

    Verder worden er een aantal functies gemaakt waarmee de lijst bewerkt
    kan worden, zie list.h voor meer informatie hierover.
    Ik hoorde pas later dat je ook EXIT_SUCCES en EXIT_FAILURE kan gebruiken
    en in list.h staat dat je 0 en 1 moet gebruiken, dus ik heb dat gedaan.
*/
#include "list.h"


/*
    Dit beschrijft een node, die een value en een pointer naar
    een andere node heeft.
*/
struct node
{
    int value;
    struct node *next;
};


/*
    Dit beschrijft een list, die een pointer naar de eerste node
    van de lijst heeft en een lengte.
    De lengte is niet per se nodig voor een singly linked list,
    maar is voor het gemak.
*/
struct list
{
    size_t length;
    struct node *head;
};


/*
    Dit geeft ruimte vrij voor een list en geeft beginwaarden.
    Daarna returned het een pointer naar de list.
*/
struct list *list_init(void)
{
    struct list *l = malloc(sizeof(struct list));

    if (!l)
    {
        return NULL;
    }

    l->length = 0;
    l->head = NULL;
    return l;
}


/*
    Dit geeft ruimte vrij voor een node en geeft beginwaarden.
    Daarna returned het een pointer naar de node.
*/
struct node *list_new_node(int num)
{
    struct node *n = malloc(sizeof(struct node));

    if (!n)
    {
        return NULL;
    }

    n->value = num;
    n->next = NULL;
    return n;
}

/*
    Dit returned een pointer naar de eerste node in de lijst
*/
struct node *list_head(struct list *l)
{
    if (l->head == NULL || l->length == 0)
    {
        return NULL;
    }

    return l->head;
}

/*
    Dit returned een pointer naar de volgende node.
*/
struct node *list_next(struct node *n)
{
    if (n == NULL || n->next == NULL)
    {
        return NULL;
    }

    return n->next;
}

/*
    Dit voegt node n vooraan de lijst l toe.
*/
int list_add_front(struct list *l, struct node *n)
{
    if (l == NULL || n == NULL)
    {
        return 1;
    }

    if (l->head)
    {
        n->next = l->head;
    }

    l->head = n;
    l->length++;
    return 0;
}

/*
    Dit returned een pointer naar de laatste node in l.
*/
struct node *list_tail(struct list *l)
{
    if (l == NULL || l->length == 0)
    {
        return NULL;
    }

    struct node *currentNode = l->head;

    while (currentNode->next)
    {
        currentNode = currentNode->next;
    }

    return currentNode;
}

/*
    Dit returned een pointer naar de node voor node n in lijst l.
*/
struct node *list_prev(struct list *l, struct node *n)
{
    if (l == NULL || n == NULL || l->head == n)
    {
        return NULL;
    }

    struct node *currentNode = l->head;

    while (currentNode->next != n)
    {
        currentNode = currentNode->next;
    }

    return currentNode;
}

/*
    Dit voegt n toe aan het einde van l.
*/
int list_add_back(struct list *l, struct node *n)
{
    if (l == NULL || n == NULL)
    {
        return 1;
    }

    if (l->head == NULL)
    {
        l->head = n;
    }
    else
    {
        struct node *currentNode = l->head;

        while (currentNode->next)
        {
            currentNode = currentNode->next;
        }

        currentNode->next = n;
    }

    l->length++;
    return 0;
}

/*
    Dit returned de waarde van n.
*/
int list_node_value(struct node *n)
{
    if (n == NULL)
    {
        return 0;
    }

    return n->value;
}

/*
    Dit haalt n uit l en zorgt ervoor dat lijst l gewoon doorloopt.
*/
int list_unlink_node(struct list *l, struct node *n)
{
    if (l == NULL || n == NULL)
    {
        return 1;
    }

    if (l->head == n)
    {
        l->head = n->next;
        n->next = NULL;
    }
    else
    {
        struct node *previousNode = l->head;

        while (previousNode->next != n)
        {
            previousNode = previousNode->next;
        }

        struct node *nextNode = previousNode->next;
        nextNode = nextNode->next;
        previousNode->next = nextNode;
        n->next = NULL;
    }

    l->length--;
    return 0;
}

/*
    Dit geeft het geheugen voor een node vrij.
*/
void list_free_node(struct node *n)
{
    if (n != NULL)
    {
        free(n);
    }
}

/*
    Dit geeft het geheugen voor een lijst en alle nodes daarin vrij.
*/
int list_cleanup(struct list *l)
{
    if (l == NULL)
    {
        return 1;
    }

    struct node *currentNode = l->head;

    while (currentNode)
    {
        struct node *previousNode = currentNode;
        currentNode = currentNode->next;
        free(previousNode);
    }

    free(currentNode);
    free(l);
    return 0;
}

/*
    Dit kijkt of n in l zit.
*/
int list_node_present(struct list *l, struct node *n)
{
    if (l == NULL || n == NULL)
    {
        return 0;
    }

    if (l->length > 0)
    {
        struct node *currentNode = l->head;

        while (currentNode)
        {
            if (currentNode == n)
            {
                return 1;
            }

            currentNode = currentNode->next;
        }
    }

    return 0;
}

/*
    Dit voegt node n toe aan lijst l na node m.
*/
int list_insert_after(struct list *l, struct node *n, struct node *m)
{
    if (l == NULL || n == NULL || m == NULL)
    {
        return 1;
    }

    if (list_node_present(l, n) == 0 && list_node_present(l, m) == 1)
    {
        n->next = m->next;
        m->next = n;
        l->length++;
        return 0;
    }

    return 1;
}

/*
    Dit voegt node n toe aan lijst l voor node m.
*/
int list_insert_before(struct list *l, struct node *n, struct node *m)
{
    if (l == NULL || n == NULL || m == NULL)
    {
        return 1;
    }

    if (list_node_present(l, n) == 0 && list_node_present(l, m) == 1)
    {
        if (list_prev(l, m) != NULL)
        {
            struct node *previousNode = list_prev(l, m);
            previousNode->next = n;
            n->next = m;
            l->length++;
            return 0;
        }
    }

    return 1;
}

/*
    Dit returned het aantal nodes in l.
*/
size_t list_length(struct list *l)
{
    if (l == NULL)
    {
        return 0;
    }

    return l->length;
}

/*
    Dit returned een pointer naar de ide node in l door de lijst te doorlopen,
    totdat de juiste node gevonden is.
    De functie telt vanaf 0, dus de i = 0 geeft de eerste node.
*/
struct node *list_get_ith(struct list *l, size_t i)
{
    if (l == NULL || i >= l->length || l->length == 0)
    {
        return NULL;
    }

    struct node *currentNode = l->head;
    size_t counter = 0;

    while (counter < i)
    {
        currentNode = currentNode->next;
        counter++;
    }

    return currentNode;
}

/*
    Dit knipt de lijst in tweeen na de node n, waarbij het tweede
    deel een nieuwe lijst wordt. Er wordt pointer naar deze nieuwe lijst
    gereturned.
*/
struct list *list_cut_after(struct list *l, struct node *n)
{
    if (l == NULL || n == NULL || list_node_present(l, n) == 0)
    {
        return NULL;
    }

    struct node *currentNode = l->head;
    size_t length = 1;

    while (currentNode != n)
    {
        currentNode = currentNode->next;
        length++;
    }

    struct list *newList = list_init();

    newList->head = n->next;
    n->next = NULL;
    newList->length = l->length - length;
    l->length = length;
    return newList;
}
