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
 * @file script.h
 * @date 2022-02-04
*/

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#define MAXSTACK 65536

enum variable_type {
	VT_UNDEFINED,
	VT_STRING,
	VT_LOCALIZED_STRING,
	VT_VECTOR,
	VT_FLOAT,
	VT_INT,
	VT_CODEPOS,
	VT_OBJECT,
	VT_KEY_VALUE,
	VT_FUNCTION,
	VT_STACK,
	VT_ANIMATION,
	VT_THREAD,
	VT_ENTITY,
	VT_STRUCT,
	VT_ARRAY,
	VT_DEAD_THREAD,
	VT_DEAD_ENTITY,
	VT_DEAD_OBJECT
};

enum hud_element_type {
	HUD_NONE,               // 0
	HUD_TEXT,               // 1
	HUD_FLOAT,              // 2
	HUD_SHADER,             // 3
	HUD_TIMER,              // 4
	HUD_TIMERUP,            // 5
	HUD_TENTHSTIMER,        // 6
	HUD_TENTHSTIMERUP,      // 7
	HUD_CLOCK,              // 8
	HUD_CLOCKUP,            // 9
	HUD_END
};

struct hud_element {
	int type;                   // 00   0
	int x;                      // 01   4
	int y;                      // 02   8
	float fontscale;            // 03   12
	int font;                   // 04   16
	int alignX;                 // 05   20
	int alignY;                 // 06   24
	byte color[4];
	byte oldColor[4];
	int fadeOverTimeStart;      // 09   36
	int fadeOverTimeLength;     // 10   40
	int label;                  // 11   44
	int shaderX;                // 12   48
	int shaderY;                // 13   52
	int shader;                 // 14   56
	int oldShaderX;             // 15   60
	int oldShaderY;             // 16   64
	int scaleOverTimeStart;     // 17   68
	int scaleOverTimeLength;    // 18   72
	int oldStartX;              // 19   76
	int oldStartY;              // 20   80
	int moveOverTimeStart;      // 21   84
	int moveOverTimeLength;     // 22   88
	int clockStart;             // 23   92
	int clockDivision;          // 24   96
	float value;                // 25   100
	int string;                 // 26   104
	float sort;                 // 27   108
	int entityNumber;           // 28   112
	int team;                   // 29   116
	bool archived;				// 30   120
};

extern struct hud_element g_hud_elements[];

typedef void (*gsc_function_ptr)(int);
struct gsc_function {
	const char *name;
	gsc_function_ptr ptr;
	int developer;
};

typedef void (*gsc_method_ptr)(int);
struct gsc_method {
	const char *name;
	gsc_method_ptr ptr;
};

//
// function declarations
//

// this is required to allow the function declarations to exist properly
// but i still prefer to use 
typedef struct gentity gentity_t;

#define UNUSED
#include "global/script_declarations.h"

intptr_t scr_far_hook(intptr_t addr);

#endif // __SCRIPT_H__
