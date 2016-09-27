"use strict";
var fs = require("fs");
var path = require("path");
var tagio = require("../lib");
var assert = require("chai").assert;


var fileCounter = 0;

var timestampFrames = [
   "TDRC",
   "TDOR",
   "TDEB",
   "TDRL",
   "TDTG"
];

describe("MPEG", function() {
    var testDir;
    var sampleFile;
    var testFile;
    var testJPEG;
    var testTEXT;

    beforeEach(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "../samples/sample.mp3");
        testFile = path.resolve(testDir, "test" + fileCounter++ + ".mp3");
        testJPEG = "file://" +  path.resolve(__dirname, "../samples/sample.jpg");
        testTEXT = "file://" +  path.resolve(__dirname, "../samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    afterEach(function () {
        //fs.unlink(testFile);
        //fs.rmdirSync(testDir);
    });

    // it("generic", function(done) {
    //     var config = {
    //         binaryDataDirectory: "/tmp",
    //         binaryDataUrlPrefix: "/attachments",
    //         binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    //         apeSave: false,
    //         id3v1Save: true,
    //         //id3v1Encoding: tagio.Encoding.UTF8,
    //         id3v2Save: false,
    //         id3v2Version: 3,
    //         id3v2Encoding: tagio.Encoding.UTF16
    //     };
    //     var mp3 = tagio.open(testFile, config);
    //     var cfg = mp3.getConfiguration();
    //     assert.equal(mp3.getPath(), "file://" + testFile);
    //     assert.equal(true, cfg.id3v1Save);
    //     assert.equal("UTF8", cfg.id3v1Encoding);
    //     assert.equal("UTF16", cfg.id3v2Encoding);
    //     assert.equal(3, cfg.id3v2Version);
    //
    //
    //     var tag = {
    //         "title": "Generic Title",
    //         "album": "Generic Album",
    //         "artist": "Generic Artist",
    //         "track": 1,
    //         "year": 2015,
    //         "genre": "Speech",
    //         "comment": "Generic Comment"
    //     };
    //     mp3.setTag(tag);
    //     mp3.save();
    //     mp3 = tagio.open(testFile, config);
    //     //mp3.log();
    //     assert.equal(JSON.stringify(mp3.getTag()), JSON.stringify(tag));
    //     done();
    // });

    //it("ape", function(done) {
    //    var apeConfig = {
    //        binaryDataDirectory: "/tmp",
    //        binaryDataUrlPrefix: "/attachments",
    //        binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    //        apeSave: true,
    //        id3v1Save: false,
    //        id3v2Save: false,
    //        id3v2Version: 3,
    //        id3v2Encoding: tagio.Encoding.UTF16
    //    };
    //    var mp3 = tagio.open(testFile, apeConfig);
    //    assert.equal(mp3.getPath(), "file://" + testFile);
    //    var tag = {
    //        "title": "Generic Title",
    //        "album": "Generic Album",
    //        "artist": "Generic Artist",
    //        "track": 1,
    //        "year": 2015,
    //        "genre": "Speech",
    //        "comment": "Generic Comment"
    //    };
    //    mp3.setAPETag(tag);
    //    mp3.save();
    //    mp3 = tagio.open(testFile, config);
    //    //mp3.log();
    //    assert.equal(JSON.stringify(mp3.getAPETag()), JSON.stringify(tag));
    //    done();
    //});


    it("Read plain", function(done) {

        var request = {
            path: testFile,
            configuration: { }
        };

        tagio.read(request).then(function (result) {
            //console.log(result);
            done();
        });
    });


    it("Write and read ID3v1 only", function(done) {

        var req = {
            path: testFile,
            configuration: {},
            id3v1: {
                "title": "Generic Title 2",
                "album": "Generic Album 2",
                "artist": "Generic Artist 2",
                "track": 1,
                "year": 2015,
                "genre": "Speech",
                "comment": "Generic Comment 2"
            },
            id3v2: []
        };
        tagio.write(req).then(function (res) {
            //console.log(res.id3v1);
            assert.equal(res.id3v1.title, req.id3v1.title);
            assert.equal(res.id3v1.album, req.id3v1.album);
            assert.equal(res.id3v1.artist, req.id3v1.artist);
            assert.equal(res.id3v1.track, req.id3v1.track);
            assert.equal(res.id3v1.year, req.id3v1.year);
            assert.equal(res.id3v1.genre, req.id3v1.genre);
            assert.equal(res.id3v1.comment, req.id3v1.comment);
            tagio.read(req).then(function (res) {
                assert.equal(res.id3v1.title, req.id3v1.title);
                assert.equal(res.id3v1.album, req.id3v1.album);
                assert.equal(res.id3v1.artist, req.id3v1.artist);
                assert.equal(res.id3v1.track, req.id3v1.track);
                assert.equal(res.id3v1.year, req.id3v1.year);
                assert.equal(res.id3v1.genre, req.id3v1.genre);
                assert.equal(res.id3v1.comment, req.id3v1.comment);
                done();
            });


        });
    });

    it("Write ID3v1 in unicode", function(done) {
        var req = {
            path: testFile,
            configuration: {},
            id3v1: {
                "title": "Příšerně",
                "album": "žluťoučký",
                "artist": "kůň",
                "track": 1,
                "year": 2015,
                "genre": "Speech",
                "comment": "úpěl ďábelské ódy"
            },
            id3v2: []
        };
        tagio.write(req).then(function (res) {
            //console.log(res.id3v1);
            assert.equal(res.id3v1.title, req.id3v1.title);
            assert.equal(res.id3v1.album, req.id3v1.album);
            assert.equal(res.id3v1.artist, req.id3v1.artist);
            assert.equal(res.id3v1.track, req.id3v1.track);
            assert.equal(res.id3v1.year, req.id3v1.year);
            assert.equal(res.id3v1.genre, req.id3v1.genre);
            assert.equal(res.id3v1.comment, req.id3v1.comment);
            tagio.read(req).then(function (res) {
                assert.equal(res.id3v1.title, req.id3v1.title);
                assert.equal(res.id3v1.album, req.id3v1.album);
                assert.equal(res.id3v1.artist, req.id3v1.artist);
                assert.equal(res.id3v1.track, req.id3v1.track);
                assert.equal(res.id3v1.year, req.id3v1.year);
                assert.equal(res.id3v1.genre, req.id3v1.genre);
                assert.equal(res.id3v1.comment, req.id3v1.comment);
                done();
            });
        });
    });

    it("Write ID3v2", function(done) {
        var frames = tagio.id3v2.frames;
        var itag = [];
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
                    text: "Some text"
                });
            } else {
                //console.log("Unsupported frame %s", frame.id);
            }
        }
        var req = {
            path: testFile,
            configuration: {},
            id3v1: {},
            id3v2: itag
        };

        //console.log(req.id3v2);
        tagio.write(req).then(function (res) {
            console.log(res.id3v2);
            // itag.forEach(function(iframe) {
            //     otag.forEach(function(oframe) {
            //         if (iframe.id === oframe.id && iframe.id !== "TIPL" && iframe.id !== "RVA2" ) {
            //             Object.keys(iframe).forEach(function(key) {
            //                 if (key === "id" || key === "image" || key === "object" || key === "file") return;
            //                 //console.log(iframe.id + "." + key + ": " + iframe[key] + " <-> " + oframe[key]);
            //                 assert.equal(iframe[key], oframe[key]);
            //             });
            //         }
            //     });
            // });
            //var otag = res.id3v2;
            // for (var i = 0, l = itag.length; i < l; i++) {
            //     var iframe = itag[i];
            //     var oframe = otag[i];
            //     console.log(oframe);
            //     assertEqual(iframe.id, oframe.id);
            // }
            done();
        });
    });
});
