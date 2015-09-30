# Configuration Format

```javascript
var config = {
  binaryDataDirectory: "/tmp",
  binaryDataUrlPrefix: "/attachments",
  binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
  apeSave: false,
  id3v1Save: false,
  id3v2Save: true,
  id3v2Version: 4,
  id3v2Encoding: tagio.Encoding.UTF8,
  id3v2UseFrameEncoding: false
};
```

#### Binary Data Processing

* **binaryDataMethod** - method used for handling binary data
  * FILENAME method output: 01100af24849aec09055a35b0f6dd050.jpg
  * ABSOLUTE_URL method output: file://tmp/01100af24849aec09055a35b0f6dd050.jpg
  * RELATIVE_URL method output: /custom/prefix/01100af24849aec09055a35b0f6dd050.jpg
* **binaryDataDirectory** - directory used for exporting and importing file attachments such as cover images
* **binaryDataUrlPrefix** - prefix used in RELATIVE_URL method


#### Ape Tag

* **apeSave** - TagLib will save APE tag if possible

#### ID3v1 Tag

* **id3v1Save** - TagLib will save ID3v1 tag if possible

#### ID3v2 Tag

* **id3v2Save** - TagLib will save ID3v2 tag if possible
* **id3v2Version** - Used tag version. Possible values are 3 and 4.
* **id3v2Encoding** - Global text encoding
* **id3v2UseFrameEncoding** - Text encoding is set from frame input data - global text encoding ignored


