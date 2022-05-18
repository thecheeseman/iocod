# iocod

[![Linux](https://img.shields.io/github/workflow/status/thecheeseman/iocod/Linux?label=Linux&logo=github&style=flat-square)]()
[![MacOS](https://img.shields.io/github/workflow/status/thecheeseman/iocod/MacOS?label=MacOS&logo=github&style=flat-square)]()
[![Windows](https://img.shields.io/github/workflow/status/thecheeseman/iocod/Windows?label=Windows&logo=github&style=flat-square)]()
[![License](https://img.shields.io/github/license/thecheeseman/iocod?&label=License&style=flat-square)](https://github.com/thecheeseman/iocod/blob/main/LICENSE)
[![Code Quality](https://img.shields.io/codacy/grade/f35c95a9a5a045f18a1e4b2dc7b4997d?label=Code%20Quality&style=flat-square)](https://www.codacy.com/gh/thecheeseman/iocod/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=thecheeseman/iocod&amp;utm_campaign=Badge_Grade)


**Table of Contents**

1. [About](#about)
2. [FAQ](#faq)
2. [Roadmap](#roadmap)
3. [Sources](#sources)
4. [Building & Installation](#building--installation)
5. [License](#license)

## About

iocod is an open source implementation of Call of Duty, patches 1.1-1.5. It is
based on the GPL releases of [Quake 3][1] / [RTCW][2] along with custom
implementations of game-specific functionality.

Currently, the project is focused on building an open source **server binary**
for Linux, Windows, & MacOS systems. A client version of this project is
planned, but not a priority at this point.

### Legal Advisory

    This project is not associated with, supported by, or provides any copyrighted
    materials or game assets from Infinity Ward, Activision, or Microsoft.

    You are required to supply your own legally-sourced game assets via an
    official download through a legitimate online retailer or via physical CD.

    Call of Duty is a registered trademark of Activision Publishing, Inc.

## FAQ

### What is the purpose of this project?

The purpose of this project is to replace the original Call of Duty server
executables and provide better interoperability, bug/exploit fixes, and 
extended functionality.

### What kinds of things are going to be added/fixed?

Here's a short list:

- DDoS packet protection
- Exploit & crash fixes
- New scripting functionality
    - ~100-150 new script functions
    - extended scripting language support (`#include` directives, `foreach()`,
      etc.)
    - script features from CoD2+
- Support for larger maps, more weapons, mods, gametypes
- Q3 Arena-like bot implementation
- Autodownloading / WWW downloads for faster redirects (like 1.5)
- Module support (add your own features and script functions)
- Tons of bug fixes and quality-of-life improvements
- Potential stuff to be implemented:
    - 1.1 & 1.5 intercompatibility - i.e. 1.1 players can play on 1.5 servers 
      and vice-versa
    - UO 1.41/1.51 compatibility - i.e. UO players can play on 1.1/1.5 servers
    - new map compiler to support even larger maps
- ... and many more!

### Will my CoDExtended mods/scripts work with this project?

Yes, there will be 100% script compatibility with CoDExtended. You will not 
have to change anything!

### What will I need to install this?

When the project is released, all you will need to do is download the server
binaries and run them like you normally run your server!

### What can I do with this right now? 

Short answer: **nothing**. It's not ready for public use. See the 
[roadmap](#roadmap) for more details.

### I have an idea for this project, can you implement it?

Maybe! Submit your ideas [here](https://github.com/thecheeseman/iocod/discussions)
and we'll see what we can do!

## Roadmap

| Timeline | &check; | System | Features |
| --- | --- |  --- | --- |
| **`2022/05`** |**--**| **`May`** |**---**|
| |**25%** completed| file system | efficient & fast<br/>switch to `zlib` / `unzip` |
| |**75%** completed| module system | support for dynamic modules<br/>module callbacks into real C code, <br/>API & documentation |
| |:white_check_mark:| core API | string library |
| |:white_check_mark:| core API | cvar, parse, infostring, logging |
| |**50%** completed | core API | system interface, vm, cmd |
| | | core API | net, msg, hunk mem |
| **`2022/06`** |**--**| **`June`** |**---**|
|  | | game engine | basic core functionality<br/>structures/data |
| | | scripting engine | basic core functionality<br/> |
| | | server | completed
| **`2022/07`** |**--**| **`July`** |**---**|
| | | game engine | completed
| | | scripting engine | completed
| **`2022/08`** |**--**| **`August`** | **`Public Alpha`** |
| **`2022/09`** |**--**| **`September`** | **`Public Beta`** |
| **`2022/10`** |**--**| **`October`** | **`Release v1.0`** |

### Release v1.0

With the first release, the expectations are to have a fully working 
server-side version that can easily be a drop-in replacement for any existing
1.1/1.5 dedicated server. 

This means it should work _indentically_ to the original, with added 
functionality. Bonus points if it actually runs better (less memory, faster, 
smoother gameplay).

### Future

- client side implementation
- latest openGL/AL or vulkan?
- metal on MacOS?

## Sources

iocod is based on the GPL releases of [id-software][8]'s
[Quake 3][1] / [RTCW][2]. Where possible, code has been modified from the
original sources to better suit the goals of this project. These changes
include minimal source changes as well as complete rewrites of original systems.

As this project is in a long line of other projects, it also shares features
and influences from the work done on [CoDExtended][3] by
[riicchhaarrd][4], [dftd][5], [prawy][9] and the rest of the xtnded team.
Inevitably, as the xtnded project was originally influenced and inspired by
[kungfooman][7]'s [libcod][6] project, that project has been included here
for posterity.

### id-software
  - [Quake III Team Arena GPL Source][1]
  - [RTCW MP GPL Source][2]

### secondary sources or inspirations
  - [CoDExtended][3] - [riicchhaarrd][4], [dftd][5], [prawy][9], and team
  - [libcod][6] - [kungfooman][7]
  - [libautoupdate][10] - Minqi Pan et al.
  - [c-logger][11] - Yuki Suzumoto

### libraries used
  - [dirent interface for MSVC][12] - Toni Ronkko
  - [dlfcn-win32][13] - Ramiro Polla
  - [getopt][14] - Hans Dietrich
  - [unistd.h][15] - AShelly

## Building & Installation

**WARNING:** At the moment, it is **not** recommended that you build from 
this repository as it is nowhere close to being ready for public distribution 
and testing. Check the [roadmap](#roadmap) for more details about the timeline.

**Build at your own risk.**

### Linux & MacOS
- a GNUC compatible compiler, i.e. `gcc` or `clang`
- `cmake` version 3.13 or greater
- `libcurl`

#### Instructions
1. Download from source or git clone repo
2. Run `cmake . && cmake --build .`
3. Copy `out/iocod` to your `callofduty` installation
4. Copy `out/game.mp.x86_64.so` to your `callofduty/main`


### Windows
- `MSVC` version 14.30 or greater, i.e. **Visual Studio 2022**
- `cmake` version 3.13 or greater
- `libcurl` (you can install with [vcpkg](https://vcpkg.io/en/index.html))

#### Instructions
1. Open Visual Studio 2022 & import the CMake project
2. Build from build menu
3. Navigate to output directory
4. Copy `out\iocod.exe` to your `Call of Duty` folder
5. Copy `out\game_mp_x86_64.dll` to your `Call of Duty\main` folder

## License

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

[1]: <https://github.com/id-Software/Quake-III-Arena>
[2]: <https://github.com/id-Software/RTCW-MP>
[3]: <https://github.com/xtnded/codextended>
[4]: <https://github.com/riicchhaarrd>
[5]: <https://github.com/dftd>
[6]: <https://github.com/kungfooman/libcod>
[7]: <https://github.com/kungfooman>
[8]: <https://github.com/id-software>
[9]: <https://github.com/PrawyCoD1>
[10]: <https://github.com/pmq20/libautoupdate>
[11]: <https://github.com/yksz/c-logger>
[12]: <https://github.com/tronkko/dirent>
[13]: <https://github.com/dlfcn-win32/dlfcn-win32>
[14]: <https://www.codeproject.com/Articles/1940/XGetopt-A-Unix-compatible-getopt-for-MFC-and-Win32>
[15]: <https://stackoverflow.com/a/826027/1202830>
