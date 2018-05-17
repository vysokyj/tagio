"use strict";
const fs = require("fs");
const path = require("path");
//const tagio = require("../build/Release/tagio");
const tagio = require("../lib");
const assert = require("chai").assert;
const os = require("os");

let fileCounter = 0;


describe("Configuration", function() {
    let testDir;
    let sampleFile;
    let testFile;
    let testJPEG;
    let testTEXT;

    beforeEach(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "../samples/sample.wav");
        testFile = path.resolve(testDir, "test" + fileCounter++ + ".wav");
        testJPEG = "file://" + path.resolve(__dirname, "../samples/sample.jpg");
        testTEXT = "file://" + path.resolve(__dirname, "../samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    afterEach(function () {
        //fs.unlinkSync(testFile);
        //fs.rmdirSync(testDir);
    });

    it("Use default configuration", function (done) {
        const conf = {
            fileExtracted: tagio.FileExtracted.IS_IGNORED,
                fileDirectory: os.tmpdir(),
                fileUrlPrefix: "/something",
                configurationReadable: true,
                audioPropertiesReadable: true,
                tagReadable: true,
                apeWritable: false,
                apeReadable: false,
                id3v1Writable: false,
                id3v1Readable: false,
                id3v1Encoding: tagio.Encoding.Latin1,
                id3v2Writable: false,
                id3v2Readable: false,
                id3v2Encoding: tagio.Encoding.UTF16BE,
                id3v2Version: 3,
                id3v2UseFrameEncoding: false,
                xiphCommentReadable: true,
                xiphCommentWritable: true
        };
        const req = {
            path: testFile
        };
        let res1 = tagio.configure(conf);
        assert.deepEqual(conf, res1);

        tagio.read(req).then(function (res) {
            assert.isNotNull(res);
            assert.isNotNull(res.path);
            assert.isNotNull(res.configuration);
            assert.deepEqual(conf, res.configuration);
            done();
        });
    });
});