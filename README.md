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

### Configuration

Configuration format:

```javascript
var config = {
  binaryDataDirectory: "/tmp",
  binaryDataUrlPrefix: "/attachments",
  binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
  saveID3v1Tag: false,
  saveID3v2Tag: true,
  saveApeTag: false
};
```

#### Binary data handling

* **binaryDataMethod** - method used for handling binary data
  * FILENAME method output: 01100af24849aec09055a35b0f6dd050.jpg
  * ABSOLUTE_URL method output: file://tmp/01100af24849aec09055a35b0f6dd050.jpg
  * RELATIVE_URL method output: /custom/prefix/01100af24849aec09055a35b0f6dd050.jpg
* **binaryDataDirectory** - directory used for exporting and importing file attachments such as cover images
* **binaryDataUrlPrefix** - prefix used in RELATIVE_URL method


#### Enable or disable tag formats

* **saveID3v1Tag** - TagLib will save ID3v1 tag if possible
* **saveID3v2Tag** - TagLib will save ID3v2 tag if possible
* **saveApeTag** - TagLib will save APE tag if possible

### Generic Tag Interface

TagIO supports generic interface for all formats supported by TagLib.

```javascript
var tagio = import('tagio');
var config = { ... };
var ogg = tagio.open(__dirname + "/samples/sample.ogg", config);
ogg.log();
```
Output:

```json
{
  "path": "/home/jirka/dev/github/tagio/test/samples/sample.ogg",
  "audioProperties": {
    "length": 2,
    "bitrate ": 112,
    "sampleRate": 44100,
    "channels": 2
  },
  "tags": {
    "generic": {
      "title": "My title",
      "album": "My album",
      "artist": "My artist",
      "track": 2,
      "year": 2000,
      "genre": "Rock",
      "comment": "My comment"
    }
  }
}
```

### MPEG (MP3) Interface

Example:

```javascript
var tagio = import('tagio');
var config = { ... };
var mp3 = tagio.open(__dirname + "/samples/sample.mp3", config);
mp3.log();
```
Output:

```json
{
  "path": "/home/jirka/dev/github/tagio/test/samples/sample.mp3",
  "audioProperties": {
    "length": 2,
    "bitrate ": 117,
    "sampleRate": 44100,
    "channels": 2
  },
  "tags": {
    "generic": {
      "title": "My title",
      "album": "My album",
      "artist": "My artist",
      "track": 1,
      "year": 2000,
      "genre": "Rock",
      "comment": "My comment"
    },
    "id3v2": [
      {
        "id": "TIT2",
        "text": "My title"
      },
      {
        "id": "TPE1",
        "text": "My artist"
      },
      {
        "id": "TPE2",
        "text": "My album artist"
      },
      {
        "id": "TALB",
        "text": "My album"
      },
      {
        "id": "TDRC",
        "text": "2000"
      },
      {
        "id": "TRCK",
        "text": "01"
      },
      {
        "id": "TCON",
        "text": "17"
      },
      {
        "id": "COMM",
        "text": "My comment"
      },
      {
        "id": "TCOM",
        "text": "My composer"
      },
      {
        "id": "TOPE",
        "text": "My original artist"
      },
      {
        "id": "TCOP",
        "text": "My copyright"
      },
      {
        "id": "WXXX",
        "url": "http://somewhere.com",
      },
      {
        "id": "TENC",
        "text": "Some Encoder"
      },
      {
        "id": "APIC",
        "mimeType": "image/jpeg",
        "description": "Cover Front.jpeg",
        "type": 3,
        "picture": "file:///tmp/01100af24849aec09055a35b0f6dd050.jpg"
      }
    ]
  }
}
```
### Available additional methods

* **hasAPETag()**
* **hasID3v1Tag()**
* **hasID3v2Tag()**
* **getID3v2Tag()**
* **setID3v2Tag(tagio)**

## Development notes

#### Trace errors

```bash
nm -C build/Release/tagio.node
```
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
