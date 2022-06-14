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

#ifndef CLIPMAP_CM_POLYLIB_H
#define CLIPMAP_CM_POLYLIB_H

#include "types/vector.h"

struct winding {
    int num_points;
    vec3_t p[4];
};

#define	MAX_POINTS_ON_WINDING	64

#define	SIDE_FRONT	0
#define	SIDE_BACK	1
#define	SIDE_ON		2
#define	SIDE_CROSS	3

#define	CLIP_EPSILON	0.1f

#define MAX_MAP_BOUNDS			65535

// you can define on_epsilon in the makefile as tighter
#ifndef	ON_EPSILON
#define	ON_EPSILON	0.1f
#endif

struct winding *alloc_winding(int points);
void free_winding(struct winding *w);
void remove_colinear_points(struct winding *w);
void winding_plane(struct winding *w, vec3_t normal, vec_t *dist);
vec_t winding_area(struct winding *w);
void winding_bounds(struct winding *w, vec3_t mins, vec3_t maxs);
void winding_center(struct winding *w, vec3_t center); 
struct winding *base_winding_for_plane(vec3_t normal, vec_t dist);
struct winding *copy_winding(struct winding *w);
void chop_winding_in_place(struct winding **inout, vec3_t normal,
                           vec_t dist, vec_t epsilon);

#endif /* CLIPMAP_CM_POLYLIB_H */
