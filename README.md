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

iocod is an open source implementation of Call of Duty™, for patches 1.1 & 1.5.
It is based on the GPL releases of [Quake III Team Arena][quake3] along with
two of its derivatives: [ioquake3][ioquake3] and [Return to Castle Wolfenstein][rtcw]. 
See [Sources](#sources) for more details. 

Currently, the project is focused on building an open source **server binary** 
for Linux, Windows, & MacOS systems which is interoperable with the existing 
original client executables. 

An open source client version of this project is planned for some time in 2023.

### Legal Advisory

    This project is not associated with, supported by, or provides any copyrighted
    materials or game assets from Infinity Ward, Activision, or Microsoft.

    You are required to supply your own legally-sourced game assets via an
    official download through a legitimate online retailer or via physical CD.

    Call of Duty™ is a registered trademark of Activision Publishing, Inc.

## FAQ

### What is the purpose of this project?

The purpose of this project is to provide binary replacements for the original 
Call of Duty™ server executables to allow for better interoperability across
patch versions, bug/exploit fixes, as well as extending functionality.

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

### What can I do with this right now? 

Short answer: **nothing**. It's not ready for public use. See the 
[roadmap](#roadmap) for more details.

### I have an idea for this project, can you implement it?

Maybe! Submit your ideas [here](https://github.com/thecheeseman/iocod/discussions)
and we'll see what we can do!

## Roadmap

| Timeline          | Milestones
| ----------------- | ---
| **`July`**        | <ul><li>Core components</li><li>Game engine</li><li>Networking</li><li>UI</li>
| **`August`**      | <ul><li>Game dll</li><li>Script engine</li></ul>
| **`September`**   | **Alpha/Beta** - TBD
| **`October`**     | **1.0 Release**
| **`November`**    | **1.1 Release** <br/>Bug fixes/performance enhancements, etc.

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

iocod is based primarily on three GPL releases:
- [Quake III Team Arena][quake3] and its derivatives
    - [ioquake3][ioquake3]
    - [Return to Castle Wolfenstein][rtcw]

Where possible, code has been modified from the original sources to better 
suit the goals of this project. These changes include minimal source changes 
as well as complete rewrites of original systems.

This project is another in a long line of other Call of Duty™ modifications
and projects, and shares features from previous work done on the
[CoDExtended][codextended] and [libcod][libcod] projects. While very little 
code has been actively borrowed from either of these projects, their influence
on this project cannot be understated. Therefore, for posterity, the authors
and contributors for those projects are cited as major influences for this
project:

- [riicchhaarrd][riicchhaarrd]
- [dftd][dftd]
- [prawy][prawy]
- [kungfooman][kungfooman]

### Secondary Sources
  - [libautoupdate][libautoupdate] - Minqi Pan et al. - used to prepare 
  custom autoupdate system along with code from [ioquake3][ioquake3]
  - [c-logger][c-logger] - Yuki Suzumoto - used to build custom logging system

### Libraries bundled
  - [miniz][miniz] - richgel999 - for handling zip/inflate/deflate

## Building & Installation

**WARNING:** At the moment, it is **not** recommended that you build from 
this repository as it is nowhere close to being ready for public distribution 
and testing. Check the [roadmap](#roadmap) for more details about the timeline.

**Build at your own risk.**

### General Requirements

- A compatible C11 compiler
    - [GCC][gcc]/[Clang][clang] - any version since about 2012 will do
    - [MSVC][msvc] - version 19.28 at a minimum (Visual Studio 2019 16.8)
- [CMake][cmake] 3.14 or greater

### Linux & MacOS Instructions

#### Required Packages

- git
- build-essential
- cmake
- libncurses-dev

#### Process

1. Download source or `git clone` from GitHub:
    ```bash
    git clone http://github.com/thecheeseman/iocod.git
    ```

    Or if you want the latest development version, then use:
    ```bash
    git clone --branch develop https://github.com/thecheeseman/iocod.git
    ```
2. Build process
    ```bash
    cd iocod
    mkdir build
    cmake . -B build
    cmake --build build 
    ```
3. If everything compiled OK, copy files from `build/staged/dedicated` to your
installation folder.

### Windows Instructions

1. Download source from GitHub
2. Open Visual Studio 2019/2022 & import the CMake project
3. Build from build menu
4. Navigate to output directory
5. Copy `bin\iocodded64.exe` to your `Call of Duty` folder
6. Copy `bin\main\icgame64.dll` to your `Call of Duty\main` folder

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


[//]: # (Links for reference)
[id-software]: <https://github.com/id-software>
[quake3]: <https://github.com/id-Software/Quake-III-Arena>
[rtcw]: <https://github.com/id-Software/RTCW-MP>
[ioquake3]: <https://github.com/ioquake/ioq3>
[codextended]: <https://github.com/xtnded/codextended>
[libcod]: <https://github.com/kungfooman/libcod>

[riicchhaarrd]: <https://github.com/riicchhaarrd>
[dftd]: <https://github.com/dftd>
[kungfooman]: <https://github.com/kungfooman>
[prawy]: <https://github.com/PrawyCoD1>
[libautoupdate]: <https://github.com/pmq20/libautoupdate>
[c-logger]: <https://github.com/yksz/c-logger>
[miniz]: <https://github.com/richgel999/miniz>

[git]: <https://git-scm.com/>
[msvc]: <https://visualstudio.microsoft.com/>
[clang]: <https://clang.llvm.org/>
[gcc]: <https://gcc.gnu.org/>
[cmake]: <https://cmake.org/>
