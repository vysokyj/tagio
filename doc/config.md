# Configuration Format

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
