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
 * @file cm_local.h
 * @date 2022-02-04
*/

#ifndef __CM_LOCAL_H__
#define __CM_LOCAL_H__

#include "shared.h"
#include "common.h"
#include "cm_polylib.h"

typedef int cliphandle_t;

#define MAX_SUBMODELS 512
#define	BOX_MODEL_HANDLE		255
#define CAPSULE_MODEL_HANDLE	254

#define MAX_PATCH_INDEXES		0xC000

#define MAX_TERRAIN_VERTS		0x2000
#define MAX_TERRAIN_INDEXES		0xC000

//
// BSP data
//
#define BSP_IDENT   (('P'<<24)+('S'<<16)+('B'<<8)+'I')
#define BSP_VERSION	59

enum {
    LUMP_SHADERS,           // 0
    LUMP_LIGHTMAPS,         // 1
    LUMP_PLANES,            // 2
    LUMP_BRUSH_SIDES,       // 3
    LUMP_BRUSHES,           // 4
    LUMP_FOGS,              // 5
    LUMP_TRIANGLE_SOUPS,    // 6
    LUMP_DRAW_VERTS,        // 7
    LUMP_DRAW_INDEXES,      // 8
    LUMP_CULL_GROUPS,       // 9
    LUMP_CULL_GROUP_INDEXES,// 10
    LUMP_PORTAL_VERTS,      // 11
    LUMP_OCCLUDERS,         // 12
    LUMP_OCCLUDER_PLANES,   // 13
    LUMP_OCCLUDER_EDGES,    // 14
    LUMP_OCCLUDER_INDEXES,  // 15
    LUMP_AABB_TREES,        // 16
    LUMP_CELLS,             // 17
    LUMP_PORTALS,           // 18
    LUMP_LIGHT_INDEXES,     // 19
    LUMP_NODES,             // 20
    LUMP_LEAFS,             // 21
    LUMP_LEAF_BRUSHES,      // 22
    LUMP_LEAF_SURFACES,     // 23
    LUMP_PATCH_COLLISION,  	// 24
    LUMP_COLLISION_VERTS,   // 25
    LUMP_COLLISION_INDEXES, // 26
    LUMP_MODELS,            // 27
    LUMP_VISIBILITY,        // 28
    LUMP_ENTITIES,          // 29
    LUMP_LIGHTS,            // 30
    LUMP_FLARE31,           // 31
    LUMP_FLARE32,           // 32
    HEADER_LUMPS            // 33
};

struct header_lump {
    int file_len;
    int file_ofs;
};

struct bsp_header {
    int ident;
    int version;

    struct header_lump lumps[HEADER_LUMPS];
};

/*
 * BSP data
 * 
 * all regularly named structures are internal structures used by the game
 * to actually do all the work. the structures with bsp_ (e.g. bsp_plane)
 * are simplified structures that are written out to the bsp when it is 
 * compiled, and those are used to rebuild all the real internal data
 * the game needs to do its stuff
 */


//
// 00 shader lump
// internal <-> bsp are the same
//
struct shader {
    char texture[64];
    int surface_flags;
    int contents;
};

//
// 01 lightmaps
//

//
// 02 planes
//
struct plane {
    vec3_t normal;
    float dist;
    byte type;
    byte sign_bits;
    byte pad[2];
};

struct bsp_plane {
    vec3_t normal;
    float dist;
};

//
// 03 brush sides
//
struct bsp_brush_side {
    int plane_num;
    int shader_num;
};

struct brush_side {
    struct plane *plane;
    int shader_num;
};

//
// 04 brushes
//
struct bsp_brush {
    int16_t num_sides;
    int16_t shader_num;
};

struct brush {
    int contents;
    vec3_t bounds[2];
    int num_sides;
    int shader_num;
    int a;
    int16_t sides[6];
};

//
// 05 fogs (unused)
//
struct bsp_fogs {
    char data[72];
};

//
// 20 nodes
//
struct bsp_node {
    int plane_num;
    int children[2];
    vec3_t mins;
    vec3_t maxs;
};

struct node {
    struct plane *plane;
    int children[2];
};

//
// 21 leafs
//
struct bsp_leaf {
    int cluster;
    int area;
    int first_surface;
    int num_surfaces;
    int first_brush;
    int num_brushes;
    int cell_num;

    int unk[2];
};

struct leaf {
    int16_t cluster;
    int16_t area;
    int first_brush;
    uint16_t num_brushes;
    uint16_t first_surface;
    uint16_t num_surfaces;
    int16_t cell_num;
};

//
// 22 leaf brushes
//
struct leaf_brush {
    int brush;
};

//
// 23 leaf surfaces
//
struct leaf_surface {
    int surface;
};

//
// 24 patch collision
//
struct bsp_surface {
    uint16_t shader_num;
    uint16_t surface_type;
    int16_t patch_width;
    int16_t patch_height;
    int first_vert;
    int num_verts;
};

struct bsp_vert {
    vec3_t xyz;
};

//
// 25 collision verts
//

//
// 26 collision indexes
//

//
// 27 models
//
struct bsp_model {
    vec3_t mins;
    vec3_t maxs;
    int unk[2];
    int first_surface;
    int num_surfaces;
    int first_brush;
    int num_brushes;
};

struct model {
    vec3_t mins;
    vec3_t maxs;
    struct leaf *leaf;
    intptr_t first_brush; // changed from int
    uint16_t num_brushes;
    uint16_t first_surface;
    uint16_t num_surfaces;
    uint16_t a;
};

//
//
//
struct area {
    int flood_num;
    int flood_valid;
};

//
//
//
struct clipmap {
    char name[64];

    int num_static_models;
    int *static_models;

    int num_shaders;
    struct shader *shaders;

    int num_brush_sides;
    struct brush_side *brush_sides;

    int num_planes;
    struct plane *planes;

    int num_nodes;
    struct node *nodes;

    int num_leafs;
    struct leaf *leafs;

    int num_leaf_brushes;
    int *leaf_brushes;

    int num_leaf_surfaces;
    int *leaf_surfaces;

    int num_submodels;
    struct model *submodels;

    int num_brushes;
    struct brush *brushes;

    int num_clusters;
    int cluster_bytes;
    byte *visibility;
    bool vised;

    int num_entity_chars;
    char *entity_string;

    int num_areas;
    struct area *areas;
    int *area_portals;

    int num_surfaces;
    struct patch *surfaces;

    int flood_valid;

    // data

    // data

    struct brush *box_brush;
    struct model box_model;
};

extern struct clipmap cm;

extern struct cvar *cm_nocurves;
extern struct cvar *cm_playercurveclip;

//
//
// cm_load.c
//
//
void cm_init_box_hull();

//
//
// cm_test.c
//
//
void cm_flood_area_connections();


#endif // __CM_LOCAL_H__
