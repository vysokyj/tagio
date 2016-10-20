const fs = require("fs");
const path = require("path");
const id3v2 = require("./id3v2");
const tagioPlugin = require("../build/Release/tagio");
const os = require("os");
var Validator = require('jsonschema').Validator;
var validator = new Validator();

var configurationSchema = require("./schemas/configuration");

var id3v1schema = require("./schemas/id3v1");
var id3v2schemas = null;


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

var getID3v2Schema = function (id) {
    if (!id3v2schemas) {
        id3v2schemas = {};
        id3v2.frames.forEach(function (frame) {
            if (frame.supported) id3v2schemas[frame.id] = frame.schema;
        });
    }
    return id3v2schemas[id];
};

var checkMPEG = function (request) {
    if (request.configuration.id3v1Writable) {
        if (!request.id3v1) return "Missing id3v1";
        var result = validator.validate(request.id3v1, id3v1schema);
        return (result.errors.length > 0) ? "Invalid id3v2[" + index + "] - " + frame.id + " - " + result.errors : null;
    }
    if (request.configuration.id3v2Writable) {
        if (!request.id3v2) return "Missing id3v2";
        return request.id3v2.reduce(function (err, frame, index) {
            if (err != null) return err; // only first returned
            const schema = new getID3v2Schema(frame.id);
            if (!schema) return "Unsupported id3v2[" + index + "] - " + frame.id;
            var result = validator.validate(frame, schema);
            return (result.errors.length > 0) ? "Invalid id3v2[" + index + "] - " + frame.id + " - " + result.errors : null;
        }, null);
    }
    return null;
};

var checkData = function (request, ext) {
    switch (ext) {
        case ".mp3":
            return checkMPEG(request);
        default:
            return null;
    }
};

var checkConfiguration = function(c) {
    c = Object.assign({}, configuration, c);
    validator.validate(c, configurationSchema);
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
        var err = checkData(request, ext);
        if (err) reject(err);
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
