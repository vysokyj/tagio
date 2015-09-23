"use strict";
var fs = require("fs");
var path = require("path");
var tagio = require("../lib");
var assert = require("chai").assert;

var config = {
    binaryDataDirectory: "/tmp",
    binaryDataUrlPrefix: "/attachments",
    binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: false
};

describe("MPEG", function() {
    var testDir;
    var sampleFile;
    var testFile;
    var testJPEG;

    before(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "./samples/sample.mp3");
        testFile = path.resolve(testDir, "test.mp3");
        testJPEG = "file://" +  path.resolve(__dirname, "./samples/sample.jpg");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    after(function () {
        //fs.rmdirSync(testDir);
    });

    it("id3v2", function(done) {
        var mp3 = tagio.open(testFile, config);
        assert.equal(mp3.getPath(), testFile);


        //mp3.log();
        var it = [];

        var frames = tagio.id3v2.frames;
        //console.log(frames);
        for (var i = 0, l = frames.length; i < l; i++) {
            var frame = frames[i];
            //console.log(frame);
            if (frame.id === "TXXX") {
                it.push({
                    id: frame.id,
                    text: frame.title,
                    description: "CUSTOM"
                });
            } else if (frame.id[0] === "T") {
                it.push({
                    id: frame.id,
                    text: frame.title
                });
            } else if (frame.id === "WXXX") {
                 it.push({
                    id: frame.id,
                    url: "http://www.testurl" + i + ".com",
                    description: frame.title
                });
            } else if (frame.id[0] === "W") {
                it.push({
                    id: frame.id,
                    url: "http://www.testurl" + i + ".com"
                });
            } else if (frame.id === "COMM") {
                it.push({
                    id: frame.id,
                    text: frame.title
                });
            } else if (frame.id === "APIC") {
                it.push({
                    id: frame.id,
                    description: frame.title,
                    mimeType: "image/jpeg",
                    type: "0",
                    image: testJPEG
                });
            //} else if (frame.id === "GEOB") {
            //    it.push({
            //        id: frame.id,
            //        filename: "sample.jpg",
            //        description: frame.title,
            //        mimeType: "image/jpeg",
            //        object: testJPEG
            //    });
            } else {
                console.log("Unsupported frame %s", frame.id);
            }
        }
        mp3.setID3v2Tag(it);
        mp3.save();
        mp3 = tagio.open(testFile, config);
        mp3.log();

        done();
    })

});
