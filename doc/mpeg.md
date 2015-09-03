# MPEG (MP3) File Interface

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
