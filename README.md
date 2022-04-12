# iocod

![Linux](https://github.com/thecheeseman/iocod/actions/workflows/linux.yml/badge.svg)
![MacOS](https://github.com/thecheeseman/iocod/actions/workflows/macos.yml/badge.svg)
![Windows](https://github.com/thecheeseman/iocod/actions/workflows/windows.yml/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f35c95a9a5a045f18a1e4b2dc7b4997d)](https://www.codacy.com?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=thecheeseman/iocod&amp;utm_campaign=Badge_Grade)

Table of Contents
1. [Introduction](#introduction)
2. [Build Requirements](#build-requirements)
    - [Linux Requirements](#linux-requirements)
    - [Windows Requirements](#windows-requirements)
    - [32-bit Compile Requirements](#bit-compile-requirements)
3. [Building](#building)
4. [Install](#install)
5. [Sources](#sources)
6. [License](#license)
7. [Excluded Code](#excluded-code)

## Introduction

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

## Build Requirements

### Linux Requirements

- `gcc` 5.0 or greater
- `cmake` 3.13 or greater
- _(optional)_ cURL

- Distro specific:
  - Debian/Unbuntu: `apt install libcurl4 libcurl4-openssl-dev`
  - CentOS: `yum install libcurl libcurl-devel`
  - Arch: `pacman -Ss curl`

### Windows Requirements
- Visual Studio 2019 or greater
- MSVC 16.8 or greater
- _(optional)_ use [vcpkg](https://vcpkg.io/en/index.html) to install 
  cURL libraries

### 32-bit Compile Requirements

If you are running a 64-bit system and would like to compile 32-bit binaries,
you will need to do a couple special things. 

**Debian & Ubuntu**
```bash
dpkg --add-architecture i386
apt update
apt install gcc-multilib
```

**CentOS**
```bash
yum install glibc.i686 glibc-devel.i686
```

Then you can compile with:
```bash
cmake -DCOMPILE_32_BIT=TRUE . && cmake --build .
```

## Building

Compilation is super simple: 
```bash
cmake . && cmake --build .
```

Optionally, you can add the following build flags:

- `-DCOMPILE_32_BIT-TRUE` force 32-bit compilation
- `-DDISABLE_AUTO_UPDATE=TRUE` disable auto update feature 
- `-DUSE_CURL=FALSE` disable cURL features (also disables auto update)

For example, to build a 32-bit binary with no cURL, you can run:

```bash
cmake -DCOMPILE_32_BIT=TRUE -DUSE_CURL=FALSE . && cmake --build .
```

## Install

Simply put the `iocod` binary into your Call of Duty server's root path, 
and then place the `game.mp.arch` library in the `main` folder.

## Sources

This project makes use of [id-software][8]'s [Quake 3][1] & [RTCW][2] GPL 
sources, as well as the excellent work done on [CoDExtended][3] by 
[riicchhaarrd][4], [dftd][5], and the rest of the xtnded team. Inevitably, 
as the xtnded project was originally influenced and inspired by 
[kungfooman][7]'s [libcod][6] project, that project has been included here 
for posterity. Additionally, any other projects who inspired or influenced
code decisions are listed below.

id-software
  - [Quake III Team Arena GPL Source][1]
  - [RTCW MP GPL Source][2]

xtnded team
  - [CoDExtended][3]

kungfooman
  - [libcod][6]

Minqi Pan et al.
  - [libautoupdate][9]

Yuki Suzumoto
  - [c-logger][10]

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

#### c-logger License
    Copyright (c) 2015 Yuki Suzumoto

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

[1]: <https://github.com/id-Software/Quake-III-Arena>
[2]: <https://github.com/id-Software/RTCW-MP>
[3]: <https://github.com/xtnded/codextended>
[4]: <https://github.com/riicchhaarrd>
[5]: <https://github.com/dftd>
[6]: <https://github.com/kungfooman/libcod>
[7]: <https://github.com/kungfooman>
[8]: <https://github.com/id-software>
[9]: <https://github.com/pmq20/libautoupdate>
[10]: <https://github.com/yksz/c-logger>
