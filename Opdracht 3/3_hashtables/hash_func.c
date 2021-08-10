
/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(unsigned char *str)
{
    return (unsigned long)*str;
}

/*
    The next three functions come directly from the slides of Lecture 4.
*/
unsigned long hash_first_char(unsigned char *str)
{
    return (unsigned long)str[0];
}

unsigned long hash_add_chars(unsigned char *str)
{
    unsigned long h = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        h += (int)str[i];
    }

    return h;
}

unsigned long hash_java(unsigned char *str)
{
    unsigned long h = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        h = h * 31 + (int)str[i];
    }
    return h;
}

/*
    The next three hash functions come from this site:
    http://www.cse.yorku.ca/~oz/hash.html
*/

unsigned long hash_djb2(unsigned char *str)
{
    unsigned long hash = 5381;
    unsigned long c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

unsigned long hash_sdbm(unsigned char *str)
{
    unsigned long hash = 0;
    unsigned long c;

    while ((c = *str++))
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned long hash_lose_lose(unsigned char *str)
{
    unsigned long hash = 0;
    unsigned long c;

    while ((c = *str++))
    {
        hash += c;
    }

    return hash;
}