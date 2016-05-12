"use strict";
var fs = require("fs");
var path = require("path");
var tagio = require("../lib");
var assert = require("chai").assert;


var fileCounter = 0;

var config = {
    binaryDataDirectory: "/tmp",
    binaryDataUrlPrefix: "/attachments",
    binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    apeSave: true,
    id3v1Save: false,
    id3v1Encoding: tagio.Encoding.UTF8,
    id3v2Save: true,
    id3v2Version: 4,
    id3v2Encoding: tagio.Encoding.UTF8,
    id3v2UseFrameEncoding: false
};

describe("FLAC", function() {
    var testDir;
    var sampleFile;
    var testFile;
    var testJPEG;
    var testTEXT;

    beforeEach(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "../samples/sample.flac");
        testFile = path.resolve(testDir, "test" + fileCounter++ + ".flac");
        testJPEG = "file://" +  path.resolve(__dirname, "../samples/sample.jpg");
        testTEXT = "file://" +  path.resolve(__dirname, "../samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    afterEach(function () {
        //fs.rmdirSync(testDir);
        //fs.rmSync(testFile);
    });

    it("generic", function(done) {
        var config = {
            binaryDataDirectory: "/tmp",
            binaryDataUrlPrefix: "/attachments",
            binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
            apeSave: false,
            id3v1Save: true,
            //id3v1Encoding: tagio.Encoding.UTF8,
            id3v2Save: false,
            id3v2Version: 3,
            id3v2Encoding: tagio.Encoding.UTF16
        };
        var flac = tagio.open(testFile, config);
        var cfg = flac.getConfiguration();
        assert.equal(flac.getPath(), "file://" + testFile);
        assert.equal(true, cfg.id3v1Save);
        assert.equal("UTF8", cfg.id3v1Encoding);
        assert.equal("UTF16", cfg.id3v2Encoding);
        assert.equal(3, cfg.id3v2Version);


        var tag = {
            "title": "Generic Title",
            "album": "Generic Album",
            "artist": "Generic Artist",
            "track": 1,
            "year": 2015,
            "genre": "Speech",
            "comment": "Generic Comment"
        };
        flac.setTag(tag);
        flac.save();
        flac = tagio.open(testFile, config);
        //flac.log();
        assert.equal(JSON.stringify(flac.getTag()), JSON.stringify(tag));
        done();
    });

    it("xiphComment", function(done) {
        var config = {
            binaryDataDirectory: "/tmp",
            binaryDataUrlPrefix: "/attachments",
            binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
            apeSave: false,
            id3v1Save: true,
            id3v1Encoding: tagio.Encoding.UTF8,
            id3v2Save: false,
            id3v2Version: 3,
            id3v2Encoding: tagio.Encoding.UTF16
        };

        var tag = [
            {"id": "ALBUM", "text": "My another album"},
            {"id": "ARTIST", "text": "My another artist"},
            {"id": "COMMENT", "text": "Comment"}
        ];

        var flac = tagio.open(testFile, config);
        assert.equal(flac.getPath(), "file://" + testFile);
        flac.setXiphComment(tag);
        flac.save();

        flac = tagio.open(testFile, config);
        var byKey = function(a, b) { a.id.localeCompare(b.id) };
        var actual = flac.getXiphComment().sort(byKey);
        var expected = tag.sort(byKey);
        //flac.log();
        assert.equal(JSON.stringify(actual), JSON.stringify(expected));
        done();
    });

    // it("id3v1", function(done) {
    //     var config = {
    //         binaryDataDirectory: "/tmp",
    //         binaryDataUrlPrefix: "/attachments",
    //         binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    //         apeSave: false,
    //         id3v1Save: true,
    //         id3v1Encoding: tagio.Encoding.UTF8,
    //         id3v2Save: false,
    //         id3v2Version: 3,
    //         id3v2Encoding: tagio.Encoding.UTF16
    //     };
    //     var flac = tagio.open(testFile, config);
    //     assert.equal(flac.getPath(), "file://" + testFile);
    //     var tag = {
    //         "title": "Generic Title 2",
    //         "album": "Generic Album 2",
    //         "artist": "Generic Artist 2",
    //         "track": 1,
    //         "year": 2015,
    //         "genre": "Speech",
    //         "comment": "Generic Comment 2"
    //     };
    //     flac.setID3v1Tag(tag);
    //     flac.save();
    //     flac = tagio.open(testFile, config);
    //     //flac.log();
    //     assert.equal(JSON.stringify(flac.getID3v1Tag()), JSON.stringify(tag));
    //     done();
    // });
    //
    // it("id3v1-unicode", function(done) {
    //     var config = {
    //         binaryDataDirectory: "/tmp",
    //         binaryDataUrlPrefix: "/attachments",
    //         binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    //         apeSave: false,
    //         id3v1Save: true,
    //         id3v1Encoding: tagio.Encoding.UTF8,
    //         id3v2Save: false,
    //         id3v2Version: 3,
    //         id3v2Encoding: tagio.Encoding.UTF16
    //     };
    //     var flac = tagio.open(testFile, config);
    //     assert.equal(flac.getPath(), "file://" + testFile);
    //     var tag = {
    //         "title": "Příšerně",
    //         "album": "žluťoučký",
    //         "artist": "kůň",
    //         "track": 1,
    //         "year": 2015,
    //         "genre": "Speech",
    //         "comment": "úpěl ďábelské ódy"
    //     };
    //     flac.setID3v1Tag(tag);
    //     flac.save();
    //     flac = tagio.open(testFile, config);
    //     //flac.log();
    //     assert.equal(JSON.stringify(flac.getID3v1Tag()), JSON.stringify(tag));
    //     done();
    // });
    //
    //
    //
    // it("id3v2", function(done) {
    //     var flac = tagio.open(testFile, config);
    //     assert.equal(flac.getPath(), "file://" + testFile);
    //
    //
    //     //flac.log();
    //     var itag = [];
    //
    //     var frames = tagio.id3v2.frames;
    //     //console.log(frames);
    //     for (var i = 0, l = frames.length; i < l; i++) {
    //         var frame = frames[i];
    //         //console.log(frame);
    //         if (frame.id === "TXXX") {
    //             itag.push({
    //                 id: frame.id,
    //                 text: frame.title,
    //                 description: "CUSTOM"
    //             });
    //         } else if (timestampFrames.indexOf(frame.id) != -1) {
    //
    //             itag.push({
    //                 id: frame.id,
    //                 text: "2015-04-01T05:23:30"
    //             });
    //         } else if (frame.id[0] === "T") {
    //             itag.push({
    //                 id: frame.id,
    //                 text: frame.title
    //             });
    //         } else if (frame.id === "WXXX") {
    //             itag.push({
    //                 id: frame.id,
    //                 url: "http://www.testurl" + i + ".com",
    //                 description: frame.title
    //             });
    //         } else if (frame.id[0] === "W") {
    //             itag.push({
    //                 id: frame.id,
    //                 url: "http://www.testurl" + i + ".com"
    //             });
    //         } else if (frame.id === "COMM") {
    //             itag.push({
    //                 id: frame.id,
    //                 text: frame.title
    //             });
    //         } else if (frame.id === "APIC") {
    //             itag.push({
    //                 id: frame.id,
    //                 description: frame.title,
    //                 mimeType: "image/jpeg",
    //                 type: "0",
    //                 image: testJPEG
    //             });
    //         } else if (frame.id === "GEOB") {
    //             itag.push({
    //                 id: frame.id,
    //                 mimeType: "text/plain",
    //                 fileName: "sample.txt",
    //                 description: frame.title,
    //                 object: testTEXT
    //             });
    //         } else if (frame.id === "POPM") {
    //             itag.push({
    //                 id: frame.id,
    //                 email: "someone@somewhere.com",
    //                 rating: 120,
    //                 counter: 25
    //             });
    //         } else if (frame.id === "PRIV") {
    //             itag.push({
    //                 id: frame.id,
    //                 owner: "Someone"
    //             });
    //         } else if (frame.id === "RVA2") {
    //             itag.push({
    //                 id: frame.id,
    //                 channels: [
    //                     {
    //                         channelType: 1, // master volume
    //                         volumeAdjustment: 0.5,
    //                         bitsRepresentingPeak: 125,
    //                         peakVolume: ""
    //                     }
    //                 ]
    //             });
    //         } else if (frame.id === "UFID") {
    //             itag.push({
    //                 id: frame.id,
    //                 owner: "Someone",
    //                 file: testTEXT
    //             });
    //         } else if (frame.id === "USLT") {
    //             itag.push({
    //                 id: frame.id,
    //                 description: frame.title,
    //                 language: "CZE",
    //                 text: "Some text"
    //             });
    //         } else {
    //             //console.log("Unsupported frame %s", frame.id);
    //         }
    //     }
    //     flac.setID3v2Tag(itag);
    //     //flac.save();
    //     //flac = tagio.open(testFile, config);
    //
    //     var otag = flac.getID3v2Tag();
    //     //flac.log();
    //     //console.log(otag);
    //     itag.forEach(function(iframe) {
    //         otag.forEach(function(oframe) {
    //             //TODO: Set by TagLib - why??
    //             if (iframe.id === oframe.id && iframe.id !== "TIPL" && iframe.id !== "RVA2" ) {
    //                 Object.keys(iframe).forEach(function(key) {
    //                    if (key === "id" || key === "image" || key === "object" || key === "file") return;
    //                    //console.log(iframe.id + "." + key + ": " + iframe[key] + " <-> " + oframe[key]);
    //                    assert.equal(iframe[key], oframe[key]);
    //                });
    //             }
    //         });
    //     });
    //
    //     //flac.log();
    //     done();
    // });

});
