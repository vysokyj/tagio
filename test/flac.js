"use strict";
const fs = require("fs");
const path = require("path");
const tagio = require("../lib");
const assert = require("chai").assert;
const id3v2Helper = require("./help/id3v2");

let fileCounter = 0;


describe("FLAC", function() {
    let testDir;
    let sampleFile;
    let testFile;
    let testJPEG;
    let testTEXT;

    beforeEach(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "../samples/sample.flac");
        testFile = path.resolve(testDir, "test" + fileCounter++ + ".flac");
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



    it("Write XIPH", function(done) {
        let req = {
            path: testFile,
            configuration: {
                configurationReadable: false,
                audioPropertiesReadable: false,
                id3v1Readable: false,
                id3v1Writable: false,
                id3v2Readable: false,
                id3v2Writable: false,
                apeReadable: false,
                apeWritable: false,
                xiphCommentReadable: true,
                xiphCommentWritable: true
            },
            xiphComment: [
                {"id": "ALBUM", "text": "My another album"},
                {"id": "ARTIST", "text": "My another artist"},
                {"id": "COMMENT", "text": "Comment"}
            ]
        };

        tagio.write(req).then(function (res) {
            assert.isNotNull(res.path);
            assert.isUndefined(res.configuration);
            assert.isUndefined(res.audioProperties);
            assert.isUndefined(res.tag);
            let byKey = function(a, b) { a.id.localeCompare(b.id) };
            let actual = res.xiphComment.sort(byKey);
            let expected = req.xiphComment.sort(byKey);
            assert.deepEqual(actual, expected);
            done();
        }).catch(function(err) { done(err); });
    });

    it("Write ID3v1 only", function(done) {
        let req = {
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
                xiphCommentReadable: false,
                xiphCommentWritable: false
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
            assert.isUndefined(res.tag);
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

    it("Write ID3v2", function(done) {
        const req = {
            path: testFile,
            id3v2: id3v2Helper.generateTestFrames(testJPEG, testTEXT)
        };
        tagio.write(req).then(function (res) {
            id3v2Helper.assertTestFrames(req.id3v2, res.id3v2);
            done();
        }).catch(function(err) { done(err); });
    });

});
