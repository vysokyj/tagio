# Generic Interface

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
