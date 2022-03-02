iocod
=====

Table of Contents
1. [Introduction](#introduction)
2. [Sources](#sources)
3. [Requirements](#requirements)
4. [Compilation](#compilation)
5. [Installation](#installation)
6. [License](#license)
7. [Excluded Code](#excluded-code)

Introduction
------------

iocod is an open-source project aimed at providing a drop-in replacement 
server binary and game library for the 2003 Infinity Ward title, Call of Duty, 
as distributed by Activison. It is designed as an easy, no-nonsense 
replacement for the original server binaries in order to provide better 
interoperability amongst the communities on various game patches (1.1 & 1.5), 
to provide a modern executable that supports 32-bit and 64-bit installations, 
as well as offering fixes for a variety of bugs and exploits which remain
unpatched in the original binaries. It is entirely compatible with the original
assets as owned and provided to players by Infinity Ward/Activision through
the official product media.

This material is not made or supported in any way by Infinity Ward, 
Activision, or Microsoft.

Sources
-------

This project makes use of [id-software][8]'s [Quake 3][1] & [RTCW][2] GPL 
sources, as well as the excellent work done on [CoDExtended][3] by 
[riicchhaarrd][4], [dftd][5], and the rest of the xtnded team. Inevitably, 
as the xtnded project was originally influenced and inspired by 
[kungfooman][7]'s [libcod][6] project, that project has been included here 
for posterity.

id-software
- [Quake III Team Arena GPL Source][1]
- [RTCW MP GPL Source][2]

xtnded team
- [CoDExtended][3]

kungfooman
- [libcod][6]

[1]: <https://github.com/id-Software/Quake-III-Arena>
[2]: <https://github.com/id-Software/RTCW-MP>
[3]: <https://github.com/xtnded/codextended>
[4]: <https://github.com/riicchhaarrd>
[5]: <https://github.com/dftd>
[6]: <https://github.com/kungfooman/libcod>
[7]: <https://github.com/kungfooman>
[8]: <https://github.com/id-software>

Requirements
------------
- `cmake` version 3.13 or above
- compatible C compiler (`gcc`, `clang` or `msvc`)

If compiling for 32-bit, please specify the build flag `-DFORCE_32_BIT`
to ensure a proper 32-bit binary is created.

Compiliation
------------

Installation
------------

License
-------

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

For more information or to read the full license agreement, see 
[LICENSE](LICENSE).

Excluded Code
-------------

The code described below and contained in this release is not 
part of the program covered by the GPL and is expressly excluded from its 
terms. You are solely responsible for obtaining from the copyright holder a 
license for such code and complying with the applicable license terms.

#### Unzip License
	IO on .zip files using portions of zlib
	src/external/unzip.c
	Copyright (C) 1998 Gilles Vollant
	zlib is Copyright (C) 1995-1998 Jean-loup Gailly and Mark Adler

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgment in the product documentation would be
		appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.

#### MD4 License
	MD4 Message-Digest Algorithm
	src/external/md4.c
	Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All rights reserved.

	License to copy and use this software is granted provided that it is identified
	as the <93>RSA Data Security, Inc. MD4 Message-Digest Algorithm<94> in all 
	material mentioning or referencing this software or this function.

	License is also granted to make and use derivative works provided that such 
	works are identified as <93>derived from the RSA Data Security, Inc. 
	MD4 Message-Digest Algorithm<94> in all material mentioning or referencing the 
	derived work. RSA Data Security, Inc. makes no representations concerning 
	either the merchantability of this software or the suitability of this software 
	for any particular purpose. It is provided <93>as is<94> without express or 
	implied warranty of any kind.
