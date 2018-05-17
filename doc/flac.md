# FLAC File Interface

## ID3v1 and ID3v2

See examples [mpeg](mpeg.md) section.

## XIPH Comment

Example:

```javascript
const tagio = import('tagio');

let req = {
    path: '/home/someone/sample.flac',
    configuration: {
        configurationReadable: false,
        audioPropertiesReadable: false,
        id3v1Readable: false,
        id3v1Writable: false,
        id3v2Readable: false,
        id3v2Writable: false,
        apeReadable: false,
        apeWritable: false,
        xiphCommentReadable: true,
        xiphCommentWritable: true
    },
    xiphComment: [
        {"id": "ALBUM", "text": "My another album"},
        {"id": "ARTIST", "text": "My another artist"},
        {"id": "COMMENT", "text": "Comment"}
    ]
};

tagio.write(req).then(function (res) {
    console.log(res);
}).catch(function(err) { console.error(err); });
```

Output is similar to input.