// slow but simple linear-time complexity map_t implementation
// Copyright (C) 2020, Artem Mikheev <c@renbou.ru>.
// Licensed under the Apache License, Version 2.0.

#ifndef CREN_LINEAR_MAP_H
#define CREN_LINEAR_MAP_H

#include "map.h"

/**
 * Creates a linear map_t. This uses malloc in order to allocate memory for the structure. The resulting map_t's
 * get and set are O(n) in time complexity on average.
 * @return a pointer to the newly created map_t
 */
const map_t* create_linear_map();

/**
 * Destroys the linear map_t, which was previously created using the create_linear_map function. Since there's
 * no other way to create a linear map_t, this shouldn't ever fail, if called on a valid instance.
 * @param linear_map the map_t to destroy
 * @return 0 if the map_t was successfully destroyed, i.e. removed from memory
 * 		   -1 if a null pointer has been passed
 * 		   -2 if the passed pointer isn't a valid linear map_t
 */
int destroy_linear_map(const map_t* linear_map);

#endif //CREN_LINEAR_MAP_H
