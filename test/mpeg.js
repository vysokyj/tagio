"use strict";
const fs = require("fs");
const path = require("path");
const tagio = require("../lib");
const assert = require("chai").assert;

const id3v2Helper = require("./help/id3v2");

let fileCounter = 0;


describe("MPEG", function() {
    let testDir;
    let sampleFile;
    let testFile;
    let testJPEG;
    let testTEXT;

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

        let request = {
            path: testFile,
            configuration: { }
        };

        tagio.read(request).then(function(result) {
            //console.log(result);
            done();
        });
    });


    it("Write and read ID3v1 only", function(done) {
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

    it("Write ID3v1 and APE tag in unicode", function(done) {
        let req = {
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
            return res;
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
        const req = {
            path: testFile,
            configuration: {
                configurationReadable: true,
                audioPropertiesReadable: false,
                id3v1Readable: false,
                id3v1Writable: false,
                id3v1Encoding: tagio.Encoding.UTF8,
                id3v2Readable: true,
                id3v2Writable: true,
                apeReadable: true,
                apeWritable: false,
                xiphCommentReadable: true,
                xiphCommentWritable: true
            },
            id3v2: id3v2Helper.generateTestFrames(testJPEG, testTEXT)
        };
        //console.log(req);
        tagio.write(req).then(function (res) {
            id3v2Helper.assertTestFrames(req.id3v2, res.id3v2);
            done();
        }).catch(function(err) { done(err); });
    });
});
