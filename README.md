# TagIO

[![Build Status](https://secure.travis-ci.org/vysokyj/tagio.svg?branch=master)](http://travis-ci.org/vysokyj/tagio)
[![Dependency Status](https://david-dm.org/vysokyj/tagio.svg)](https://david-dm.org/vysokyj/tagio)
[![NPM Version](https://img.shields.io/npm/v/tagio.svg)](https://npmjs.org/package/tagio)
[![NPM Downloads](https://img.shields.io/npm/dm/tagio.svg)](https://npmjs.org/package/tagio)

Advanced TagLib wrapper for Node.JS

## Status

**API Changed - TagIO is now multi-threaded and non-blocking and uses promises.**
**Windows build still not working. Help wanted.**

| *Format*   | *Status - supported tags*        |
| ---------- | -------------------------------- |
| MP3        | generic, ID3v1, ID3v2, APE       |
| MPC        | generic                          |
| FLAC       | generic, XIPH, ID3v1, ID3v2      |
| MP4        | generic                          |
| ASF        | generic                          |
| AIFF       | generic                          |
| WAV        | generic                          |
| TrueAudio  | generic                          |
| WavPack    | generic                          |
| Ogg FLAC   | generic                          |
| Ogg Vorbis | generic                          |
| Speex      | generic                          |
| Opus       | generic                          |

## Prerequisites

*   Installed C++ compiler supporting C++11 (GCC, CLANG, MSVC)
*   Installed [cmake](https://cmake.org/) (version 2.8 or above)
*   Installed [cmake-js](https://www.npmjs.com/package/cmake-j)

## Documentation

### General Usage Information

*   [Basic Usage](./doc/basic.md)
*   [Configuration Format](./doc/config.md)
*   [Generic Interface](./doc/generic.md)
*   [MPEG (MP3) File Interface](./doc/mpeg.md)

### Other

*   [Development Notes](./doc/notes.md)

## Copyright

TagIO - Advanced TagLib wrapper for Node.JS

Copyright (C) 2015  Jiri Vysoky

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
