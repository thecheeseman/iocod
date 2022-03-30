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
 * @file cm_test.c
 * @date 2022-02-04
*/

#include "common/error.h"
#include "cm_local.h"

/**
 * @brief 
 * @param area_num 
 * @param flood_num 
*/
void cm_flood_area_r(int area_num, int flood_num)
{
    int i, *con;
    struct area *area;

    area = &cm.areas[area_num];

    if (area->flood_valid == cm.flood_valid) {
        if (area->flood_num == flood_num)
            return;

        com_error(ERR_DROP, "reflooded");
    }

    area->flood_num = flood_num;
    area->flood_valid = cm.flood_valid;
    con = cm.area_portals + area_num * cm.num_areas;
    for (i = 0; i < cm.num_areas; i++) {
        if (con[i] > 0)
            cm_flood_area_r(i, flood_num);
    }
}

/**
 * @brief 
 * @param  
*/
void cm_flood_area_connections(void)
{
    int i, flood_num;
    struct area *area;

    cm.flood_valid++;
    flood_num = 0;

    for (i = 0; i < cm.num_areas; i++) {
        area = &cm.areas[i];
        if (area->flood_valid == cm.flood_valid)
            continue; // already flooded into

        flood_num++;
        cm_flood_area_r(i, flood_num);
    }
}
