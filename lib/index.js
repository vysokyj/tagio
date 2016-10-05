const fs = require("fs");
const path = require("path");
const id3v2 = require("./id3v2");
const tagioPlugin = require("../build/Release/tagio");
const os = require("os");

var FileExtracted = {
    IS_IGNORED: "IS_IGNORED",
    AS_FILENAME: "AS_FILENAME",
    AS_ABSOLUTE_URL: "AS_ABSOLUTE_URL",
    AS_RELATIVE_URL: "AS_RELATIVE_URL"
};

var Encoding = {
    Latin1: "Latin1",
    UTF16: "UTF16",
    UTF16BE: "UTF16BE",
    UTF8: "UTF8",
    UTF16LE: "UTF16LE"
};

const defaultConfiguration = {
    fileExtracted: FileExtracted.AS_FILENAME,
    fileDirectory: os.tmpdir(),
    fileUrlPrefix: "/attachments",
    configurationReadable: false,
    audioPropertiesReadable: false,
    tagReadable: false,
    apeWritable: true,
    apeReadable: true,
    id3v1Writable: true,
    id3v1Readable: true,
    id3v1Encoding: Encoding.UTF8,
    id3v2Writable: true,
    id3v2Readable: true,
    id3v2Encoding: Encoding.UTF8,
    id3v2Version: 4,
    id3v2UseFrameEncoding: false,
    xiphCommentReadable: true,
    xiphCommentWritable: true
};

var configuration = Object.assign({}, defaultConfiguration);

var checkPath = function(f) {
    f = path.resolve(f);
    if (!fs.existsSync(f))
        throw "File '"  + f + "' not exists";
    if (!fs.statSync(f).isFile())
        throw "Path '"  + f + "' is not file";
    return f
};

var checkDirectory = function(d) {
    d = path.resolve(d);
    if (!fs.existsSync(d))
        throw "Directory - '"  + d + "' not exists";
    if (!fs.statSync(d).isDirectory())
        throw "Path - '"  + d + "' is not directory";
    return d;
};

var checkConfiguration = function(c) {
    c = Object.assign({}, configuration, c);
    c.fileDirectory = checkDirectory(c.fileDirectory);
    return c;
};


var getNativeReadMethod = function (ext) {
    switch (ext) {
        case ".mp3":
            return tagioPlugin.readMPEG;
        case ".flac":
            return tagioPlugin.readFLAC;
        default:
            return tagioPlugin.readGeneric;
    }
};

var getNativeWriteMethod = function (ext) {
    switch (ext) {
        case ".mp3":
            return tagioPlugin.writeMPEG;
        case ".flac":
            return tagioPlugin.writeFLAC;
        default:
            return tagioPlugin.writeGeneric;
    }
};


var read = function(request) {
    return new Promise(function(resolve, reject) {
        request.path = checkPath(request.path);
        request.configuration = checkConfiguration(request.configuration);
        var ext = path.extname(request.path);
        var nativeRead = getNativeReadMethod(ext);
        nativeRead(request, function (err, response) {
            if (err) reject(err);
            else resolve(response);
        });
    });
};

var write =function (request) {
    return new Promise(function(resolve, reject) {
        request.path = checkPath(request.path);
        request.configuration = checkConfiguration(request.configuration);
        //console.log(request);
        var ext = path.extname(request.path);
        var nativeWrite = getNativeWriteMethod(ext);
        nativeWrite(request, function (err, response) {
            if (err) reject(err);
            else resolve(response);
        });
    });
};

var configure = function (conf) {
    if (!conf) configuration = checkConfiguration(defaultConfiguration);
    else configuration = checkConfiguration(conf);
    return configuration;
};

module.exports = {
    configure: configure,
    read: read,
    write: write,
    id3v2: id3v2,
    Encoding: Encoding,
    FileExtracted: FileExtracted
};
