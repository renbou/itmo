// slow but simple linear-time complexity map_t implementation
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "linear_map.h"
#include "errors.h"

static const uint32_t LINEAR_MAP_INIT_CAPACITY = 32;

/**
 * Internal structure for our linear map_t - storing all elements as an array.
 */
typedef struct {
	map_header;
	uint32_t size;
	uint32_t capacity;
	const char **keys;
	const map_entry_t **values;
} internal_linear_map;

custom_map_get(internal_linear_map_get) {
	internal_linear_map *linear_map = (internal_linear_map *)(_map);
	for (uint32_t i = 0; i < linear_map->size; i++) {
		if (strcmp(linear_map->keys[i], key) == 0) {
			return linear_map->values[i];
		}
	}
	return NULL;
}

custom_map_set(internal_linear_map_set) {
	internal_linear_map *linear_map = (internal_linear_map *)(_map);
	// First try to find the key in the map_t, in the case that it is already is present we don't need to expand
	for (uint32_t i = 0; i < linear_map->size; i++) {
		if (strcmp(linear_map->keys[i], key) == 0) {
			linear_map->values[i] = value;
			return 0;
		}
	}

	// expand if we need to
	if (linear_map->size == linear_map->capacity) {
		// expand to twice of capacity, this way expansions take O(1) amortized
		uint32_t new_capacity = linear_map->capacity * 2;
		const char **new_keys = NULL;
		const map_entry_t **new_values = NULL;
		if ((new_keys = (const char **)malloc(sizeof(const char *) * new_capacity)) != NULL &&
			(new_values = (const map_entry_t **)malloc(sizeof(map_entry_t *) * new_capacity)) != NULL) {
			memccpy(new_keys, linear_map->keys, sizeof(const char *), linear_map->capacity);
			memccpy(new_values, linear_map->values, sizeof(map_entry_t *), linear_map->capacity);

			// set the second half to zeroes
			memset(new_keys + linear_map->capacity, 0, sizeof(const char *) * linear_map->capacity);
			memset(new_values + linear_map->capacity, 0, sizeof(map_entry_t *) * linear_map->capacity);

			free(linear_map->keys);
			free(linear_map->values);

			linear_map->keys = new_keys;
			linear_map->values = new_values;
			linear_map->capacity = new_capacity;
		} else {
		// can't expand the map_t, so don't change the map_t and revert to the previous state (do nothing)
			if (new_keys != NULL) {
				free(new_keys);
			}
			if (new_values != NULL) {
				free(new_values);
			}

			return -1;
		}
	}

	linear_map->keys[linear_map->size] = key;
	linear_map->values[linear_map->size] = value;

	linear_map->size++;
	return 0;
}

custom_map_iterate(internal_linear_map_iterate) {
	internal_linear_map *linear_map = (internal_linear_map *)(_map);

	if (linear_map == NULL || current == NULL || next == NULL) {
		return -1;
	}

	uint32_t iterator_position = current - linear_map->values;
	if (iterator_position >= linear_map->size) {
		return -2;
	}

	if (iterator_position + 1 == linear_map->size) {
		*next = NULL;
	} else {
		*next = &linear_map->values[iterator_position + 1];
	}

	return 0;
}

custom_map_first_entry(internal_linear_map_first_entry) {
	internal_linear_map *linear_map = (internal_linear_map *)(_map);
	if (linear_map->size != 0) {
		return linear_map->values;
	}
	return NULL;
}

const map_t* create_linear_map() {
	internal_linear_map * linear_map = (internal_linear_map *)malloc(sizeof(internal_linear_map));
	linear_map->get = internal_linear_map_get;
	linear_map->set = internal_linear_map_set;
	linear_map->first_entry = internal_linear_map_first_entry;
	linear_map->iterate = internal_linear_map_iterate;

	with(linear_map != NULL, free(linear_map), {
		linear_map->keys = (const char **)malloc(sizeof(const char *) * LINEAR_MAP_INIT_CAPACITY);

		with(linear_map->keys != NULL, free(linear_map->keys), {
			linear_map->values = (const map_entry_t **)malloc(sizeof(const map_entry_t *) * LINEAR_MAP_INIT_CAPACITY);

			with(linear_map->values != NULL, free(linear_map->values), {
				// fill map with zeros
				memset(linear_map->keys, 0, sizeof(const char *) * LINEAR_MAP_INIT_CAPACITY);
				memset(linear_map->values, 0, sizeof(map_entry_t *) * LINEAR_MAP_INIT_CAPACITY);

				linear_map->size = 0;
				linear_map->capacity = LINEAR_MAP_INIT_CAPACITY;

				return (map_t *)linear_map;
			});
		});
	});
	return NULL;
}

int destroy_linear_map(const map_t* _map) {
	internal_linear_map * linear_map = (internal_linear_map *)(_map);

	if (linear_map == NULL) {
		return -1;
	}

	// maybe in the future we can implement a better way to systematically identify the type of the map_t
	// and its correctness, but currently this'll do
	if (linear_map->size > linear_map->capacity ||
			linear_map->keys == NULL || linear_map->values == NULL) {
		return -2;
	}

	free(linear_map->values);
	free(linear_map->keys);
	free(linear_map);
	return 0;
}
