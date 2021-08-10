/* Hashtable interface
 * Specialized for storing character arrays as the key and
 * arrays of integers as the value */

/* Handle to hash table data structure. */
struct table;

/* Handle to the hash table nodes */
struct node;

/* Initialise a hash table and return a pointer to it, returns NULL on failure.
 * Requires a starting capacity, a load factor after which to resize and a
 * function pointer to the hash function to be used for all hashing
 * operations. */
struct table *table_init(unsigned long capacity,
                         double max_load,
                         unsigned long (*hash_func)(unsigned char *));

/* Copies and inserts an array of characters as a key into the hash table,
 * together with the value, stored in a resizing integer array. If the key is
 * already present in the table, the value is appended to the existing array
 * instead. Returns 0 if successful and 1 otherwise. */
int table_insert(struct table *t, char *key, int value);

/* Returns the array of all inserted integer values for the specified key.
 * Returns NULL if the key is not present in the table. */
struct array *table_lookup(struct table *t, char *key);

/* Returns the load factor of the hash table. The load factor is defined
 * as: number of elements stored / size of hash table. */
double table_load_factor(struct table *t);

/* Remove the specified key and associated value from the hash table.
 * Returns 0 if the key was removed from the list.
 * Returns 1 if the key was not present in the hash table. */
int table_delete(struct table *t, char *key);

/* Clean up the hash table data structure. */
void table_cleanup(struct table *t);
