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
 * @file cm_patch.c
 * @date 2022-02-04
*/

#include "cm_local.h"
#include "qmath.h"

static int num_planes;
static struct patch_plane planes[MAX_PATCH_PLANES];

//static const struct patch_collide *debug_patch_collide;
//static const struct facet *debug_facet;
static bool debug_block;
static vec3_t debug_block_points[4];

/**
 * @brief 
 * @param normal 
 * @return 
*/
static int cm_sign_bits_for_normal(vec3_t normal)
{
    int	bits, j;

    bits = 0;
    for (j = 0; j < 3; j++) {
        if (normal[j] < 0)
            bits |= 1 << j;
    }

    return bits;
}

/**
 * @brief Returns false if the triangle is degenrate. The normal will 
 * point out of the clock for clockwise ordered points
 * 
 * @param plane 
 * @param a 
 * @param b 
 * @param c 
 * @return 
*/
static bool cm_plane_from_points(vec4_t plane, vec3_t a, vec3_t b, vec3_t c)
{
    vec3_t d1, d2;

    vector_subtract(b, a, d1);
    vector_subtract(c, a, d2);
    cross_product(d2, d1, plane);

    if (vector_normalize(plane) == 0)
        return false;

    plane[3] = dot_product(a, plane);
    return true;
}

/**
 * @brief Returns true if the given quadratic curve is not flat
 * enough for our collision detection purposes
 *
 * @param a 
 * @param b 
 * @param c 
 * @param first_vert 
 * @return 
 * 
 * @todo compare against executable & assign static once finished
*/
bool cm_needs_subdivision(vec3_t a, vec3_t b, vec3_t c, int first_vert) {
    vec3_t cmid;
    vec3_t lmid;
    vec3_t delta;
    float dist;
    int	i;

    UNUSED(first_vert);

    // calculate the linear midpoint
    for (i = 0; i < 3; i++)
        lmid[i] = 0.5 * (a[i] + c[i]);

    // calculate the exact curve midpoint
    for (i = 0; i < 3; i++)
        cmid[i] = 0.5 * (0.5 * (a[i] + b[i]) + 0.5 * (b[i] + c[i]));

    // see if the curve is far enough away from the linear mid
    vector_subtract(cmid, lmid, delta);
    dist = vector_length(delta);

    return dist >= SUBDIVIDE_DISTANCE;
}

/**
 * @brief a, b, and c are control points. the subdivided sequence
 * will be: a, out1, out2, out3, c
 * 
 * @param a 
 * @param b 
 * @param c 
 * @param out1 
 * @param out2 
 * @param out3 
*/
static void cm_subdivide(vec3_t a, vec3_t b, vec3_t c,
                         vec3_t out1, vec3_t out2, vec3_t out3)
{
    int i;

    for (i = 0; i < 3; i++) {
        out1[i] = 0.5 * (a[i] + b[i]);
        out3[i] = 0.5 * (b[i] + c[i]);
        out2[i] = 0.5 * (out1[i] + out3[i]);
    }
}

/**
 * @brief Swap the rows and columns in place
 * 
 * @param grid 
*/
static void cm_transpose_grid(struct grid *grid)
{
    int i, j, l;
    vec3_t temp;
    bool temp_wrap;

    if (grid->width > grid->height) {
        for (i = 0; i < grid->height; i++) {
            for (j = i + 1; j < grid->width; j++) {
                if (j < grid->height) {
                    // swap the value
                    vector_copy(grid->points[i][j], temp);
                    vector_copy(grid->points[j][i], grid->points[i][j]);
                    vector_copy(temp, grid->points[j][i]);
                } else {
                    // just copy
                    vector_copy(grid->points[j][i], grid->points[i][j]);
                }
            }
        }
    } else {
        for (i = 0; i < grid->width; i++) {
            for (j = i + 1; j < grid->height; j++) {
                if (j < grid->width) {
                    // swap the value
                    vector_copy(grid->points[j][i], temp);
                    vector_copy(grid->points[i][j], grid->points[j][i]);
                    vector_copy(temp, grid->points[i][j]);
                } else {
                    // just copy
                    vector_copy(grid->points[i][j], grid->points[j][i]);
                }
            }
        }
    }

    l = grid->width;
    grid->width = grid->height;
    grid->height = l;

    temp_wrap = grid->wrap_width;
    grid->wrap_width = grid->wrap_height;
    grid->wrap_height = temp_wrap;
}

/**
 * @brief If the left and right columns are exactly equal, 
 * set grid->wrap_width to true
 * 
 * @param grid
*/
static void cm_set_grid_wrap_width(struct grid *grid)
{
    int i, j;
    float d;

    for (i = 0; i < grid->height; i++) {
        for (j = 0; j < 3; j++) {
            d = grid->points[0][i][j] - grid->points[grid->width - 1][i][j];
            if (d < -WRAP_POINT_EPSILON || d > WRAP_POINT_EPSILON)
                break;
        }

        if (j != 3)
            break;
    }

    if (i == grid->height)
        grid->wrap_width = true;
    else
        grid->wrap_width = false;
}

/**
 * @brief Adds columns as necessary to the grid until all the
 * approximating points are within SUBDIVIDE_DISTANCE from the
 * true curve
 * 
 * @param grid
 * @param first_vert UNUSED
*/
static void cm_subdivide_grid_columns(struct grid *grid, int first_vert)
{
    int i, j, k;
    vec3_t prev, mid, next;

    for (i = 0; i < grid->width - 2;) {
        // first see if we can collapse the approximating column away
        for (j = 0; j < grid->height; j++) {
            if (cm_needs_subdivision(grid->points[i][j],
                                     grid->points[i + 1][j],
                                     grid->points[i + 2][j],
                                     first_vert)) {
                break;
            }
        }

        if (j == grid->height) {
            // all of the points were close enough to the linear midpoints
            // that we can collapse the entire column away
            for (j = 0; j < grid->height; j++) {
                // remove the column
                for (k = i + 2; k < grid->width; k++) {
                    vector_copy(grid->points[k][j], grid->points[k - 1][j]);
                }
            }

            grid->width--;

            // go to the next curve segment
            i++;
            continue;
        }

        // we need to subdivide the curve
        for (j = 0; j < grid->height; j++) {
            // save the control points now
            vector_copy(grid->points[i][j], prev);
            vector_copy(grid->points[i + 1][j], mid);
            vector_copy(grid->points[i + 2][j], next);

            // make room for two additional columns in the grid
            // columns i + 1 will be replaced, column i + 2 will become i + 4
            // i + 1, i + 2, and i + 3 will be generated
            for (k = grid->width - 1; k > i + 1; k--) {
                vector_copy(grid->points[k][j], grid->points[k + 2][j]);
            }

            // generate the subdivided points
            cm_subdivide(prev, mid, next,
                         grid->points[i + 1][j],
                         grid->points[i + 2][j],
                         grid->points[i + 3][j]);
        }

        grid->width += 2;

        // the new aproximating point at i + 1 may need to be removed
        // or subdivided farther, so don't advance i
    }
}

#define	POINT_EPSILON	0.1
/**
 * @brief 
 * @param a 
 * @param b 
 * @return 
*/
static bool cm_compare_points(float *a, float *b) 
{
    float d;

    d = a[0] - b[0];
    if (d < -POINT_EPSILON || d > POINT_EPSILON)
        return false;

    d = a[1] - b[1];
    if (d < -POINT_EPSILON || d > POINT_EPSILON)
        return false;

    d = a[2] - b[2];
    if (d < -POINT_EPSILON || d > POINT_EPSILON)
        return false;

    return true;
}

/**
 * @brief If there are any identical columns, remove them
 * 
 * @param grid 
*/
static void cm_remove_degenerate_columns(struct grid *grid)
{
    int i, j, k;

    for (i = 0; i < grid->width - 1; i++) {
        for (j = 0; j < grid->height; j++) {
            if (!cm_compare_points(grid->points[i][j], grid->points[i + 1][j]))
                break;
        }

        if (j != grid->height)
            continue;	// not degenerate

        for (j = 0; j < grid->height; j++) {
            // remove the column
            for (k = i + 2; k < grid->width; k++) {
                vector_copy(grid->points[k][j], grid->points[k - 1][j]);
            }
        }
        grid->width--;

        // check against the next column
        i--;
    }
}

#define	NORMAL_EPSILON	0.0001
#define	DIST_EPSILON	0.02

/**
 * @brief 
 * @param p 
 * @param plane 
 * @param flipped 
 * @return 
*/
bool cm_plane_equal(struct patch_plane *p, float plane[4], bool *flipped) 
{
    float invplane[4];

    if (fabs(p->plane[0] - plane[0]) < NORMAL_EPSILON
        && fabs(p->plane[1] - plane[1]) < NORMAL_EPSILON
        && fabs(p->plane[2] - plane[2]) < NORMAL_EPSILON
        && fabs(p->plane[3] - plane[3]) < DIST_EPSILON) {
        *flipped = false;
        return true;
    }

    vector_negate(plane, invplane);
    invplane[3] = -plane[3];

    if (fabs(p->plane[0] - invplane[0]) < NORMAL_EPSILON
        && fabs(p->plane[1] - invplane[1]) < NORMAL_EPSILON
        && fabs(p->plane[2] - invplane[2]) < NORMAL_EPSILON
        && fabs(p->plane[3] - invplane[3]) < DIST_EPSILON) {
        *flipped = true;
        return true;
    }

    return false;
}

/**
 * @brief 
 * @param normal 
*/
void cm_snap_vector(vec3_t normal) 
{
    int	i;

    for (i = 0; i < 3; i++) {
        if (fabs(normal[i] - 1) < NORMAL_EPSILON) {
            vector_clear(normal);
            normal[i] = 1;
            break;
        }

        if (fabs(normal[i] - -1) < NORMAL_EPSILON) {
            vector_clear(normal);
            normal[i] = -1;
            break;
        }
    }
}

/**
 * @brief 
 * @param plane 
 * @param flipped 
 * @return 
*/
int cm_find_plane2(float plane[4], bool *flipped) 
{
    int i;

    // see if the points are close enough to an existing plane
    for (i = 0; i < num_planes; i++) {
        if (cm_plane_equal(&planes[i], plane, flipped)) 
            return i;
    }

    // add a new plane
    if (num_planes == MAX_PATCH_PLANES)
        com_error(ERR_DROP, "MAX_PATCH_PLANES");

    vector4_copy(plane, planes[num_planes].plane);
    planes[num_planes].sign_bits = cm_sign_bits_for_normal(plane);

    num_planes++;

    *flipped = false;

    return num_planes - 1;
}

/**
 * @brief 
 * @param p1 
 * @param p2 
 * @param p3 
 * @return 
*/
static int cm_find_plane(float *p1, float *p2, float *p3)
{
    vec4_t plane;
    int	i;
    float d;

    if (!cm_plane_from_points(plane, p1, p2, p3))
        return -1;

    // see if the points are close enough to an existing plane
    for (i = 0; i < num_planes; i++) {
        if (dot_product(plane, planes[i].plane) < 0)
            continue;	// allow backwards planes?

        d = dot_product(p1, planes[i].plane) - planes[i].plane[3];
        if (d < -PLANE_TRI_EPSILON || d > PLANE_TRI_EPSILON)
            continue;

        d = dot_product(p2, planes[i].plane) - planes[i].plane[3];
        if (d < -PLANE_TRI_EPSILON || d > PLANE_TRI_EPSILON)
            continue;

        d = dot_product(p3, planes[i].plane) - planes[i].plane[3];
        if (d < -PLANE_TRI_EPSILON || d > PLANE_TRI_EPSILON)
            continue;

        // found it
        return i;
    }

    // add a new plane
    if (num_planes == MAX_PATCH_PLANES)
        com_error(ERR_DROP, "MAX_PATCH_PLANES");

    vector4_copy(plane, planes[num_planes].plane);
    planes[num_planes].sign_bits = cm_sign_bits_for_normal(plane);

    num_planes++;

    return num_planes - 1;
}

/**
 * @brief 
 * @param p 
 * @param planeNum 
 * @return 
*/
static int cm_point_on_plane_side(float *p, int plane_num) 
{
    float *plane;
    float	d;

    if (plane_num == -1)
        return SIDE_ON;

    plane = planes[plane_num].plane;

    d = dot_product(p, plane) - plane[3];

    if (d > PLANE_TRI_EPSILON)
        return SIDE_FRONT;

    if (d < -PLANE_TRI_EPSILON)
        return SIDE_BACK;

    return SIDE_ON;
}

/**
 * @brief 
 * @param grid_planes 
 * @param i 
 * @param j 
 * @param tri 
 * @return 
*/
static int cm_grid_plane(int grid_planes[MAX_GRID_SIZE][MAX_GRID_SIZE][2],
                         int i, int j, int tri)
{
    int		p;

    p = grid_planes[i][j][tri];
    if (p != -1)
        return p;

    p = grid_planes[i][j][!tri];
    if (p != -1)
        return p;

    // should never happen
    com_warning("cm_grid_plane unresolvable\n");
    return -1;
}

/**
 * @brief 
 * @param grid 
 * @param grid_planes 
 * @param i 
 * @param j 
 * @param k 
 * @return 
*/
static int cm_edge_plane_num(struct grid *grid,
                             int grid_planes[MAX_GRID_SIZE][MAX_GRID_SIZE][2],
                             int i, int j, int k)
{
    float *p1, *p2;
    vec3_t up;
    int	 p;

    switch (k) {
        case 0:	// top border
            p1 = grid->points[i][j];
            p2 = grid->points[i + 1][j];
            p = cm_grid_plane(grid_planes, i, j, 0);
            vector_ma(p1, 4, planes[p].plane, up);
            return cm_find_plane(p1, p2, up);

        case 2:	// bottom border
            p1 = grid->points[i][j + 1];
            p2 = grid->points[i + 1][j + 1];
            p = cm_grid_plane(grid_planes, i, j, 1);
            vector_ma(p1, 4, planes[p].plane, up);
            return cm_find_plane(p2, p1, up);

        case 3: // left border
            p1 = grid->points[i][j];
            p2 = grid->points[i][j + 1];
            p = cm_grid_plane(grid_planes, i, j, 1);
            vector_ma(p1, 4, planes[p].plane, up);
            return cm_find_plane(p2, p1, up);

        case 1:	// right border
            p1 = grid->points[i + 1][j];
            p2 = grid->points[i + 1][j + 1];
            p = cm_grid_plane(grid_planes, i, j, 0);
            vector_ma(p1, 4, planes[p].plane, up);
            return cm_find_plane(p1, p2, up);

        case 4:	// diagonal out of triangle 0
            p1 = grid->points[i + 1][j + 1];
            p2 = grid->points[i][j];
            p = cm_grid_plane(grid_planes, i, j, 0);
            vector_ma(p1, 4, planes[p].plane, up);
            return cm_find_plane(p1, p2, up);

        case 5:	// diagonal out of triangle 1
            p1 = grid->points[i][j];
            p2 = grid->points[i + 1][j + 1];
            p = cm_grid_plane(grid_planes, i, j, 1);
            vector_ma(p1, 4, planes[p].plane, up);
            return cm_find_plane(p1, p2, up);

    }

    com_error(ERR_DROP, "bad k");
    return -1;
}

/**
 * @brief 
 * @param facet 
 * @param grid 
 * @param grid_planes 
 * @param i 
 * @param j 
 * @param which 
*/
static void INCOMPLETE cm_set_border_inward(struct facet *facet, struct grid *grid,
                                 int grid_planes[MAX_GRID_SIZE][MAX_GRID_SIZE][2],
                                 int i, int j, int which)
{
    int	k, l;
    float *points[4];
    int	numPoints;

    UNUSED(grid_planes);

    switch (which) {
        case -1:
            points[0] = grid->points[i][j];
            points[1] = grid->points[i + 1][j];
            points[2] = grid->points[i + 1][j + 1];
            points[3] = grid->points[i][j + 1];
            numPoints = 4;
            break;
        case 0:
            points[0] = grid->points[i][j];
            points[1] = grid->points[i + 1][j];
            points[2] = grid->points[i + 1][j + 1];
            numPoints = 3;
            break;
        case 1:
            points[0] = grid->points[i + 1][j + 1];
            points[1] = grid->points[i][j + 1];
            points[2] = grid->points[i][j];
            numPoints = 3;
            break;
        default:
            com_error(ERR_FATAL, "bad parameter");
            numPoints = 0;
            break;
    }

    for (k = 0; k < facet->num_borders; k++) {
        int	front, back;

        front = 0;
        back = 0;

        for (l = 0; l < numPoints; l++) {
            int	side;

            side = cm_point_on_plane_side(points[l], facet->border_planes[k]);
            if (side == SIDE_FRONT)
                front++;
            if (side == SIDE_BACK)
                back++;
        }

        if (front > 0 && back == 0) {
            facet->border_inward[k] = true;
        } else if (back > 0&& front == 0) {
            facet->border_inward[k] = false;
        } else if (front == 0 && back == 0) {
            // flat side border
            facet->border_planes[k] = -1;
        } else {
            // bisecting side border
            com_debug_warning("mixed plane sides\n");
            facet->border_inward[k] = false;

            if (!debug_block) {
                debug_block = true;

                vector_copy(grid->points[i][j], debug_block_points[0]);
                vector_copy(grid->points[i + 1][j], debug_block_points[1]);
                vector_copy(grid->points[i + 1][j + 1], debug_block_points[2]);
                vector_copy(grid->points[i][j + 1], debug_block_points[3]);
            }
        }
    }
}

/**
 * @brief If the facet isn't bounded by its borders, we screwed up
 * @param facet 
 * @return 
 * 
 * @todo finish function
*/
bool cm_validate_facet(struct facet *facet)
{
     float plane[4];
     int j;
     struct winding *w;
     vec3_t	bounds[2];

     if (facet->surface_plane == -1)
         return false;

     vector4_copy(planes[facet->surface_plane].plane, plane);
     w = base_winding_for_plane(plane, plane[3]);

     for (j = 0; j < facet->num_borders && w; j++) {
         if (facet->border_planes[j] == -1)
             return false;

         vector4_copy(planes[facet->border_planes[j]].plane, plane);
         if (!facet->border_inward[j]) {
             vector_subtract(vec3_origin, plane, plane);
             plane[3] = -plane[3];
         }

         chop_winding_in_place(&w, plane, plane[3], 0.1f);
     }

     if (w == NULL)
         return false;		// winding was completely chopped away

     // see if the facet is unreasonably large
     winding_bounds(w, bounds[0], bounds[1]);
     free_winding(w);

     for (j = 0; j < 3; j++) {
         if (bounds[1][j] - bounds[0][j] > MAX_MAP_BOUNDS)
             return false;		// we must be missing a plane
         
         if (bounds[0][j] >= MAX_MAP_BOUNDS)
             return false;
         
         if (bounds[1][j] <= -MAX_MAP_BOUNDS)
             return false;
     }

     return true;		// winding is fine
}

/**
 * @brief 
 * @param facet 
 * 
 * @todo finish function
*/
void cm_add_facet_bevels(struct facet *facet)
{

    int i, j, k, l;
    int axis, dir, order;
    bool flipped;
    float plane[4], d, newplane[4];
    struct winding *w, *w2;
    vec3_t mins, maxs, vec, vec2;

    vector4_copy(planes[facet->surface_plane].plane, plane);

    w = base_winding_for_plane(plane, plane[3]);
    for (j = 0; j < facet->num_borders && w; j++) {
        if (facet->border_planes[j] == facet->surface_plane)
            continue;

        vector4_copy(planes[facet->border_planes[j]].plane, plane);

        if (!facet->border_inward[j]) {
            vector_subtract(vec3_origin, plane, plane);
            plane[3] = -plane[3];
        }

        chop_winding_in_place(&w, plane, plane[3], 0.1f);
    }

    if (w == NULL)
        return;

    winding_bounds(w, mins, maxs);

    // add the axial planes
    order = 0;
    for (axis = 0; axis < 3; axis++) {
        for (dir = -1; dir <= 1; dir += 2, order++) {
            vector_clear(plane);
            plane[axis] = dir;

            if (dir == 1)
                plane[3] = maxs[axis];
            else
                plane[3] = -mins[axis];

            //if it's the surface plane
            if (cm_plane_equal(&planes[facet->surface_plane], 
                              plane, &flipped)) {
                continue;
            }

            // see if the plane is allready present
            for (i = 0; i < facet->num_borders; i++) {
                if (cm_plane_equal(&planes[facet->border_planes[i]],
                                  plane, &flipped)) {
                    break;
                }
            }

            if (i == facet->num_borders) {
                if (facet->num_borders > 4 + 6 + 16) 
                    com_debug_warning("too many bevels\n");

                facet->border_planes[facet->num_borders] = 
                    cm_find_plane2(plane, &flipped);
                facet->border_noadjust[facet->num_borders] = 0;
                facet->border_inward[facet->num_borders] = flipped;
                facet->num_borders++;
            }
        }
    }
    //
    // add the edge bevels
    //
    // test the non-axial plane edges
    for (j = 0; j < w->num_points; j++) {
        k = (j + 1) % w->num_points;
        vector_subtract(w->p[j], w->p[k], vec);

        //if it's a degenerate edge
        if (vector_normalize(vec) < 0.5)
            continue;

        cm_snap_vector(vec);

        for (k = 0; k < 3; k++) {
            if (vec[k] == -1 || vec[k] == 1)
                break;	// axial
        }

        if (k < 3)
            continue;	// only test non-axial edges

        // try the six possible slanted axials from this edge
        for (axis = 0; axis < 3; axis++) {
            for (dir = -1; dir <= 1; dir += 2) {
                // construct a plane
                vector_clear(vec2);
                vec2[axis] = dir;
                cross_product(vec, vec2, plane);

                if (vector_normalize(plane) < 0.5)
                    continue;

                plane[3] = dot_product(w->p[j], plane);

                // if all the points of the facet winding are
                // behind this plane, it is a proper edge bevel
                for (l = 0; l < w->num_points; l++) {
                    d = dot_product(w->p[l], plane) - plane[3];

                    if (d > 0.1)
                        break;	// point in front
                }

                if (l < w->num_points)
                    continue;

                //if it's the surface plane
                if (cm_plane_equal(&planes[facet->surface_plane], 
                                  plane, &flipped)) {
                    continue;
                }

                // see if the plane is allready present
                for (i = 0; i < facet->num_borders; i++) {
                    if (cm_plane_equal(&planes[facet->border_planes[i]], 
                                      plane, &flipped)) {
                        break;
                    }
                }

                if (i == facet->num_borders) {
                    if (facet->num_borders > 4 + 6 + 16) 
                        com_debug_warning("too many bevels\n");

                    facet->border_planes[facet->num_borders] = 
                        cm_find_plane2(plane, &flipped);

                    for (k = 0; k < facet->num_borders; k++) {
                        if (facet->border_planes[facet->num_borders] ==
                            facet->border_planes[k]) {
                            com_debug_warning("bevel plane already used\n");
                        }
                    }

                    facet->border_noadjust[facet->num_borders] = 0;
                    facet->border_inward[facet->num_borders] = flipped;

                    w2 = copy_winding(w);
                    vector4_copy(
                        planes[facet->border_planes[facet->num_borders]].plane, 
                        newplane);

                    if (!facet->border_inward[facet->num_borders]) {
                        vector_negate(newplane, newplane);
                        newplane[3] = -newplane[3];
                    } 

                    chop_winding_in_place(&w2, newplane, newplane[3], 0.1f);
                    if (!w2) {
                        com_debug_warning("invalid bevel\n");
                        continue;
                    } else {
                        free_winding(w2);
                    }

                    facet->num_borders++;
                }
            }
        }
    }

    free_winding(w);

    // add opposite plane
    facet->border_planes[facet->num_borders] = facet->surface_plane;
    facet->border_noadjust[facet->num_borders] = 0;
    facet->border_inward[facet->num_borders] = true;
    facet->num_borders++;
}

enum edgename {
    EN_TOP,
    EN_RIGHT,
    EN_BOTTOM,
    EN_LEFT
};

/**
 * @brief 
 * @param grid 
 * @param pf 
*/
static void cm_patch_collide_from_grid(struct grid *grid,
                                       struct patch_collide *pf)
{
    int i, j;
    float *p1, *p2, *p3;

    struct facet facets[MAX_FACETS];
    struct facet *facet;
    int grid_planes[MAX_GRID_SIZE][MAX_GRID_SIZE][2];
    int borders[4], noadjust[4];
    int num_facets;

    num_planes = 0;
    num_facets = 0;

    // find the planes for each triangle of the grid
    for (i = 0; i < grid->width - 1; i++) {
        for (j = 0; j < grid->height - 1; j++) {
            p1 = grid->points[i][j];
            p2 = grid->points[i + 1][j];
            p3 = grid->points[i + 1][j + 1];
            grid_planes[i][j][0] = cm_find_plane(p1, p2, p3);

            p1 = grid->points[i + 1][j + 1];
            p2 = grid->points[i][j + 1];
            p3 = grid->points[i][j];
            grid_planes[i][j][1] = cm_find_plane(p1, p2, p3);
        }
    }

    // create the borders for each facet
    for (i = 0; i < grid->width - 1; i++) {
        for (j = 0; j < grid->height - 1; j++) {
            borders[EN_TOP] = -1;

            if (j > 0)
                borders[EN_TOP] = grid_planes[i][j - 1][1];
            else if (grid->wrap_height)
                borders[EN_TOP] = grid_planes[i][grid->height - 2][1];
            
            noadjust[EN_TOP] = (borders[EN_TOP] == grid_planes[i][j][0]);
            if (borders[EN_TOP] == -1 || noadjust[EN_TOP])
                borders[EN_TOP] = cm_edge_plane_num(grid, grid_planes, i, j, 0);

            borders[EN_BOTTOM] = -1;
            if (j < grid->height - 2)
                borders[EN_BOTTOM] = grid_planes[i][j + 1][0];
            else if (grid->wrap_height)
                borders[EN_BOTTOM] = grid_planes[i][0][0];

            noadjust[EN_BOTTOM] = (borders[EN_BOTTOM] == grid_planes[i][j][1]);
            if (borders[EN_BOTTOM] == -1 || noadjust[EN_BOTTOM])
                borders[EN_BOTTOM] = cm_edge_plane_num(grid, grid_planes, 
                                                       i, j, 2);

            borders[EN_LEFT] = -1;
            if (i > 0)
                borders[EN_LEFT] = grid_planes[i - 1][j][0];
            else if (grid->wrap_width)
                borders[EN_LEFT] = grid_planes[grid->width - 2][j][0];

            noadjust[EN_LEFT] = (borders[EN_LEFT] == grid_planes[i][j][1]);
            if (borders[EN_LEFT] == -1 || noadjust[EN_LEFT])
                borders[EN_LEFT] = cm_edge_plane_num(grid, grid_planes, 
                                                     i, j, 3);

            borders[EN_RIGHT] = -1;
            if (i < grid->width - 2)
                borders[EN_RIGHT] = grid_planes[i + 1][j][1];
            else if (grid->wrap_width)
                borders[EN_RIGHT] = grid_planes[0][j][1];

            noadjust[EN_RIGHT] = (borders[EN_RIGHT] == grid_planes[i][j][0]);
            if (borders[EN_RIGHT] == -1 || noadjust[EN_RIGHT])
                borders[EN_RIGHT] = cm_edge_plane_num(grid, grid_planes,
                                                      i, j, 1);

            if (num_facets == MAX_FACETS)
                com_error(ERR_DROP, "MAX_FACETS");

            facet = &facets[num_facets];
            memset(facet, 0, sizeof(*facet));

            if (grid_planes[i][j][0] == grid_planes[i][j][1]) {
                if (grid_planes[i][j][0] == -1)
                    continue;		// degenrate

                facet->surface_plane = grid_planes[i][j][0];
                facet->num_borders = 4;
                facet->border_planes[0] = borders[EN_TOP];
                facet->border_noadjust[0] = noadjust[EN_TOP];
                facet->border_planes[1] = borders[EN_RIGHT];
                facet->border_noadjust[1] = noadjust[EN_RIGHT];
                facet->border_planes[2] = borders[EN_BOTTOM];
                facet->border_noadjust[2] = noadjust[EN_BOTTOM];
                facet->border_planes[3] = borders[EN_LEFT];
                facet->border_noadjust[3] = noadjust[EN_LEFT];

                cm_set_border_inward(facet, grid, grid_planes, i, j, -1);
                if (cm_validate_facet(facet)) {
                    cm_add_facet_bevels(facet);
                    num_facets++;
                }
            } else {
                // two seperate triangles
                facet->surface_plane = grid_planes[i][j][0];
                facet->num_borders = 3;
                facet->border_planes[0] = borders[EN_TOP];
                facet->border_noadjust[0] = noadjust[EN_TOP];
                facet->border_planes[1] = borders[EN_RIGHT];
                facet->border_noadjust[1] = noadjust[EN_RIGHT];
                facet->border_planes[2] = grid_planes[i][j][1];

                if (facet->border_planes[2] == -1) {
                    facet->border_planes[2] = borders[EN_BOTTOM];

                    if (facet->border_planes[2] == -1) {
                        facet->border_planes[2] =
                            cm_edge_plane_num(grid, grid_planes, i, j, 4);
                    }
                }

                cm_set_border_inward(facet, grid, grid_planes, i, j, 0);
                if (cm_validate_facet(facet)) {
                    cm_add_facet_bevels(facet);
                    num_facets++;
                }

                if (num_facets == MAX_FACETS)
                    com_error(ERR_DROP, "MAX_FACETS");

                facet = &facets[num_facets];
                memset(facet, 0, sizeof(*facet));

                facet->surface_plane = grid_planes[i][j][1];
                facet->num_borders = 3;
                facet->border_planes[0] = borders[EN_BOTTOM];
                facet->border_noadjust[0] = noadjust[EN_BOTTOM];
                facet->border_planes[1] = borders[EN_LEFT];
                facet->border_noadjust[1] = noadjust[EN_LEFT];
                facet->border_planes[2] = grid_planes[i][j][0];
                if (facet->border_planes[2] == -1) {
                    facet->border_planes[2] = borders[EN_TOP];

                    if (facet->border_planes[2] == -1) {
                        facet->border_planes[2] = 
                            cm_edge_plane_num(grid, grid_planes, i, j, 5);
                    }
                }

                cm_set_border_inward(facet, grid, grid_planes, i, j, 1);
                if (cm_validate_facet(facet)) {
                    cm_add_facet_bevels(facet);
                    num_facets++;
                }
            }
        }
    }

    // copy the results out
    pf->num_planes = num_planes;
    pf->num_facets = num_facets;
    pf->facets = hunk_alloc(num_facets * sizeof(*pf->facets));
    memcpy(pf->facets, facets, num_facets * sizeof(*pf->facets));
    pf->planes = hunk_alloc(num_planes * sizeof(*pf->planes));
    memcpy(pf->planes, planes, num_planes * sizeof(*pf->planes));
}

/**
 * @brief Creates an internal structure that will be used to perform
 * collision detection with a patch mesh. Points are packed as 
 * concatenated rows.
 * 
 * @param width
 * @param height
 * @param first_vert
 * @param points
 * @param origin
 * @return 
*/
struct patch_collide *cm_generate_patch_collide(int width, int height,
    int first_vert, vec3_t *points, vec3_t *origin)
{
    struct patch_collide *pf;
    struct grid grid;
    int i, j;
    
    if (width <= 2 || height <= 2 || !points) {
        com_error(ERR_DROP, "bad parameters: %i, %i, %p",
                  width, height, points);
    }

    if (!(width & 1) || !(height & 1))
        com_error(ERR_DROP, "even sizes are invalid for quadratic meshes");

    if (width > MAX_GRID_SIZE || height > MAX_GRID_SIZE)
        com_error(ERR_DROP, "source is > MAX_GRID_SIZE");

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            vector_copy(points[j * width + i], grid.points[i][j]);
        }
    }

    cm_set_grid_wrap_width(&grid);
    cm_subdivide_grid_columns(&grid, first_vert);
    cm_remove_degenerate_columns(&grid);
    
    cm_transpose_grid(&grid);

    cm_set_grid_wrap_width(&grid);
    cm_subdivide_grid_columns(&grid, first_vert);
    cm_remove_degenerate_columns(&grid);

    pf = hunk_alloc(sizeof(*pf));
    clear_bounds(origin[0], origin[1]);
    for (i = 0; i < grid.width; i++) {
        for (j = 0; j < grid.height; j++)
            add_point_to_bounds(grid.points[i][j], origin[0], origin[1]);
    }

    cm_patch_collide_from_grid(&grid, pf);

    origin[0][0] -= 1.0;
    origin[0][1] -= 1.0;
    origin[0][2] -= 1.0;
    origin[1][0] += 1.0;
    origin[1][1] += 1.0;
    origin[1][2] += 1.0;

    return pf;
}

/**
 * @brief 
 * @param count 
 * @param arr 
 * @param idx1 
 * @param idx2 
 * @return 
*/
int INCOMPLETE cm_terrain_func1(int count, uint16_t *arr, uint16_t idx1, 
                                uint16_t idx2)
{
    int i;

    while (true) {
        if (count <= i)
            return -1;

        if (((arr[i * 2] == idx2) && (arr[i * 2 + 1] == idx1)) ||
            ((arr[i * 2] == idx1 && (arr[i * 2 + 1] == idx2))))
            break;

        i++;
    }

    return i;
}

/**
 * @brief Creates an internal structure that will be used to perform
 * collision detection with a terrain mesh. Points are packed as
 * concatenated rows.
 * 
 * @param num_indexes
 * @param indexes
 * @param num_verts
 * @param points
 * @param origin
 * @return
 * 
 * @todo finish decompiling / rewritting
*/
#define MAX_TERRAIN_VAL 65536
struct terrain_collide * INCOMPLETE cm_generate_terrain_collide(int num_indexes,
    uint16_t *indexes, int num_verts, vec3_t *points, vec3_t *origin)
{
    //int i, j, c, r1;
    //uint16_t idx1, idx2, idx3;
    //uint16_t edges[MAX_TERRAIN_VAL];
    //uint16_t arr1[MAX_TERRAIN_VAL][2];
    //vec3_t v1, v2, v3, v4;
    struct terrain_collide *pf;

    UNUSED(indexes);
    UNUSED(points);
    UNUSED(origin);

    if (num_indexes % 3)
        com_error(ERR_DROP, "num_indexes % 3 != 0, corrupt bsp?");

    if (num_verts >= MAX_TERRAIN_VAL)
        com_error(ERR_DROP, "too many vertices");
    /*
    c = 0;
    for (i = 0; i < num_indexes; i += 3) {
        for (j = 0; j < 3; j++) {
            idx1 = indexes[j + i];
            idx2 = indexes[(j + 1) % 3 + i];
            idx3 = indexes[(j + 2) % 3 + i];

            if (idx1 == idx2)
                com_error(ERR_DROP, "degenerate triangle, corrupt bsp?");

            r1 = cm_terrain_func1(c, *arr1, idx1, idx2);
            if (r1 < 0) {
                if (c >= MAX_TERRAIN_VAL)
                    com_error(ERR_DROP, "too many edges");

                arr1[c][0] = idx1;
                arr1[c][1] = idx2;
                edges[c] = idx3;
                c++;
            } else {
                vector_subtract(points[idx1], points[idx3], v1);
                vector_subtract(points[idx2], points[idx3], v2);
                vector_subtract(points[edges[r1]], points[idx3], v3);
            }
        }
    }*/

    pf = hunk_alloc(sizeof(*pf));
    return pf;
}
