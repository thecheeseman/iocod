/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

/**
 * @file math.c
 * @date 2022-02-04
*/
#include "shared.h"

vec3_t vec3_origin = {0, 0, 0};

void clear_bounds(vec3_t mins, vec3_t maxs) 
{
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}

void add_point_to_bounds(const vec3_t v, vec3_t mins, vec3_t maxs) 
{
	if (v[0] < mins[0])
		mins[0] = v[0];
	if (v[0] > maxs[0])
		maxs[0] = v[0];

	if (v[1] < mins[1])
		mins[1] = v[1];
	if (v[1] > maxs[1])
		maxs[1] = v[1];

	if (v[2] < mins[2])
		mins[2] = v[2];
	if (v[2] > maxs[2])
		maxs[2] = v[2];
}

void cross_product(const vec3_t v1, const vec3_t v2, vec3_t cross) 
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

vec_t vector_normalize(vec3_t v) 
{
	float length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = (vec_t) sqrt(length);

	if (length) {
		ilength = 1 / length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
}

vec_t vector_normalize2(const vec3_t v, vec3_t out) 
{
	float length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = (vec_t) sqrt(length);

	if (length) {
		ilength = 1 / length;
		out[0] = v[0] * ilength;
		out[1] = v[1] * ilength;
		out[2] = v[2] * ilength;
	} else {
		vector_clear(out);
	}

	return length;

}

vec_t vector_length(const vec3_t v) 
{
	return (vec_t) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

vec_t vector_length_squared(const vec3_t v) 
{
	return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

vec_t distance(const vec3_t p1, const vec3_t p2) 
{
	vec3_t v;

	vector_subtract(p2, p1, v);
	return vector_length(v);
}

vec_t distance_squared(const vec3_t p1, const vec3_t p2) 
{
	vec3_t v;

	vector_subtract(p2, p1, v);
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}