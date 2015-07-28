# TagIO

Advanced TagLib wrapper for Node.JS

**This module is under hard development!**

## Implemented

* MPEG - generic API, ID3v2


## Prerequisites

* Mac or Linux OS
* Installed cmake
* Installed taglib

## Usage

### Generic Interface

TagLib supports generic interface for all supported formats.

```javascript
var tagio = import('tagio');

var config = { ... };

var mp3 = tagio.open("./sample.mp3", config);
console.log("path: %s", mp3.getPath());
console.log("getAlbum: \"%s\"", mp3.getAlbum());
console.log("getArtist: \"%s\"", mp3.getArtist());
console.log("getComment: \"%s\"", mp3.getComment());
console.log("getGenre: \"%s\"", mp3.getGenre());
console.log("getTitle: \"%s\"", mp3.getTitle());
console.log("getTrack: %s", mp3.getTrack());
console.log("getYear: %s", mp3.getYear());

```

### MPEG (MP3) Interface

```javascript
var tagio = import('tagio');

var config = {
	attachmentsDir: "/tmp",
	attachmentsCtx: "/attachments/",
	saveID3v1Tag: false,
	saveID3v2Tag: true,
	saveApeTag: false
};

var mp3 = tagio.open("./sample.mp3", config);

console.log("hasAPETag: %s", mp3.hasAPETag());
console.log("hasID3v1Tag: %s", mp3.hasID3v1Tag());
console.log("hasID3v2Tag: %s", mp3.hasID3v2Tag());

var frames = mp3.getID3v2Tag();
for (var i = 0, l = frames.length; i < l; i++) {
    console.log("%d: %j", i, frames[i]);
}
```

#### Configuration Parameters

* **attachmentsDir** - directory used for exporting and importing attachments such as cover images
* **attachmentsCtx** - prefix generated to the output file path in jsoun - for server purposes
* **saveID3v1Tag** - TagLib will save ID3v1 tag
* **saveID3v2Tag** - TagLib will save ID3v2 tag
* **saveApeTag** - TagLib will save APE tag


## Development notes

## Trace errors

nm -C build/Release/tagio.node

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
