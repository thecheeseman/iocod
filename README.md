<!--
SPDX-FileCopyrightText: 2023 thecheeseman

SPDX-License-Identifier: CC0-1.0
-->

# iocod

iocod is an open source reimplementation of a modified id Tech 3 engine used
in several popular video game titles. It is based on the GPL release of
Quake III Arena™, and includes source code from other derivative GPL releases
of id Software titles such as Return to Castle Wolfenstein™, Enemy Territory™,
and Doom 3: BFG Edition™.

The GPL sources for each of these can be found below:

- [id-Software/Quake-III-Arena](https://github.com/id-Software/Quake-III-Arena)
- [id-Software/RTCW-MP](https://github.com/id-Software/RTCW-MP)
- [id-Software/Enemy-Territory](https://github.com/id-Software/Enemy-Territory)
- [id-Software/DOOM-3-BFG](https://github.com/id-Software/DOOM-3-BFG)

Other open source software used include:

- [ioquake3](https://github.com/ioquake/ioq3)
- [Catch2](https://github.com/catchorg/Catch2)
- [expected-lite](https://github.com/martinmoene/expected-lite)
- [fmtlib](https://github.com/fmtlib/fmt)
- [json](https://github.com/nlohmann/json)
- [miniz](https://github.com/richgel999/miniz)
- [spdlog](https://github.com/gabime/spdlog)

## Expected feature set

- DDoS packet protection
- Exploit & crash fixes
- New scripting functionality
    - ~100-150 new script functions
    - extended scripting language support (`#include` directives, `foreach()`,
      etc.)
    - script features from newer game engines
- Support for larger maps, more weapons, mods, gametypes
- Upgraded Q3 Arena bot implementation
- Autodownloading / WWW downloads for faster redirects
- Module support (add your own features and script functions)
- Tons of bug fixes and quality-of-life improvements
- Potential stuff to be implemented:
    - multi-client version intercompatibility
    - new map compiler to support even larger maps
- ... and many more!

### Legal Advisory

    This project is not associated with nor supported by id Software LLC,
    ZeniMax Media Inc., Activision Publishing Inc., or related companies,
    nor contains, provides, or otherwise makes accessible any copyrighted or
    proprietary materials, game assets, or other compiled software
    from any video game or series derived from the id Tech 3 engine.

    This project is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    Quake III Arena™, Return to Castle Wolfenstein™, Enemy Territory™, and
    Doom 3: BFG Edition™ are registered trademarks of ZeniMax Media Inc.

For more information or to read the full license agreement, see 
[LICENSE](LICENSE).
