# Generic Interface

TagIO supports generic interface for all formats supported by TagLib.

Write:

```javascript
var tagio = require('tagio');

const req = {
    path: sample.wav,
    configuration: {
        configurationReadable: false,
        audioPropertiesReadable: true,
        tagReadable: true
    },
    tag: {
        title: 'Generic Title',
        album: 'Generic Album',
        artist: 'Generic Artist',
        track: 1,
        year: 2015,
        genre: 'Speech',
        comment: 'Generic Comment'
    }
};

tagio.write(req).then(function (res) {
    console.log(res);
});
```

Read:

```javascript
var tagio = require('tagio');

const req = {
    path: sample.wav,
    configuration: {
        configurationReadable: true,
        audioPropertiesReadable: true,
        tagReadable: true
    }
};

tagio.read(req).then(function (res) {
    console.log(res);
});
```

Output (same for both):

```json
{
  "path": "/home/someone/sample.wav",
  "audioProperties": {
     "length": 2,
     "bitrate": 1411,
     "sampleRate": 44100,
     "channels": 2
  },
  "tag": {
     "title": "Generic Title",
     "album": "Generic Album",
     "artist": "Generic Artist",
     "track": 1,
     "year": 2015,
     "genre": "Speech",
     "comment": "Generic Comment"
  }
}
```
