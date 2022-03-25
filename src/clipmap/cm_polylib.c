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
 * @file cm_polylib.c
 * @date 2022-02-05
*/

#include "shared.h"
#include "common.h"

#include "common/common_error.h"
#include "cm_polylib.h"
#include "qmath.h"

// counters are only bumped when running single threaded,
// because they are an awefull coherence problem
int	c_active_windings;
int	c_peak_windings;
int	c_winding_allocs;
int	c_winding_points;

/**
 * @brief 
 * @param points 
 * @return 
*/
struct winding *alloc_winding(int points)
{
    struct winding *w;
    //int	s;

    c_winding_allocs++;
    c_winding_points += points;
    c_active_windings++;
    if (c_active_windings > c_peak_windings)
        c_peak_windings = c_active_windings;

    //s = sizeof(vec_t) * 3 * points + sizeof(int);
    //w = z_malloc(s);
    w = z_malloc(sizeof(*w)); // not sure why the specifics above
    memset(w, 0, sizeof(*w));
    return w;
}

/**
 * @brief 
 * @param w 
*/
void free_winding(struct winding *w)
{
    if (*(unsigned *) w == 0xdeaddead)
        com_error(ERR_FATAL, "freed a freed winding");
    *(unsigned *) w = 0xdeaddead;

    c_active_windings--;
    z_free(w);
}

int	c_removed;

/**
 * @brief 
 * @param w 
*/
void remove_colinear_points(struct winding *w)
{
    int	i, j, k;
    vec3_t v1, v2;
    int	nump;
    vec3_t p[MAX_POINTS_ON_WINDING];

    nump = 0;
    for (i = 0; i < w->num_points; i++) {
        j = (i + 1) % w->num_points;
        k = (i + w->num_points - 1) % w->num_points;

        vector_subtract(w->p[j], w->p[i], v1);
        vector_subtract(w->p[i], w->p[k], v2);
        vector_normalize2(v1, v1);
        vector_normalize2(v2, v2);

        if (dot_product(v1, v2) < 0.999) {
            vector_copy(w->p[i], p[nump]);
            nump++;
        }
    }

    if (nump == w->num_points)
        return;

    c_removed += w->num_points - nump;
    w->num_points = nump;

    memcpy(w->p, p, nump * sizeof(p[0]));
}

/**
 * @brief 
 * @param w 
 * @param normal 
 * @param dist 
*/
void winding_plane(struct winding *w, vec3_t normal, vec_t *dist)
{
    vec3_t v1, v2;

    vector_subtract(w->p[1], w->p[0], v1);
    vector_subtract(w->p[2], w->p[0], v2);
    cross_product(v2, v1, normal);
    vector_normalize2(normal, normal);
    *dist = dot_product(w->p[0], normal);
}

/**
 * @brief 
 * @param w 
 * @return 
*/
vec_t winding_area(struct winding *w)
{
    int	i;
    vec3_t d1, d2, cross;
    vec_t total;

    total = 0;
    for (i = 2; i < w->num_points; i++) {
        vector_subtract(w->p[i - 1], w->p[0], d1);
        vector_subtract(w->p[i], w->p[0], d2);
        cross_product(d1, d2, cross);
        total += 0.5 * vector_length(cross);
    }

    return total;
}

/**
 * @brief 
 * @param w 
 * @param mins 
 * @param maxs 
*/
void winding_bounds(struct winding *w, vec3_t mins, vec3_t maxs)
{
    vec_t v;
    int	i, j;

    mins[0] = mins[1] = mins[2] = MAX_MAP_BOUNDS;
    maxs[0] = maxs[1] = maxs[2] = -MAX_MAP_BOUNDS;

    for (i = 0; i < w->num_points; i++) {
        for (j = 0; j < 3; j++) {
            v = w->p[i][j];

            if (v < mins[j])
                mins[j] = v;
            if (v > maxs[j])
                maxs[j] = v;
        }
    }
}

/**
 * @brief 
 * @param w 
 * @param center 
*/
void winding_center(struct winding *w, vec3_t center)
{
    int	i;
    float scale;

    vector_copy(vec3_origin, center);
    for (i = 0; i < w->num_points; i++)
        vector_add(w->p[i], center, center);

    scale = 1.0 / w->num_points;
    vector_scale(center, scale, center);
}

/**
 * @brief 
*/
struct winding *base_winding_for_plane(vec3_t normal, vec_t dist)
{
    int	i, x;
    vec_t max, v;
    vec3_t org, vright, vup;
    struct winding *w;

    // find the major axis

    max = -MAX_MAP_BOUNDS;
    x = -1;
    for (i = 0; i < 3; i++) {
        v = fabs(normal[i]);

        if (v > max) {
            x = i;
            max = v;
        }
    }

    if (x == -1)
        com_error(ERR_DROP, "no axis found");

    vector_copy(vec3_origin, vup);
    switch (x) {
        case 0:
        case 1:
            vup[2] = 1;
            break;
        case 2:
            vup[0] = 1;
            break;
    }

    v = dot_product(vup, normal);
    vector_ma(vup, -v, normal, vup);
    vector_normalize2(vup, vup);

    vector_scale(normal, dist, org);

    cross_product(vup, normal, vright);

    vector_scale(vup, MAX_MAP_BOUNDS, vup);
    vector_scale(vright, MAX_MAP_BOUNDS, vright);

    // project a really big	axis aligned box onto the plane
    w = alloc_winding(4);

    vector_subtract(org, vright, w->p[0]);
    vector_add(w->p[0], vup, w->p[0]);

    vector_add(org, vright, w->p[1]);
    vector_add(w->p[1], vup, w->p[1]);

    vector_add(org, vright, w->p[2]);
    vector_subtract(w->p[2], vup, w->p[2]);

    vector_subtract(org, vright, w->p[3]);
    vector_subtract(w->p[3], vup, w->p[3]);

    w->num_points = 4;

    return w;
}

/**
 * @brief 
*/
struct winding *copy_winding(struct winding *w)
{
    int	size;
    struct winding *c;

    c = alloc_winding(w->num_points);
    size = (intptr_t) ((struct winding *) 0)->p[w->num_points];
    memcpy(c, w, size);
    return c;
}

/**
 * @brief 
 * @param inout 
 * @param normal 
 * @param dist 
 * @param epsilon 
*/
void chop_winding_in_place(struct winding **inout, vec3_t normal, 
                           vec_t dist, vec_t epsilon)
{
    struct winding *in;
    vec_t	dists[MAX_POINTS_ON_WINDING + 4];
    int		sides[MAX_POINTS_ON_WINDING + 4];
    int		counts[3];
    static	vec_t	dot;		// vc 4.2 optimizer bug if not static
    int		i, j;
    vec_t *p1, *p2;
    vec3_t	mid;
    struct winding *f;
    int		maxpts;

    in = *inout;
    counts[0] = counts[1] = counts[2] = 0;

    // determine sides for each point
    for (i = 0; i < in->num_points; i++) {
        dot = dot_product(in->p[i], normal);
        dot -= dist;
        dists[i] = dot;

        if (dot > epsilon)
            sides[i] = SIDE_FRONT;
        else if (dot < -epsilon)
            sides[i] = SIDE_BACK;
        else
            sides[i] = SIDE_ON;

        counts[sides[i]]++;
    }
    sides[i] = sides[0];
    dists[i] = dists[0];

    if (!counts[0]) {
        free_winding(in);
        *inout = NULL;
        return;
    }

    if (!counts[1])
        return;		// inout stays the same

    maxpts = in->num_points + 4;// cant use counts[0]+2 because
                                // of fp grouping errors

    f = alloc_winding(maxpts);

    for (i = 0; i < in->num_points; i++) {
        p1 = in->p[i];

        if (sides[i] == SIDE_ON) {
            vector_copy(p1, f->p[f->num_points]);
            f->num_points++;
            continue;
        }

        if (sides[i] == SIDE_FRONT) {
            vector_copy(p1, f->p[f->num_points]);
            f->num_points++;
        }

        if (sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
            continue;

        // generate a split point
        p2 = in->p[(i + 1) % in->num_points];

        dot = dists[i] / (dists[i] - dists[i + 1]);
        for (j = 0; j < 3; j++) {	
            // avoid round off error when possible
            if (normal[j] == 1)
                mid[j] = dist;
            else if (normal[j] == -1)
                mid[j] = -dist;
            else
                mid[j] = p1[j] + dot * (p2[j] - p1[j]);
        }

        vector_copy(mid, f->p[f->num_points]);
        f->num_points++;
    }

    if (f->num_points > maxpts)
        com_error(ERR_DROP, "points exceeded estimate");
    if (f->num_points > MAX_POINTS_ON_WINDING)
        com_error(ERR_DROP, "MAX_POINTS_ON_WINDING");

    free_winding(in);
    *inout = f;
}
