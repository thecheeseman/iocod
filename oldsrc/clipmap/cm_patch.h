#ifndef CLIPMAP_CM_PATCH_H
#define CLIPMAP_CM_PATCH_H

#include <stdint.h>
#include "types/bool.h"
#include "types/vector.h"

#define MAX_PATCH_PLANES 0x1000
#define MAX_FACETS 0x400

#define	SUBDIVIDE_DISTANCE 4 // never more than this units away from curve
#define	PLANE_TRI_EPSILON 0.1
#define	WRAP_POINT_EPSILON 0.1

struct patch_plane {
    vec4_t plane;
    int sign_bits;
};

struct facet {
    int surface_plane;
    int num_borders;

    int border_planes[4 + 6 + 16];
    int border_inward[4 + 6 + 16];
    int border_noadjust[4 + 6 + 16];
};

struct patch_collide {
    int num_planes;
    struct patch_plane *planes;
    int num_facets;
    struct facet *facets;
};

struct ter2 {
    int field_0x00;
    int field_0x04;
    int field_0x08;
    int field_0x0c;

    vec3_t field_0x10;
    vec3_t field_0x1c;
    vec3_t field_0x28;
    float field_0x34;
};

struct terrain_collide {
    int16_t num_indexes;
    int16_t field_0x02;

    float field_0x04;
    float field_0x08;
    float field_0x0c;

    int field_0x10;

    float field_0x14;
    float field_0x18;
    float field_0x1c;
    float field_0x20;
    float field_0x24;
    float field_0x28;
    float field_0x2c;
    float field_0x30;

    int field_0x34;
    int field_0x38;
    int field_0x3c;

    struct ter2 *field_0x40;
    struct ter2 *field_0x44;
};

struct patch {
    int surface_flags;
    int shader_num;
    int contents;

    vec3_t origin[2];

    struct patch_collide *patch;
    struct terrain_collide *terrain;
};

#define MAX_GRID_SIZE 129
struct grid {
    int width;
    int height;
    bool wrap_width;
    bool wrap_height;
    vec3_t points[MAX_GRID_SIZE][MAX_GRID_SIZE];
};

struct patch_collide *cm_generate_patch_collide(int width, int height,
    int first_vert, vec3_t *points, vec3_t *origin);
struct terrain_collide *cm_generate_terrain_collide(int num_indexes,
    uint16_t *indexes, int num_verts, vec3_t *points, vec3_t *origin);

#endif /* CLIPMAP_CM_PATCH_H */
