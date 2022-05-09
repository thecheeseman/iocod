---
layout: page
title: "Roadmap"
permalink: /roadmap/
---

# Roadmap

| Timeline | &check; | System | Features |
| --- | --- |  --- | --- |
| **`2022/05`** |**--**| **`May`** |**---**|
| | | file system | efficient & fast<br/>switch to `zlib` / `unzip` |
| | | module system | support for dynamic modules<br/>module callbacks into real C code, <br/>API & documentation |
| | | iocod API | internal common functions<br/>platform specific compilation/checks</br>string library</br>memory library |
| | | platform | basic networking functionality |
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