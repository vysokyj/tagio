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

var print = function(mp3) {
	console.log("Path: %s", mp3.getPath());
	console.log("Generic tag: %j", mp3.getTag());
	if (mp3.hasID3v2Tag()) {
		console.log("ID3v2 tag:");
		var frames = mp3.getID3v2Tag();
		for (var i = 0, l = frames.length; i < l; i++)
			console.log(" - frame %d: %j", i, frames[i]);
	} else {
		console.log("ID3v2 tag: undedfined");
	}
};

var mp3 = tagio.open("./samples/sample.mp3", config);
print(mp3);

// some changes

var tag = {
	title: "My title",
	album: "My album",
	artist: "My artist",
	track: 1,
	genre: "Rock",
	comment: "My comment"
};

//mp3.setTag(tag);
//mp3.save();
//print(mp3);
