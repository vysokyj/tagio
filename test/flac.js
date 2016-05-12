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

});
