
/* Example hash function with terrible performance */
unsigned long hash_too_simple(unsigned char *str);

/* Hash functions from the slides */
unsigned long hash_first_char(unsigned char *str);

unsigned long hash_add_chars(unsigned char *str);

unsigned long hash_java(unsigned char *str);

/*
    Hash functions from this website:
    http://www.cse.yorku.ca/~oz/hash.html
*/
unsigned long hash_djb2(unsigned char *str);

unsigned long hash_sdbm(unsigned char *str);

unsigned long hash_lose_lose(unsigned char *str);

/* Add the header for your own added hash functions here. You may search online
 * for existing solutions for hashing function, as long as you as you
 * \emph{attribute the source}, meaning links to the used material.  */
