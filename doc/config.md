# TagIO Configuration

## Full Configuration Example

```javascript
var configuration = {
    fileExtracted: tagio.FileExtracted.AS_FILENAME,
    fileDirectory: os.tmpdir(),
    fileUrlPrefix: "/attachments",
    configurationReadable: false,
    audioPropertiesReadable: false,
    tagReadable: false,
    apeWritable: true,
    apeReadable: true,
    id3v1Writable: true,
    id3v1Readable: true,
    id3v1Encoding: tagio.Encoding.UTF8,
    id3v2Writable: true,
    id3v2Readable: true,
    id3v2Encoding: tagio.Encoding.UTF8,
    id3v2Version: 4,
    id3v2UseFrameEncoding: false
};
```

## Configuration Properties

### fileExtracted

Possible values:

* IS_IGNORED - attached file is not exported and JSON contains "IGNORED" string
* AS_FILENAME - attached file is exported and JSON contains plain file name
* AS_ABSOLUTE_URL - attached file is exported and JSON contains full path in format file:///somedir/somefile.ext
* AS_RELATIVE_URL - attached file is exported and JSON contains relative path = "someprefix/somefile.ext", someprefix is from variable fileUrlPrefix. 

### fileDirectory

Directory where attached files are exported. Defaults to system temp directory.

### fileUrlPrefix

Used when fileExtracted == tagio.FileExtracted.AS_RELATIVE_URL.

### configurationReadable

If true returns parsed configuration in response. Defaults to false.

### audioPropertiesReadable

If true returns audio properties. Defaults to true.

### tagReadable

If true generic tag is included in response. Defaults to false.

### apeWritable

Write or keep APE tag when possible.

### id3v1Writable

Write or keep ID3v1 tag when possible.

### id3v2Writable

Write or keep ID3v2 tag when possible.

