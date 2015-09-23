"use strict";
var fs = require("fs");
var path = require("path");
var tagio = require("../lib");
var assert = require("chai").assert;

var timestampFrames = [
   "TDRC",
   "TDOR",
   "TDEB",
   "TDRL",
   "TDTG"
];


var config = {
    binaryDataDirectory: "/tmp",
    binaryDataUrlPrefix: "/attachments",
    binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: true,
    id3v2Version: 4
};

describe("MPEG", function() {
    var testDir;
    var sampleFile;
    var testFile;
    var testJPEG;
    var testTEXT;

    before(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "./samples/sample.mp3");
        testFile = path.resolve(testDir, "test.mp3");
        testJPEG = "file://" +  path.resolve(__dirname, "./samples/sample.jpg");
        testTEXT = "file://" +  path.resolve(__dirname, "./samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    after(function () {
        //fs.rmdirSync(testDir);
    });

    it("generic", function(done) {
        var config1 = {
            binaryDataDirectory: "/tmp",
            binaryDataUrlPrefix: "/attachments",
            binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
            saveID3v1Tag: true,
            saveID3v2Tag: false,
            saveApeTag: false,
            id3v2Version: 3
        };
        var mp3 = tagio.open(testFile, config1);
        assert.equal(mp3.getPath(), "file://" + testFile);
        var tag = {
            "title": "Generic Title",
            "album": "Generic Album",
            "artist": "Generic Artist",
            "track": 1,
            "year": 2015,
            "genre": "Speech",
            "comment": "Generic Comment"
        };
        mp3.setTag(tag);
        mp3.save();
        mp3 = tagio.open(testFile, config);
        //mp3.log();
        assert.equal(JSON.stringify(mp3.getTag()), JSON.stringify(tag));
        done();
    });

    it("id3v2", function(done) {
        var mp3 = tagio.open(testFile, config);
        assert.equal(mp3.getPath(), "file://" + testFile);


        //mp3.log();
        var itag = [];

        var frames = tagio.id3v2.frames;
        //console.log(frames);
        for (var i = 0, l = frames.length; i < l; i++) {
            var frame = frames[i];
            //console.log(frame);
            if (frame.id === "TXXX") {
                itag.push({
                    id: frame.id,
                    text: frame.title,
                    description: "CUSTOM"
                });
            } else if (timestampFrames.indexOf(frame.id) != -1) {

                itag.push({
                    id: frame.id,
                    text: "2015-04-01T05:23:30"
                });
            } else if (frame.id[0] === "T") {
                itag.push({
                    id: frame.id,
                    text: frame.title
                });
            } else if (frame.id === "WXXX") {
                itag.push({
                    id: frame.id,
                    url: "http://www.testurl" + i + ".com",
                    description: frame.title
                });
            } else if (frame.id[0] === "W") {
                itag.push({
                    id: frame.id,
                    url: "http://www.testurl" + i + ".com"
                });
            } else if (frame.id === "COMM") {
                itag.push({
                    id: frame.id,
                    text: frame.title
                });
            } else if (frame.id === "APIC") {
                itag.push({
                    id: frame.id,
                    description: frame.title,
                    mimeType: "image/jpeg",
                    type: "0",
                    image: testJPEG
                });
            } else if (frame.id === "GEOB") {
                itag.push({
                    id: frame.id,
                    mimeType: "text/plain",
                    fileName: "sample.txt",
                    description: frame.title,
                    object: testTEXT
                });
            } else if (frame.id === "POPM") {
                itag.push({
                    id: frame.id,
                    email: "someone@somewhere.com",
                    rating: 120,
                    counter: 25
                });
            } else if (frame.id === "PRIV") {
                itag.push({
                    id: frame.id,
                    owner: "Someone"
                });
            } else if (frame.id === "RVA2") {
                itag.push({
                    id: frame.id,
                    channels: [
                        {
                            channelType: 1, // master volume
                            volumeAdjustment: 0.5,
                            bitsRepresentingPeak: 125,
                            peakVolume: ""
                        }
                    ]
                });
            } else if (frame.id === "UFID") {
                itag.push({
                    id: frame.id,
                    owner: "Someone",
                    file: testTEXT
                });
            } else if (frame.id === "USLT") {
                itag.push({
                    id: frame.id,
                    description: frame.title,
                    language: "CZE",
                    textEncoding: "UTF8",
                    text: "Some text"
                });
            } else {
                //console.log("Unsupported frame %s", frame.id);
            }
        }
        mp3.setID3v2Tag(itag);
        mp3.save();

        mp3 = tagio.open(testFile, config);
        var otag = mp3.getID3v2Tag();
        //mp3.log();
        //console.log(otag);
        itag.forEach(function(iframe) {
            otag.forEach(function(oframe) {
                //TODO: Set by TagLib - why??
                if (iframe.id === oframe.id && iframe.id !== "TIPL" && iframe.id !== "RVA2" ) {
                    Object.keys(iframe).forEach(function(key) {
                       if (key === "id" || key === "image" || key === "object" || key === "file") return;
                       //console.log(iframe.id + "." + key + ": " + iframe[key] + " <-> " + oframe[key]);
                       assert.equal(iframe[key], oframe[key]);
                   });
                }
            });
        });

        //mp3.log();
        done();
    });

});
