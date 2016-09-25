"use strict";
var fs = require("fs");
var path = require("path");
//var tagio = require("../build/Release/tagio");
var tagio = require("../lib");
var assert = require("chai").assert;

var fileCounter = 0;


describe("ASYNC", function() {
    var testDir;
    var sampleFile;
    var testFile;
    var testJPEG;
    var testTEXT;

    beforeEach(function () {
        testDir = path.resolve(__dirname, "../build/Test");
        sampleFile = path.resolve(__dirname, "../samples/sample.flac");
        testFile = path.resolve(testDir, "test" + fileCounter++ + ".flac");
        testJPEG = "file://" + path.resolve(__dirname, "../samples/sample.jpg");
        testTEXT = "file://" + path.resolve(__dirname, "../samples/sample.txt");
        if (!fs.existsSync(testDir)) fs.mkdirSync(testDir);
        fs.writeFileSync(testFile, fs.readFileSync(sampleFile));
    });

    afterEach(function () {
        //fs.rmdirSync(testDir);
        //fs.rmSync(testFile);
    });

    it("readAsync", function (done) {
        var request = {
            path: testFile,
            configuration: { }
        };
        // tagio.readGeneric(request, function (err, result) {
        //     console.log(result);
        //     done();
        // });
        tagio.read(request).then(function (result) {
            console.log(result);
            done();
        }).catch(function (err) {
            console.log("ERROR");
            console.error(err);
            done();
        });
    });
});