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

### fileDirectory

Directory where attached files are exported. Defaults to system temp directory.

### fileExtracted

**NOT WORKING YET!**

Possible values:

* IS_IGNORED - attached file is not exported and JSON contains "IGNORED" string
* AS_FILENAME - attached file is exported and JSON contains plain file name
* AS_ABSOLUTE_URL - attached file is exported and JSON contains full path in format file:///somedir/somefile.ext
* AS_RELATIVE_URL - attached file is exported and JSON contains relative path = "someprefix/somefile.ext", someprefix is from variable fileUrlPrefix. 

### fileUrlPrefix

**NOT WORKING YET!**

Used when fileExtracted == tagio.FileExtracted.AS_RELATIVE_URL.

### somethingReadable

Manages reading of something from the input file.
 
### somethingWritable

Manages writing of something to the output file.

### somethingEncoding

Output encoding for something.

### id3v2Version

3 or 4 to write ID3v2.3 or ID3v2.4 

### id3v2UseFrameEncoding

Use or ignore encoding of ID3v2 frames. If false used id3v2Encoding property.


