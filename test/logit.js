//// Example
//var fs = require("fs");
//var tagio = require("../lib/tagio");
//var print = require("./print");
//
//var config = {
//    binaryDataDirectory: "/tmp",
//    binaryDataUrlPrefix: "/attachments",
//    binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
//	saveID3v1Tag: false,
//	saveID3v2Tag: true,
//	saveApeTag: false
//};
//
//// declaration
//var tag = {
//    title: "My title",
//    album: "My album",
//    artist: "My artist",
//    track: 1,
//    genre: "Rock",
//    comment: "My comment"
//};
//
//var mp3 = tagio.open(__dirname + "/samples/sample.mp3", config);
//mp3.log();
//
//var ogg = tagio.open(__dirname + "/samples/sample.ogg", config);
//ogg.log();
//
//var wav = tagio.open(__dirname + "/samples/sample.wav", config);
//wav.log();