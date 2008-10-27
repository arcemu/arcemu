/*
 * Generic map implementation.
 * free() must be invoked when only one thread has access to the hashmap.
 */
#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
#include "hashmap.h"

#define INITIAL_SIZE 1024

// We need to keep keys and values
typedef struct _hashmap_element{
	int key;
	int in_use;
	any_t data;
} hashmap_element;

// A hashmap has some maximum size and current size,
// as well as the data to hold.
typedef struct _hashmap_map{
	int table_size;
	int size;
	hashmap_element *data;
} hashmap_map;

/*
 * Return an empty hashmap, or NULL on failure.
 */
map_t hashmap_new() {
	hashmap_map* m = (hashmap_map*) malloc(sizeof(hashmap_map));
	if(!m) goto err;

	m->data = (hashmap_element*) calloc(INITIAL_SIZE, sizeof(hashmap_element));
	if(!m->data) goto err;

	m->table_size = INITIAL_SIZE;
	m->size = 0;

	return m;
	err:
		if (m)
			hashmap_free(m);
		return NULL;
}

/*
 * Hashing function for an integer
 */
unsigned int hashmap_hash_int(hashmap_map * m, unsigned int key){
	/* Robert Jenkins’ 32 bit Mix Function */
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

	/* Knuth’s Multiplicative Method */
	key = (key >> 3) * 2654435761;

	return key % m->table_size;
}

/*
 * Return the integer of the location in data
 * to store the point to the item, or MAP_FULL.
 */
int hashmap_hash(map_t in, int key){
	int curr;
	int i;

	/* Cast the hashmap */
	hashmap_map* m = (hashmap_map *) in;

	/* If full, return immediately */
	if(m->size == m->table_size) return MAP_FULL;

	/* Find the best index */
	curr = hashmap_hash_int(m, key);

	/* Linear probling */
	for(i = 0; i< m->table_size; i++){
		if(m->data[curr].in_use == 0)
			return curr;

		if(m->data[curr].key == key && m->data[curr].in_use == 1)
			return curr;

		curr = (curr + 1) % m->table_size;
	}

	return MAP_FULL;
}

/*
 * Doubles the size of the hashmap, and rehashes all the elements
 */
int hashmap_rehash(map_t in){
	int i;
	int old_size;
	hashmap_element* curr;

	/* Setup the new elements */
	hashmap_map *m = (hashmap_map *) in;
	hashmap_element* temp = (hashmap_element *)
		calloc(2 * m->table_size, sizeof(hashmap_element));
	if(!temp) return MAP_OMEM;

	/* Update the array */
	curr = m->data;
	m->data = temp;

	/* Update the size */
	old_size = m->table_size;
	m->table_size = 2 * m->table_size;
	m->size = 0;

	/* Rehash the elements */
	for(i = 0; i < old_size; i++){
		int status = hashmap_put(m, curr[i].key, curr[i].data);
		if (status != MAP_OK)
			return status;
	}

	free(curr);

	return MAP_OK;
}

/*
 * Add a pointer to the hashmap with some key
 */
int hashmap_put(map_t in, int key, any_t value){
	int index;
	hashmap_map* m;

	/* Cast the hashmap */
	m = (hashmap_map *) in;

	/* Find a place to put our value */
	index = hashmap_hash(in, key);
	while(index == MAP_FULL){
		if (hashmap_rehash(in) == MAP_OMEM) {
			return MAP_OMEM;
		}
		index = hashmap_hash(in, key);
	}

	/* Set the data */
	m->data[index].data = value;
	m->data[index].key = key;
	m->data[index].in_use = 1;
	m->size++; 

	return MAP_OK;
}

/*
 * Get your pointer out of the hashmap with a key
 */
int hashmap_get(map_t in, int key, any_t *arg){
	int curr;
	int i;
	hashmap_map* m;

	/* Cast the hashmap */
	m = (hashmap_map *) in;

	/* Find data location */
	curr = hashmap_hash_int(m, key);

	/* Linear probing, if necessary */
	for(i = 0; i< m->table_size; i++){

		if(m->data[curr].key == key && m->data[curr].in_use == 1){
			if (arg) {
				*arg = (int *) (m->data[curr].data);
			}
			return MAP_OK;
		}

		curr = (curr + 1) % m->table_size;
	}

	if (arg) {
		*arg = NULL;
	}

	/* Not found */
	return MAP_MISSING;
}

/*
 * Get element at position index from the hashmap
 */
int hashmap_get_index(map_t in, int index, int *key, any_t *arg) {
	hashmap_map* m;
	int i,j;

	/* Cast the hashmap */
	m = (hashmap_map *) in;

	/* Index higher that length? */
	if (index > hashmap_length(m))
		return MAP_MISSING;

	for(i = 0, j = 0; i< m->table_size; i++)
		if(m->data[i].in_use != 0) {
			if (j == index) {
				*key = m->data[i].key;
				if (arg) {
					*arg = (any_t) (m->data[i].data);
				}
				return MAP_OK;
			}
			j++;
		}
	return MAP_MISSING;
}

/*
 * Remove an element with that key from the map
 */
int hashmap_remove(map_t in, int key){
	int i;
	int curr;
	hashmap_map* m;

	/* Cast the hashmap */
	m = (hashmap_map *) in;

	/* Find key */
	curr = hashmap_hash_int(m, key);

	/* Linear probing, if necessary */
	for(i = 0; i< m->table_size; i++){
		if(m->data[curr].key == key && m->data[curr].in_use == 1){
			/* Blank out the fields */
			m->data[curr].in_use = 0;
			m->data[curr].data = NULL;
			m->data[curr].key = 0;

			/* Reduce the size */
			m->size--;
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}

	/* Data not found */
	return MAP_MISSING;
}

/* Deallocate the hashmap */
void hashmap_free(map_t in){
	hashmap_map* m = (hashmap_map*) in;
	free(m->data);
	free(m);
}

/* Return the length of the hashmap */
int hashmap_length(map_t in){
	hashmap_map* m = (hashmap_map *) in;
	if(m != NULL) return m->size;
	else return 0;
}

// We need to keep keys and values
typedef struct _hashmap_element64{
	int64 key;
	int in_use;
	any_t data;
} hashmap_element64;

// A hashmap has some maximum size and current size,
// as well as the data to hold.
typedef struct _hashmap_map64{
	int table_size;
	int size;
	hashmap_element64 *data;
} hashmap_map64;

/*
 * Return an empty hashmap, or NULL on failure.
 */
map_t hashmap64_new() {
	hashmap_map64* m = (hashmap_map64*) malloc(sizeof(hashmap_map64));
	if(!m) goto err;

	m->data = (hashmap_element64*) calloc(INITIAL_SIZE, sizeof(hashmap_element64));
	if(!m->data) goto err;

	m->table_size = INITIAL_SIZE;
	m->size = 0;

	return m;
	err:
		if (m)
			hashmap64_free(m);
		return NULL;
}

/*
 * Hashing function for a 64bit value
 */
unsigned int hashmap64_hash_int(hashmap_map64 * m, uint64 key){
	key = (~key) + (key << 18);
	key = key ^ (key >> 31);
	key = key * 21;
	key = key ^ (key >> 11);
	key = key + (key << 6);
	key = key ^ (key >> 22);

	return key % m->table_size;
}

/*
 * Return the integer of the location in data
 * to store the point to the item, or MAP_FULL.
 */
int hashmap64_hash(map_t in, int64 key){
	int curr;
	int i;

	/* Cast the hashmap */
	hashmap_map64* m = (hashmap_map64 *) in;

	/* If full, return immediately */
	if(m->size == m->table_size) return MAP_FULL;

	/* Find the best index */
	curr = hashmap64_hash_int(m, key);

	/* Linear probling */
	for(i = 0; i< m->table_size; i++){
		if(m->data[curr].in_use == 0)
			return curr;

		if(m->data[curr].key == key && m->data[curr].in_use == 1)
			return curr;

		curr = (curr + 1) % m->table_size;
	}

	return MAP_FULL;
}

/*
 * Doubles the size of the hashmap, and rehashes all the elements
 */
int hashmap64_rehash(map_t in){
	int i;
	int old_size;
	hashmap_element64* curr;

	/* Setup the new elements */
	hashmap_map64 *m = (hashmap_map64 *) in;
	hashmap_element64* temp = (hashmap_element64 *)
		calloc(2 * m->table_size, sizeof(hashmap_element64));
	if(!temp) return MAP_OMEM;

	/* Update the array */
	curr = m->data;
	m->data = temp;

	/* Update the size */
	old_size = m->table_size;
	m->table_size = 2 * m->table_size;
	m->size = 0;

	/* Rehash the elements */
	for(i = 0; i < old_size; i++){
		int status = hashmap64_put(m, curr[i].key, curr[i].data);
		if (status != MAP_OK)
			return status;
	}

	free(curr);

	return MAP_OK;
}

/*
 * Add a pointer to the hashmap with some key
 */
int hashmap64_put(map_t in, int64 key, any_t value){
	int index;
	hashmap_map64* m;

	/* Cast the hashmap */
	m = (hashmap_map64 *) in;

	/* Find a place to put our value */
	index = hashmap64_hash(in, key);
	while(index == MAP_FULL){
		if (hashmap64_rehash(in) == MAP_OMEM) {
			return MAP_OMEM;
		}
		index = hashmap64_hash(in, key);
	}

	/* Set the data */
	m->data[index].data = value;
	m->data[index].key = key;
	m->data[index].in_use = 1;
	m->size++; 

	return MAP_OK;
}

/*
 * Get your pointer out of the hashmap with a key
 */
int hashmap64_get(map_t in, int64 key, any_t *arg){
	int curr;
	int i;
	hashmap_map64* m;

	/* Cast the hashmap */
	m = (hashmap_map64 *) in;

	/* Find data location */
	curr = hashmap64_hash_int(m, key);

	/* Linear probing, if necessary */
	for(i = 0; i< m->table_size; i++){

		if(m->data[curr].key == key && m->data[curr].in_use == 1){
			if (arg) {
				*arg = (int *) (m->data[curr].data);
			}
			return MAP_OK;
		}

		curr = (curr + 1) % m->table_size;
	}

	if (arg) {
		*arg = NULL;
	}

	/* Not found */
	return MAP_MISSING;
}

/*
 * Get element at position index from the hashmap
 */
int hashmap64_get_index(map_t in, int index, int64 *key, any_t *arg) {
	hashmap_map64* m;
	int i,j;

	/* Cast the hashmap */
	m = (hashmap_map64 *) in;

	/* Index higher that length? */
	if (index > hashmap64_length(m))
		return MAP_MISSING;

	for(i = 0, j = 0; i< m->table_size; i++)
		if(m->data[i].in_use != 0) {
			if (j == index) {
				*key = m->data[i].key;
				if (arg) {
					*arg = (any_t) (m->data[i].data);
				}
				return MAP_OK;
			}
			j++;
		}
	return MAP_MISSING;
}

/*
 * Remove an element with that key from the map
 */
int hashmap64_remove(map_t in, int64 key){
	int i;
	int curr;
	hashmap_map64* m;

	/* Cast the hashmap */
	m = (hashmap_map64 *) in;

	/* Find key */
	curr = hashmap64_hash_int(m, key);

	/* Linear probing, if necessary */
	for(i = 0; i< m->table_size; i++){
		if(m->data[curr].key == key && m->data[curr].in_use == 1){
			/* Blank out the fields */
			m->data[curr].in_use = 0;
			m->data[curr].data = NULL;
			m->data[curr].key = 0;

			/* Reduce the size */
			m->size--;
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}

	/* Data not found */
	return MAP_MISSING;
}

/* Deallocate the hashmap */
void hashmap64_free(map_t in){
	hashmap_map64* m = (hashmap_map64*) in;
	free(m->data);
	free(m);
}

/* Return the length of the hashmap */
int hashmap64_length(map_t in){
	hashmap_map64* m = (hashmap_map64 *) in;
	if(m != NULL) return m->size;
	else return 0;
}
