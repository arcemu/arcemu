/*
 * Generic hashmap manipulation functions
 */
#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

/*
 * any_t is a pointer.  This allows you to put arbitrary structures in
 * the hashmap.
 */
typedef void *any_t;

/*
 * PFany is a pointer to a function that takes int key and any_t data arguments
 * and return an integer. Returns status code..
 */
typedef int (*PFany)(int, any_t);

/*
 * map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented.  They see and manipulate only map_t's.
 */
typedef any_t map_t;

/*
 * Return an empty hashmap. Returns NULL if empty.
*/
extern map_t hashmap_new();

/*
 * Add an element to the hashmap. Return MAP_OK or MAP_OMEM.
 */
extern int hashmap_put(map_t in, int key, any_t value);

/*
 * Get an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int hashmap_get(map_t in, int key, any_t *arg);

/*
 * Remove an element from the hashmap. Return MAP_OK or MAP_MISSING.
 */
extern int hashmap_remove(map_t in, int key);

/*
 * Get element at position index from the hashmap
 */
extern int hashmap_get_index(map_t in, int index, int *key, any_t *arg);

/*
 * Free the hashmap
 */
extern void hashmap_free(map_t in);

/*
 * Get the current size of a hashmap
 */
extern int hashmap_length(map_t in);

/*
 * 64bit functions
 */
extern map_t hashmap64_new();
extern int hashmap64_put(map_t in, int64 key, any_t value);
extern int hashmap64_get(map_t in, int64 key, any_t *arg);
extern int hashmap64_remove(map_t in, int64 key);
extern int hashmap64_get_index(map_t in, int index, int64 *key, any_t *arg);
extern void hashmap64_free(map_t in);
extern int hashmap64_length(map_t in);

#endif /* __HASHMAP_H__ */
