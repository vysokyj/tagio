# Basic Usage

TagIO has only three methods: configure, read and write. Read and write are promises.

```javascript
var tagio = require("tagio");
var conf = { audioPropertiesReadable: false };

// create global configuration with my preferences
tagio.configure(conf);

// read something
tagio.read({
       path: /home/someone/music.mp3,
       configuration: {
           id3v1Readable: false
       }
}).then(function (res) {
    console.log(res);
}).catch(function(err) { console.error(err); });

// write something
tagio.write({
       path: /home/someone/music.mp3,
       configuration: {
           id3v1Readable: true,
           id3v1Writable: true
       },
       id3v1: {
           "title": "Generic Title 2",
           "album": "Generic Album 2",
           "artist": "Generic Artist 2",
           "track": 1,
           "year": 2015,
           "genre": "Speech",
           "comment": "Generic Comment 2"
       }
}).then(function (res) {
    console.log(res);
}).catch(function(err) { console.error(err); });

```