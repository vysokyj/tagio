// Example
var fs = require("fs");
var tagio = require("./tagio");

var config = {
	attachmentsDir: "/tmp",
	attachmentsCtx: "/attachments/",
	saveID3v1Tag: false,
	saveID3v2Tag: true,
	saveApeTag: false
};

var printTag = function(ref) {
    console.log("Path: %s", ref.getPath());
    console.log("Audio properties: %j", ref.getAudioProperties());
    console.log("Generic tag: %j", ref.getTag());
};

var printID3v2 = function(mp3) {
    if (mp3.hasID3v2Tag()) {
        console.log("ID3v2 tag:");
        var frames = mp3.getID3v2Tag();
        for (var i = 0, l = frames.length; i < l; i++)
            console.log(" - frame %d: %j", i, frames[i]);
    } else {
        console.log("ID3v2 tag: undefined");
    }
};

//var file = tagio.open("./samples/sample.mp3", config);
//printTag(file);
//printID3v2(file);

//file = tagio.open("./samples/sample.ogg", config);
//printTag(file);

var tag = {
    title: "My title",
    album: "My album",
    artist: "My artist",
    track: 1,
    genre: "Rock",
    comment: "My comment"
};


var file = tagio.open("./samples/sample.ogg", config);
printTag(file);
file.setTag(tag);
printTag(file);

// some changes



//mp3.setTag(tag);
//mp3.save();
//print(mp3);
