var fs = require("fs");
var path = require("path");
var extend = require('util')._extend;
var id3v2 = require("./id3v2");
var tagioPlugin = require("../build/Release/tagio");

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

var Configuration = {
    fileExtracted: FileExtracted.AS_FILENAME,
    fileDirectory: "/tmp",
    fileUrlPrefix: "/attachments",
    
    APEWritable: true,
    APEReadable: true,
    APEExpunged: false,

    ID3v1Writable: true,
    ID3v1Readable: true,
    ID3v1Encoding: Encoding.UTF8,
    
    ID3v2Writable: true,
    ID3v2Readable: true,
    ID3v2Expunged: false,
    ID3v2Version: 4,
    ID3v2Encoding: Encoding.UTF8,
    ID3v2UseFrameEncoding: false
};

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
    c = extend(Configuration, c);
    if (!c) throw "Configuration missing";
    //if (!c.binaryDataDirectory) throw "Missing configuration.binaryDataDirectory";
    //if (!c.binaryDataUrlPrefix) throw "Missing configuration.binaryDataUrlPrefix";
    //if (!c.binaryDataMethod) throw "Missing configuration.binaryDataMethod";
    c.fileDirectory = checkDirectory(c.fileDirectory);
    return c;
};


var getNativeReadMethod = function (ext) {
    switch (ext) {
        case ".mp3":
            return tagioPlugin.readMPEG;
        // case ".flac":
        //     return new FLAC(filePath, configuration)
        default:
            return tagioPlugin.readGeneric;
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


module.exports = {
    read: read,
    id3v2: id3v2,
    Encoding: Encoding,
    FileExtracted: FileExtracted
};
