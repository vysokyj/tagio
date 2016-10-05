var tagio = require("../../lib");
var assert = require("chai").assert;

var timestampFrames = [
    "TDRC",
    "TDOR",
    "TDEB",
    "TDRL",
    "TDTG"
];

var getRandomInt = function (min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min;
};

var mapTestFrame = function (frame, testJPEG, testTEXT) {
    if (frame.id === "TXXX") {
        return {
            id: frame.id,
            text: frame.title,
            description: "CUSTOM"
        };
    } else if (timestampFrames.indexOf(frame.id) != -1) {
        return {
            id: frame.id,
            text: "2015-04-01T05:23:30"
        };
    } else if (frame.id[0] === "T") {
        return {
            id: frame.id,
            text: frame.title
        };
    } else if (frame.id === "WXXX") {
        return {
            id: frame.id,
            url: "http://www.testurl" + getRandomInt(1000, 9999) + ".com",
            description: frame.title
        };
    } else if (frame.id[0] === "W") {
        return {
            id: frame.id,
            url: "http://www.testurl" + getRandomInt(1000, 9999) + ".com"
        };
    } else if (frame.id === "COMM") {
        return {
            id: frame.id,
            text: frame.title
        };
    } else if (frame.id === "APIC") {
        return {
            id: frame.id,
            description: frame.title,
            mimeType: "image/jpeg",
            type: "0",
            picture: testJPEG
        };
    } else if (frame.id === "GEOB") {
        return {
            id: frame.id,
            mimeType: "text/plain",
            fileName: "sample.txt",
            description: frame.title,
            object: testTEXT
        };
    } else if (frame.id === "POPM") {
        return {
            id: frame.id,
            email: "someone@somewhere.com",
            rating: 120,
            counter: 25
        };
    } else if (frame.id === "PRIV") {
        return {
            id: frame.id,
            owner: "Someone"
        };
    // } else if (frame.id === "RVA2") {
    //     return {
    //         id: frame.id,
    //         channels: [
    //             {
    //                 channelType: 1, // master volume
    //                 volumeAdjustment: 0.5,
    //                 bitsRepresentingPeak: 125,
    //                 peakVolume: ""
    //             }
    //         ]
    //     };
    } else if (frame.id === "UFID") {
        return {
            id: frame.id,
            owner: "Someone",
            identifier: testTEXT
        };
    } else if (frame.id === "USLT") {
        return {
            id: frame.id,
            description: frame.title,
            language: "CZE",
            text: "Some text"
        };
    } else {
        //console.log("Frame ", frame.id, " is not supported by TagIO");
        return null;
    }
};


var generateTestFrames = function (testJPEG, testTEXT) {
    return tagio.id3v2.frames
        .map(function (n) { return mapTestFrame(n, testJPEG, testTEXT) })
        .filter(function(n) { return n != undefined });
};

var assertTestFrames = function (iframes, oframes) {
    if (!iframes || !oframes) throw "Missing frames";
    //if (iframes.length != oframes.length) throw "Frame count mismatch";
    iframes.forEach(function(iframe) {
        oframes.forEach(function(oframe) {
            if (iframe.id === oframe.id) {
                Object.keys(iframe).forEach(function(key) {
                    if (key === "id" || key === "picture" || key === "object" || key === "identifier") return;
                    //console.log(iframe.id + "." + key + ": " + iframe[key] + " <-> " + oframe[key]);
                    assert.equal(iframe[key], oframe[key]);
                });
            }
        });
    });
};

module.exports = {
    generateTestFrames: generateTestFrames,
    assertTestFrames: assertTestFrames
};