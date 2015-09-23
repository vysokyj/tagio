"use strict";
var fs = require("fs");
var path = require("path");
var tagio = require("../lib");
var print = require("./print");
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

    before(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "./samples/sample.mp3");
        testFile = path.resolve(testDir, "test.mp3");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    after(function () {
        //fs.rmdirSync(testDir);
    });

    it("read", function(done) {
        var mp3 = tagio.open(testFile, config);
        //mp3.log();
        assert.equal(mp3.getPath(), testFile);
        done();
    })

});
