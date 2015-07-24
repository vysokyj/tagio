// test.js

var tagio = require('./tagio');

var config = {
	attachmentsDir: "/tmp",
	attachmentsCtx: "/attachments/",
	saveID3v1Tag: false,
	saveID3v2Tag: true,
	saveApeTag: false
};

var printConfig = function(mp3) {
	console.log("getAlbum: \"%s\"", mp3.getAlbum());
	console.log("getArtist: \"%s\"", mp3.getArtist());
	console.log("getComment: \"%s\"", mp3.getComment());
	console.log("getGenre: \"%s\"", mp3.getGenre());
	console.log("getTitle: \"%s\"", mp3.getTitle());
	console.log("getTrack: %s", mp3.getTrack());
	console.log("getYear: %s", mp3.getYear());
	console.log("hasAPETag: %s", mp3.hasAPETag());
	console.log("hasID3v1Tag: %s", mp3.hasID3v1Tag());
	console.log("hasID3v2Tag: %s", mp3.hasID3v2Tag());
	console.log("path: %s", mp3.getPath());
};

var printID3v2Tag = function(mp3) {
	var frames = mp3.getID3v2Tag();
	for (var i = 0, l = frames.length; i < l; i++)
		console.log("%d: %j", i, frames[i]);

};

//var mp3 = new tagio.MPEG("./sample.mp3", config);
var mp3 = tagio.open("./sample.mp3", config);
////var mp3 = new tagio.MPEG("/home/mpd/cz/punk/psi-vojaci/02/01.mp3", config);
printConfig(mp3)
printID3v2Tag(mp3);
console.log("\nAFTER CHANGE:");
var tag = mp3.getID3v2Tag();
tag[0]['text'] = "Tam u pekelných bran";
mp3.setID3v2Tag(tag);
printID3v2Tag(mp3);

//console.log( 'This should be eight:', addon.add(3,5) );
