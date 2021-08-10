/* Resizing array interface
 * Specialized for integers. */

/* Handle to array data structure. */
struct array;

/* Initialise an array and return a pointer to it.
 * Return NULL on failure. */
struct array *array_init(unsigned long initial_capacity);

/* Return the element at the index position in the array.
 * Return -1 if the index is not a valid position in the array. */
int array_get(struct array *a, unsigned long index);

/* Add the element at the end of the array.
 * Return 0 if succesful, 1 otherwise. */
int array_append(struct array *a, int elem);

/* Return the number of elements in the array */
unsigned long array_size(struct array *a);

/* Cleanup array data structure. */
void array_cleanup(struct array *a);
