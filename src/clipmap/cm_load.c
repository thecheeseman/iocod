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
 * @file cm_load.c
 * @date 2022-02-04
*/

#include "cm_local.h"

/**
 * @brief local map structure
*/
struct clipmap cm;

/**
 * @brief pointer to the base of map data
*/
byte *cmod_base;

struct cvar *cm_nocurves;
struct cvar *cm_playercurveclip;

/**
 * @brief Load shader data from BSP 
 * 
 * @param l shader header lump
*/
void cmod_load_shaders(struct header_lump *l)
{
	struct shader *in;
	int count;

	in = (struct shader *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");

	count = l->file_len / sizeof(*in);
	if (count < 1)
		com_error(ERR_DROP, "map with no shaders");

	cm.shaders = hunk_alloc(count * sizeof(*cm.shaders));
	cm.num_shaders = count;

	memcpy(cm.shaders, in, count * sizeof(*cm.shaders));
}

/**
 * @brief Load plane data from BSP 
 * 
 * @param l planes header lump
*/
void cmod_load_planes(struct header_lump *l)
{
	struct bsp_plane *in;
	struct plane *out;
	int i, j, count, bits;

	in = (struct bsp_plane *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");

	count = l->file_len / sizeof(*in);
	if (count < 1)
		com_error(ERR_DROP, "map with no planes");

	cm.planes = hunk_alloc(count * sizeof(*cm.planes));
	cm.num_planes = count;

	out = cm.planes;
	for (i = 0; i < count; i++, in++, out++) {
		bits = 0;

		for (j = 0; j < 3; j++) {
			out->normal[j] = in->normal[j];
			if (out->normal[j] < 0.0)
				bits |= 1 << j;
		}

		out->dist = in->dist;
		out->type = plane_type_for_normal(out->normal);
		out->sign_bits = bits;
	}
}

/**
 * @brief Load brush data from BSP 
 * 
 * @param l1 brushes header lump
 * @param l2 brush sides header lump
 * 
 * @todo figure out why this ends up with a different brush_side count than q3map
*/
void cmod_load_brushes(struct header_lump *l1, struct header_lump *l2)
{
	struct bsp_brush *brush_in;
	struct brush *brush_out;
	struct bsp_brush_side *brush_side_in;
	struct brush_side *brush_side_out;
	int i, j, k, num, brush_count, side_count;

	brush_in = (struct bsp_brush *) (cmod_base + l1->file_ofs);
	if (l1->file_len % sizeof(*brush_in))
		com_error(ERR_DROP, "funny lump size");
	brush_count = l1->file_len / sizeof(*brush_in);

	brush_side_in = (struct bsp_brush_side *) (cmod_base + l2->file_ofs);
	if (l2->file_len % sizeof(*brush_side_in))
		com_error(ERR_DROP, "funny lump size");

	side_count = (l2->file_len / sizeof(*brush_side_in)) + (brush_count * -6);
	if (side_count < 0)
		com_error(ERR_DROP, "bad side count");

	// the side_count calculation above gives odd results
	// for example, q3map reports mp_harbor as having
	//	3466 brushes
	// 24771 brushsides
	// 
	// but the above code reports
	//  3466 brushes
	//  3975 brushsides
	// the correct value is reached without the `+ (brush_count * -6)` part 
	// but that's in the actual executable ??
	// maybe it doesn't matter or it's a bug?
	/*
	com_printf("side_count = (%d / %d) + (%d * -6)\n",
			   l2->file_len, sizeof(*brush_side_in), brush_count);
	com_printf("  (%d / %d) = %d\n", l2->file_len, sizeof(*brush_side_in),
			   (l2->file_len / sizeof(*brush_side_in)));
	com_printf("  (%d * -6) = %d\n", brush_count, (brush_count * -6));
	com_printf("  %d + %d = %d\n", (l2->file_len / sizeof(*brush_side_in)),
			   (brush_count * -6), side_count);
	*/

	if (side_count > 0)
		cm.brush_sides = hunk_alloc(side_count * sizeof(*cm.brush_sides));
	else
		cm.brush_sides = NULL;
	cm.num_brush_sides = side_count;

	cm.brushes = hunk_alloc((brush_count + 1) * sizeof(*cm.brushes));
	cm.num_brushes = brush_count;

	brush_out = cm.brushes;
	brush_side_out = cm.brush_sides;
	for (i = 0; i < brush_count; i++, brush_in++, brush_out++) {
		brush_out->num_sides = brush_in->num_sides - 6;

		if (brush_out->num_sides < 0)
			com_error(ERR_DROP, "brush has less than 6 sides");

		// something with shadernum

		for (j = 0; j < 3; j++) {
			for (k = 0; k < 2; k++, brush_side_in++) {
				brush_out->bounds[k][j] = brush_side_in->plane_num;

				num = brush_side_in->shader_num;
				if (num < 0 ||cm.num_shaders <= num)
					com_error(ERR_DROP, "bad shadernum: %i",num);

				brush_out->sides[k * 3 + j] = (short) num;
				if (brush_out->sides[k * 3 + j] != num)
					com_error(ERR_DROP, "axialshadernum exceeded");
			}
		}

		for (j = 0; j < brush_out->num_sides; j++, brush_side_in++,
			 brush_side_out++) {
			brush_side_out->plane = cm.planes + brush_side_in->plane_num;

			num = brush_side_in->shader_num;
			brush_side_out->shader_num = num;
			if (num < 0 || cm.num_shaders <= num)
				com_error(ERR_DROP, "bad shadernum: %i", num);
		}

		num = brush_in->shader_num;
		brush_out->shader_num = num;
		if (num < 0 || cm.num_shaders <= num)
			com_error(ERR_DROP, "bad shadernum: %i", num);

		brush_out->contents = cm.shaders[num].contents;
	}
}

/**
 * @brief Load node data from BSP 
 * 
 * @param l nodes header lump
*/
void cmod_load_nodes(struct header_lump *l)
{
	struct bsp_node *in;
	struct node *out;
	int i, j, count;

	in = (struct bsp_node *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");

	count = l->file_len / sizeof(*in);
	if (count < 1)
		com_error(ERR_DROP, "map has no nodes");

	cm.nodes = hunk_alloc(count * sizeof(*cm.nodes));
	cm.num_nodes = count;

	out = cm.nodes;
	for (i = 0; i < count; i++, in++, out++) {
		out->plane = cm.planes + in->plane_num;

		for (j = 0; j < 2; j++) {
			out->children[j] = (int16_t) in->children[j];
			if (out->children[j] != in->children[j])
				com_error(ERR_DROP, "children exceeded");
		}
	}
}

/**
 * @brief Load leaf data from BSP 
 * 
 * @param l leafs header lump
*/
void cmod_load_leafs(struct header_lump *l)
{
	struct bsp_leaf *in;
	struct leaf *out;
	int i, count;

	in = (struct bsp_leaf *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");
	count = l->file_len / sizeof(*in);

	if (count < 1)
		com_error(ERR_DROP, "map with no leafs");

	cm.leafs = hunk_alloc(count * sizeof(*cm.leafs));
	cm.num_leafs = count;

	out = cm.leafs;
	for (i = 0; i < count; i++, in++, out++) {
		out->cell_num = (int16_t) in->cell_num;
		if (out->cell_num != in->cell_num)
			com_error(ERR_DROP, "cellnum exceeded");

		out->cluster = (int16_t) in->cluster;
		if (out->cluster != in->cluster)
			com_error(ERR_DROP, "cluster exceeded");

		out->area = (int16_t) in->area;
		if (out->area != in->area)
			com_error(ERR_DROP, "area exceeded");

		out->first_brush = in->first_brush;

		out->num_brushes = (int16_t) in->num_brushes;
		if (out->num_brushes != in->num_brushes)
			com_error(ERR_DROP, "numleafbrushes exceeded");

		out->first_surface = (int16_t) in->first_surface;
		if (out->first_surface != in->first_surface)
			com_error(ERR_DROP, "firstleafsurface exceeded");

		out->num_surfaces = (int16_t) in->num_surfaces;
		if (out->num_surfaces != in->num_surfaces)
			com_error(ERR_DROP, "numleafsurfaces exceeded");

		if (cm.num_clusters <= in->cluster)
			cm.num_clusters = in->cluster + 1;
		if (cm.num_areas <= in->area)
			cm.num_areas = in->area + 1;
	}

	cm.areas = hunk_alloc(cm.num_areas * sizeof(*cm.areas));
	cm.area_portals = hunk_alloc(cm.num_areas * cm.num_areas * sizeof(*cm.areas));
}

/**
 * @brief Load leaf brush data from BSP 
 * 
 * @param l leaf brushes header lump
*/
void cmod_load_leaf_brushes(struct header_lump *l)
{
	//struct leaf_brush *in, *out;
	int *in, *out;
	int i, count;

	//in = (struct leaf_brush *) (cmod_base + l->file_ofs);
	in = (int *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");
	count = l->file_len / sizeof(*in);

	cm.leaf_brushes = hunk_alloc(count * sizeof(*cm.leaf_brushes));
	cm.num_leaf_brushes = count;

	out = cm.leaf_brushes;
	for (i = 0; i < count; i++, in++, out++)
		//out->brush = in->brush;
		*out = *in;
}

/**
 * @brief Load leaf surface data from BSP 
 * 
 * @param l leaf surfaces header lump
*/
void cmod_load_leaf_surfaces(struct header_lump *l)
{
	//struct leaf_surface *in, *out;
	int *in, *out;
	int i, count;

	//in = (struct leaf_surface *) (cmod_base + l->file_ofs);
	in = (int *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");
	count = l->file_len / sizeof(*in);

	cm.leaf_surfaces = hunk_alloc(count * sizeof(*cm.leaf_surfaces));
	cm.num_leaf_surfaces = count;

	out = cm.leaf_surfaces;
	for (i = 0; i < count; i++, in++, out++)
		//out->surface = in->surface;
		*out = *in;
}

/**
 * @brief Sign extend a given 24 bit value -> 32 bit
 * 
 * @param x
 * @return 
 * 
 * @note not sure this is needed in @see cmod_load_leaf_patches
*/
uint32_t sign_extend_24_32(uint32_t x)
{
	const int bits = 24;
	uint32_t m = 1u << (bits - 1);
	return (x ^ m) - m;
}

#define MAX_PATCH_VERTS 0x2000

/**
 * @brief Load leaf patches and collision data from BSP 
 * 
 * @param l1 patch collision header lump
 * @param l2 collision verts header lump
 * @param l3 collision indexes header lump
*/
void cmod_load_leaf_patches(struct header_lump *l1, struct header_lump *l2,
							struct header_lump *l3)
{
	struct bsp_surface *in;
	struct bsp_vert *vert_in, *vp;
	struct patch *out;
	uint16_t local_indexes[MAX_PATCH_INDEXES];
	uint16_t *index_in, *p_index;
	vec3_t points[1024];
	int count, i, j, c, verts, indexes;

	in = (struct bsp_surface *) (cmod_base + l1->file_ofs);
	if (l1->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");

	count = l1->file_len / sizeof(*in);

	vert_in = (struct bsp_vert *) (cmod_base + l2->file_ofs);
	if (l2->file_len % sizeof(*vert_in))
		com_error(ERR_DROP, "funny lump size");

	index_in = (uint16_t *)(cmod_base + l3->file_ofs);
	if (l3->file_len % sizeof(*index_in))
		com_error(ERR_DROP, "funny lump size");

	com_printf("index_in @ %d len %d\n", l3->file_ofs, l3->file_len);

	cm.num_surfaces = count;
	cm.surfaces = hunk_alloc(cm.num_surfaces * sizeof(*cm.surfaces));

	out = cm.surfaces;
	for (i = 0; i < count; i++, in++, out++) {
		out->surface_flags = 0;
		out->shader_num = in->shader_num;
		out->contents = cm.shaders[in->shader_num].contents;

		if (*(char *) &in->surface_type == '\0') {
			c = in->patch_width * in->patch_height;
			if (c > MAX_PATCH_VERTS) {
				com_error(ERR_DROP, "more than %i verts in curve",
						  MAX_PATCH_VERTS);
			}

			vp = (vert_in + in->num_verts);
			for (j = 0; j < c; j++) {
				vector_copy(vp->xyz, points[j]);
			}

			out->patch = cm_generate_patch_collide(in->patch_width,
												   in->patch_height,
												   in->first_vert,
												   points,
												   out->origin);
			out->terrain = NULL;
		} else {
			verts = sign_extend_24_32(in->patch_width);
			if (verts > MAX_TERRAIN_VERTS) {
				com_error(ERR_DROP, "more than %i verts in terrain",
						  MAX_TERRAIN_VERTS);
			}

			vp = (vert_in + in->first_vert);
			for (j = 0; j < verts; j++) {
				vector_copy(vp->xyz, points[j]);
			}

			indexes = sign_extend_24_32(in->patch_height);
			if (indexes > MAX_TERRAIN_INDEXES) {
				com_error(ERR_DROP, "more than %i indexes in terrain",
						  MAX_TERRAIN_INDEXES);
			}

			p_index = (in->num_verts + index_in);
			for (j = 0; j < indexes; j++)
				local_indexes[j] = *p_index;

			out->patch = NULL;
			out->terrain = cm_generate_terrain_collide(indexes,
													   local_indexes,
													   verts,
													   points,
													   out->origin);
		}
	}
}

/**
 * @brief Load submodels (brushmodels) from BSP data 
 * 
 * @param l submodel header lump
*/
void cmod_load_submodels(struct header_lump *l)
{
	struct bsp_model *in;
	struct model *out;
	int i, j, count, *indexes;

	in = (struct bsp_model *) (cmod_base + l->file_ofs);
	if (l->file_len % sizeof(*in))
		com_error(ERR_DROP, "funny lump size");

	count = l->file_len / sizeof(*in);
	if (count < 1)
		com_error(ERR_DROP, "map with no models");

	cm.submodels = hunk_alloc(count * sizeof(*cm.submodels));
	cm.num_submodels = count;

	if (count > MAX_SUBMODELS)
		com_error(ERR_DROP, "MAX_SUBMODELS exceeded");

	out = cm.submodels;
	for (i = 0; i < count; i++, in++, out++) {
		for (j = 0; j < 3; j++) {
			out->mins[j] = in->mins[j] - 1;
			out->maxs[j] = in->maxs[j] + 1;
		}

		// skip worldspawn
		if (i == 0)
			continue;

		out->num_brushes = (uint16_t) in->num_brushes;
		if (out->num_brushes != in->num_brushes)
			com_error(ERR_DROP, "numleafbrushes exceeded");

		indexes = hunk_alloc(in->num_brushes * sizeof(*indexes));
		out->first_brush = ((intptr_t) indexes - (intptr_t) cm.leaf_brushes) >> 2;
		for (j = 0; j < in->num_brushes; j++)
			indexes[j] = in->first_brush + j;

		out->num_surfaces = (uint16_t) in->num_surfaces;
		if (out->num_surfaces != in->num_surfaces)
			com_error(ERR_DROP, "numleafsurfaces exceeded");

		out->first_surface = (uint16_t) in->first_surface;
		if (out->first_surface != in->first_surface)
			com_error(ERR_DROP, "firstleafsurface exceeded");
	}
}

/**
 * @brief Load visibility data from BSP 
 * 
 * @param l visibility header lump
*/
void cmod_load_visibility(struct header_lump *l)
{
	byte *buf;

	if (l->file_len == 0) {
		cm.cluster_bytes = (cm.num_clusters + 31) & ~31; // 32 bit magic again
		cm.visibility = (byte *) hunk_alloc(cm.cluster_bytes);
		memset(cm.visibility, 255, cm.cluster_bytes);
		return;
	}

	buf = (cmod_base + l->file_ofs);

	cm.vised = true;
	cm.visibility = (byte *) hunk_alloc(l->file_len);
	cm.num_clusters = ((intptr_t *) buf)[0];
	cm.cluster_bytes = ((intptr_t *) buf)[1];
	memcpy(cm.visibility, buf + 2, l->file_len);
}

/**
 * @brief Load entity data from BSP 
 * 
 * @param l entity string header lump
*/
void cmod_load_entity_string(struct header_lump *l)
{
	cm.entity_string = hunk_alloc(l->file_len);
	cm.num_entity_chars = l->file_len;
	memcpy(cm.entity_string, (cmod_base + l->file_ofs), l->file_len);
}

/**
 * @brief Print info matching `q3map -info` style 
 * 
 * @param mapname 
 * @param length 
 * @param header 
*/
void print_bsp_info(const char *mapname, int length, struct bsp_header *header)
{
	com_printf("%s: %d\n", mapname, length);

	#define INFO(c, s, l) \
		com_printf("%7i %-20s %8i B %8i KB %4.1f%%\n", \
				   c, s, header->lumps[l].file_len, \
				   header->lumps[l].file_len / 1024, \
				   ((float) header->lumps[l].file_len / (float) length) * 100.0);

	INFO(cm.num_submodels, "models", LUMP_MODELS);
	INFO(cm.num_shaders, "shaders", LUMP_SHADERS);
	INFO(cm.num_brushes, "brushes", LUMP_BRUSHES);
	INFO(cm.num_brush_sides, "brushsides", LUMP_BRUSH_SIDES);
	INFO(0, "fogs", LUMP_FOGS);
	INFO(cm.num_planes, "planes", LUMP_PLANES);
	INFO(cm.num_entity_chars, "entdata", LUMP_ENTITIES);
	INFO(cm.num_nodes, "nodes", LUMP_NODES);
	INFO(cm.num_leafs, "leafs", LUMP_LEAFS);
	INFO(cm.num_leaf_brushes, "leafbrushes", LUMP_LEAF_BRUSHES);
	INFO(cm.num_leaf_surfaces, "leafsurfaces", LUMP_LEAF_SURFACES);
	INFO(cm.num_surfaces, "patchcollision", LUMP_PATCH_COLLISION);
	INFO(0, "collisionverts", LUMP_COLLISION_VERTS);
	INFO(0, "collisionindexes", LUMP_COLLISION_INDEXES);
	INFO(0, "drawverts", LUMP_DRAW_VERTS);
	INFO(0, "drawindexes", LUMP_DRAW_INDEXES);
	INFO(0, "trianglesoups", LUMP_TRIANGLE_SOUPS);
	INFO(0, "lightmaps", LUMP_LIGHTMAPS);
	INFO(0, "lights", LUMP_LIGHTS);
	INFO(0, "lightindexes", LUMP_LIGHT_INDEXES);
	INFO(0, "visibility", LUMP_VISIBILITY);
	INFO(0, "portalverts", LUMP_PORTAL_VERTS);
	INFO(0, "occluders", LUMP_OCCLUDERS);
	INFO(0, "occluderplanes", LUMP_OCCLUDER_PLANES);
	INFO(0, "occluderedges", LUMP_OCCLUDER_EDGES);
	INFO(0, "occluderindexes", LUMP_OCCLUDER_INDEXES);
	INFO(0, "aabbtrees", LUMP_AABB_TREES);
	INFO(0, "cells", LUMP_CELLS);
	INFO(0, "portals", LUMP_PORTALS);
	INFO(0, "cullgroups", LUMP_CULL_GROUPS);
	INFO(0, "cullgroupindexes", LUMP_CULL_GROUPS);

	#undef INFO
}

/**
 * @brief Load a BSP
 * @param name name of bsp file
 * @param clientload 
 * @param checksum
*/
void INCOMPLETE cm_load_map(const char *name, bool clientload, int *checksum)
{
	char mapname[64];
	static unsigned last_checksum;
	int length;
	intptr_t *buf;

	struct bsp_header header;

	if (name == NULL || *name == '\0')
		com_error(ERR_DROP, "null name");

	q_strncpyz(mapname, name, sizeof(mapname));

	cm_nocurves = cvar_get("cm_nocurves", "0", CVAR_CHEAT);
	cm_playercurveclip = cvar_get("cm_playercurveclip", "1", 
								  CVAR_ARCHIVE | CVAR_CHEAT);

	com_printf_dbginfo("%s : %i\n", name, clientload);

	if (!clientload || com_sv_running->integer == 0) {
		memset(&cm, 0, sizeof(cm));

		length = fs_read_file(name, (void **) &buf);
		if (buf == NULL)
			com_error(ERR_DROP, "couldn't load %s", name);

		last_checksum = com_block_checksum(buf, length);
		*checksum = last_checksum;

		header = *(struct bsp_header *) buf;

		// q3 has a complicated littlelong() conversion here
		// but we really don't need that

		if (header.version != BSP_VERSION)
			com_error(ERR_DROP, "%s is version %i, not %i", name, header.version,
					  BSP_VERSION);

		cmod_base = (byte *) buf;

		// load into heap
		cmod_load_shaders(&header.lumps[LUMP_SHADERS]);
		cmod_load_planes(&header.lumps[LUMP_PLANES]);
		cmod_load_brushes(&header.lumps[LUMP_BRUSHES],
						  &header.lumps[LUMP_BRUSH_SIDES]);
		cmod_load_nodes(&header.lumps[LUMP_NODES]);
		cmod_load_leafs(&header.lumps[LUMP_LEAFS]);
		cmod_load_leaf_brushes(&header.lumps[LUMP_LEAF_BRUSHES]);
		cmod_load_leaf_surfaces(&header.lumps[LUMP_LEAF_SURFACES]);
		cmod_load_leaf_patches(&header.lumps[LUMP_PATCH_COLLISION],
							   &header.lumps[LUMP_COLLISION_VERTS],
							   &header.lumps[LUMP_COLLISION_INDEXES]);
		cmod_load_submodels(&header.lumps[LUMP_MODELS]);
		cmod_load_visibility(&header.lumps[LUMP_VISIBILITY]);
		cmod_load_entity_string(&header.lumps[LUMP_ENTITIES]);
		// load

		fs_free_file(buf);

		print_bsp_info(mapname, length, &header);

		cm_init_box_hull();
		cm_flood_area_connections();

		// fun_08058dd0(); // some crazy stuff
		// fun_080515d4(); // static model loading

		if (!clientload)
			q_strncpyz(cm.name, name, sizeof(cm.name));
	} else {
		*checksum = last_checksum;
	}
}

/**
 * @brief Set up the planes and nodes so that the six floats of a bounding box
 * can just be stored out and get a proper clipping hull structure.
*/
void cm_init_box_hull()
{
	int i;

	cm.box_brush = cm.brushes + cm.num_brushes;

	cm.box_brush->num_sides = 0;
	cm.box_brush->shader_num = 0;
	cm.box_brush->contents = -1;

	for (i = 0; i < 6; i++)
		cm.box_brush->sides[i] = -1;

	cm.box_model.num_brushes = 1;
	cm.box_model.first_brush = cm.num_leaf_brushes;
	cm.leaf_brushes[cm.num_leaf_brushes] = cm.num_brushes;
}

/**
 * @brief 
 * @param handle
 * @return
*/
struct model *cm_clip_handle_to_model(cliphandle_t handle)
{
	if (handle < 0)
		com_error(ERR_DROP, "bad handle %i", handle);

	if (handle < cm.num_submodels)
		return &cm.submodels[handle];

	if (handle == BOX_MODEL_HANDLE || handle == CAPSULE_MODEL_HANDLE)
		return &cm.box_model;

	if (handle < MAX_SUBMODELS) {
		com_error(ERR_DROP, "bad handle %i < %i < %i", cm.num_submodels,
				  handle, MAX_SUBMODELS);
	}

	com_error(ERR_DROP, "bad handle %d", handle + MAX_SUBMODELS);

	return NULL;
}

/**
 * @brief 
 * @param index 
 * @return 
*/
cliphandle_t cm_inline_model(int index)
{
	if (index < 0 || index >= cm.num_submodels)
		com_error(ERR_DROP, "bad number");

	return index;
}

/**
 * @brief 
 * @param model 
 * @param mins 
 * @param maxs 
*/
void cm_model_bounds(cliphandle_t model, vec3_t mins, vec3_t maxs)
{
	struct model *m;

	m = cm_clip_handle_to_model(model);
	vector_copy(m->mins, mins);
	vector_copy(m->maxs, maxs);
}
