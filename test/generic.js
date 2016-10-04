"use strict";
var fs = require("fs");
var path = require("path");
//var tagio = require("../build/Release/tagio");
var tagio = require("../lib");
var assert = require("chai").assert;

var fileCounter = 0;


describe("WAV (generic tag)", function() {
    var testDir;
    var sampleFile;
    var testFile;
    var testJPEG;
    var testTEXT;

    beforeEach(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "../samples/sample.wav");
        testFile = path.resolve(testDir, "test" + fileCounter++ + ".wav");
        testJPEG = "file://" + path.resolve(__dirname, "../samples/sample.jpg");
        testTEXT = "file://" + path.resolve(__dirname, "../samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
        tagio.configure();
    });

    afterEach(function () {
        //fs.unlinkSync(testFile);
        //fs.rmdirSync(testDir);
    });

    it("read", function (done) {
        const req = {
            path: testFile,
            configuration: {
                configurationReadable: true,
                audioPropertiesReadable: true,
                tagReadable: true
            }
        };
        tagio.read(req).then(function (res) {
            assert.isNotNull(res);
            assert.isNotNull(res.path);
            assert.isNotNull(res.configuration);
            assert.isNotNull(res.audioProperties);
            assert.isNotNull(res.tag);
            done();
        });
    });

    it("write", function (done) {
        const req = {
            path: testFile,
            configuration: {
                configurationReadable: false,
                audioPropertiesReadable: false,
                tagReadable: true
            },
            tag: {
                "title": "Generic Title",
                "album": "Generic Album",
                "artist": "Generic Artist",
                "track": 1,
                "year": 2015,
                "genre": "Speech",
                "comment": "Generic Comment"
            }
        };
        tagio.write(req).then(function (res) {
            //console.log(res);
            assert.isNotNull(res.path);
            assert.isUndefined(res.configuration);
            assert.isUndefined(res.audioProperties);
            assert.isNotNull(res.tag);
            assert.equal(res.tag.title, req.tag.title);
            assert.equal(res.tag.album, req.tag.album);
            assert.equal(res.tag.artist, req.tag.artist);
            assert.equal(res.tag.track, req.tag.track);
            assert.equal(res.tag.year, req.tag.year);
            assert.equal(res.tag.genre, req.tag.genre);
            assert.equal(res.tag.comment, req.tag.comment);
            done();
        });

    });
});