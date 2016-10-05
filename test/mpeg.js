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
        //testJPEG = "file://" +  path.resolve(__dirname, "../samples/sample.jpg");
        testJPEG = path.resolve(__dirname, "../samples/sample.jpg");
        //testTEXT = "file://" +  path.resolve(__dirname, "../samples/sample.txt");
        testTEXT = path.resolve(__dirname, "../samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
        tagio.configure();
    });

    afterEach(function () {
        //fs.unlink(testFile);
        //fs.rmdirSync(testDir);
    });

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
            configuration: {
                configurationReadable: false,
                audioPropertiesReadable: false,
                id3v1Readable: true,
                id3v1Writable: true,
                id3v2Readable: false,
                id3v2Writable: false,
                apeReadable: false,
                apeWritable: false,
                xiphCommentReadable: true,
                xiphCommentWritable: true
            },
            id3v1: {
                "title": "Generic Title 2",
                "album": "Generic Album 2",
                "artist": "Generic Artist 2",
                "track": 1,
                "year": 2015,
                "genre": "Speech",
                "comment": "Generic Comment 2"
            }
        };
        tagio.write(req).then(function (res) {
            //console.log(res);
            assert.isNotNull(res.path);
            assert.isUndefined(res.configuration);
            assert.isUndefined(res.audioProperties);
            assert.isUndefined(res.tag)
            assert.equal(res.id3v1.title, req.id3v1.title);
            assert.equal(res.id3v1.album, req.id3v1.album);
            assert.equal(res.id3v1.artist, req.id3v1.artist);
            assert.equal(res.id3v1.track, req.id3v1.track);
            assert.equal(res.id3v1.year, req.id3v1.year);
            assert.equal(res.id3v1.genre, req.id3v1.genre);
            assert.equal(res.id3v1.comment, req.id3v1.comment);
            return tagio.read(req);
        }).then(function (res) {
            assert.equal(res.id3v1.title, req.id3v1.title);
            assert.equal(res.id3v1.album, req.id3v1.album);
            assert.equal(res.id3v1.artist, req.id3v1.artist);
            assert.equal(res.id3v1.track, req.id3v1.track);
            assert.equal(res.id3v1.year, req.id3v1.year);
            assert.equal(res.id3v1.genre, req.id3v1.genre);
            assert.equal(res.id3v1.comment, req.id3v1.comment);
            done();
        }).catch(function(err) { done(err); });
    });

    it("Write ID3v1 and APE tag in unicode", function(done) {
        var req = {
            path: testFile,
            configuration: {
                id3v1Readable: true,
                id3v1Writable: true,
                id3v1Encoding: tagio.Encoding.UTF8,
                id3v2Readable: false,
                id3v2Writable: false,
                apeReadable: true,
                apeWritable: true,
                xiphCommentReadable: true,
                xiphCommentWritable: true
            },
            id3v1: {
                "title": "Příšerně",
                "album": "žluťoučký",
                "artist": "kůň",
                "track": 1,
                "year": 2015,
                "genre": "Speech",
                "comment": "úpěl ďábelské ódy"
            },
            ape: {
                "title": "Příšerně",
                "album": "žluťoučký",
                "artist": "kůň",
                "track": 1,
                "year": 2015,
                "genre": "Speech",
                "comment": "úpěl ďábelské ódy"
            }
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
            assert.equal(res.ape.title, req.ape.title);
            assert.equal(res.ape.album, req.ape.album);
            assert.equal(res.ape.artist, req.ape.artist);
            assert.equal(res.ape.track, req.ape.track);
            assert.equal(res.ape.year, req.ape.year);
            assert.equal(res.ape.genre, req.ape.genre);
            assert.equal(res.ape.comment, req.ape.comment);
            return tagio.read({
                path: testFile,
                configuration: {
                    id3v1Readable: true,
                    id3v1Writable: true,
                    id3v1Encoding: tagio.Encoding.UTF8,
                    id3v2Readable: false,
                    id3v2Writable: false,
                    apeReadable: true,
                    apeWritable: true,
                    xiphCommentReadable: true,
                    xiphCommentWritable: true
                }
            });
        }).then(function (res) {
            assert.isNotNull(res.path);
            assert.isUndefined(res.configuration);
            assert.isUndefined(res.audioProperties);
            assert.isUndefined(res.tag);
            assert.isUndefined(res.id3v2);
            assert.equal(res.id3v1.title, req.id3v1.title);
            assert.equal(res.id3v1.album, req.id3v1.album);
            assert.equal(res.id3v1.artist, req.id3v1.artist);
            assert.equal(res.id3v1.track, req.id3v1.track);
            assert.equal(res.id3v1.year, req.id3v1.year);
            assert.equal(res.id3v1.genre, req.id3v1.genre);
            assert.equal(res.id3v1.comment, req.id3v1.comment);
            assert.equal(res.ape.title, req.ape.title);
            assert.equal(res.ape.album, req.ape.album);
            assert.equal(res.ape.artist, req.ape.artist);
            assert.equal(res.ape.track, req.ape.track);
            assert.equal(res.ape.year, req.ape.year);
            assert.equal(res.ape.genre, req.ape.genre);
            assert.equal(res.ape.comment, req.ape.comment);
            req.configurationReadable = true;
            req.apeReadable = true;
            req.apeWritable = false;
            return tagio.write({
                path: testFile,
                configuration: {
                    configurationReadable: true,
                    audioPropertiesReadable: false,
                    id3v1Readable: true,
                    id3v1Writable: true,
                    id3v1Encoding: tagio.Encoding.UTF8,
                    id3v2Readable: true,
                    id3v2Writable: false,
                    apeReadable: true,
                    apeWritable: false,
                    xiphCommentReadable: true,
                    xiphCommentWritable: true
                },
                id3v1: {
                    "title": "Příšerně",
                    "album": "žluťoučký",
                    "artist": "kůň",
                    "track": 1,
                    "year": 2015,
                    "genre": "Speech",
                    "comment": "úpěl ďábelské ódy"
                },
                ape: {
                    "title": "Příšerně",
                    "album": "žluťoučký",
                    "artist": "kůň",
                    "track": 1,
                    "year": 2015,
                    "genre": "Speech",
                    "comment": "úpěl ďábelské ódy"
                }
            });
        }).then(function (res) {
            //console.log(res);
            assert.isNotNull(res.path);
            assert.isNotNull(res.configuration);
            assert.isUndefined(res.audioProperties);
            assert.isUndefined(res.tag);
            assert.isUndefined(res.id3v2);
            assert.equal(res.configuration.apeWritable, false);
            assert.isUndefined(res.ape); // ape should be ignored
            done()
        }).catch(function(err) { done(err); });
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
                    picture: testJPEG
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
                    identifier: testTEXT
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
            id3v2: itag
        };

        tagio.write(req).then(function (res) {
            //console.log(res.id3v2);
            const otag = res.id3v2;
            itag.forEach(function(iframe) {
                otag.forEach(function(oframe) {
                    if (iframe.id === oframe.id && iframe.id !== "TIPL" && iframe.id !== "RVA2" ) {
                        Object.keys(iframe).forEach(function(key) {
                            if (key === "id" || key === "picture" || key === "object" || key === "identifier") return;
                            //console.log(iframe.id + "." + key + ": " + iframe[key] + " <-> " + oframe[key]);
                            assert.equal(iframe[key], oframe[key]);
                        });
                    }
                });
            });
            done();
        }).catch(function(err) { done(err); });
    });
});
