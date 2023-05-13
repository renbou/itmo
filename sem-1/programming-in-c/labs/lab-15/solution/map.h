// dead stupid maps for C
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#ifndef CREN_MAP_H
#define CREN_MAP_H

/**
 * Structure defining an element (the value assigned to some key) of any map_t. This can later
 * be taken out into a separate library in order to change the implementation but overall the structure
 * should stay pretty much the same.
 */
typedef struct map_entry_s {
	// A variable-length string defining the type of the variable stored here,
	// this is technically up to the usage needs.
	const char *type;
	// Pointer to the actual value stored here, depending on the type specified previously.
	const void *value;
} map_entry_t;

/**
 * Cringy map_t-like structure. Since we currently only need a map_t to be able to do these two simple things,
 * but you might want to expand on this - you can create a structure with a similar beginning (the first two pointers)
 * and then add more functions, but ehh.
 */
typedef struct map_s {
	// Function which gets an entry of the map_t which is assigned to the given key, otherwise returns a null.
	const map_entry_t *(*get)(const struct map_s * _map, const char *key);
	// Function which sets the key equal to the value in the map_t. If not specified otherwise, this overwrites
	// previously assigned values. Returns 0 on success and a different code on failure. Positive return codes
	// from here mean that the value can't be set for some reason but it is intended (logic error), negative
	// return codes mean that the value couldn't be set and the map_t is now broken. The implementation should
	// restore the map_t to the previous state, and try to continue working, but overall this means that you
	// shouldn't call any more set's and if you need them - quit with an error.
	int (*set)(const struct map_s * const _map, const char *key, const map_entry_t *value);
	// Function which can be used to iterate thru the map. The current argument should point to the current "iterator"
	// position, and next should be a pointer where the next iterator should be written to. If the current element
	// is the last, then NULL is written. On success a 0 is returned, otherwise -1 is returned if one of the arguments
	// is null, -2 if the current pointer points to an item which isn't part of the map, and -3 if some other error happened.
	int (*iterate)(const struct map_s * const _map, const map_entry_t **current, const map_entry_t ***next);
	// Function which returns a pointer to the first element in the map, this is pretty much needed only for iterating
	// thru the map with the use of iterate.
	const map_entry_t **(*first_entry)(const struct map_s * const _map);
} map_t;

// So that defining custom maps is easier.
#define map_header const map_entry_t *(*get)(const map_t * const _map, const char *key);\
int (*set)(const map_t * const _map, const char *key, const map_entry_t *value);\
int (*iterate)(const struct map_s * const _map, const map_entry_t **current, const map_entry_t ***next);\
const map_entry_t **(*first_entry)(const struct map_s * const _map);

#define custom_map_get(func_name) const map_entry_t * func_name(const map_t * const _map, const char *key)
#define custom_map_set(func_name) int func_name(const map_t * const _map, const char *key, const map_entry_t *value)
#define custom_map_iterate(func_name) int func_name(const struct map_s * const _map, const map_entry_t **current, const map_entry_t ***next)
#define custom_map_first_entry(func_name) const map_entry_t **func_name(const struct map_s * const _map)

#endif //CREN_MAP_H
