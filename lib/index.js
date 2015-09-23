var fs = require("fs");
var path = require("path");
var id3v2 = require("./id3v2");
var GENERIC = require("./generic");
var MPEG = require("./mpeg");

var BinaryDataMethod = {
    IGNORE: "IGNORE",
    FILENAME: "FILENAME",
    ABSOLUTE_URL: "ABSOLUTE_URL",
    RELATIVE_URL: "RELATIVE_URL"
};

var Configuration = {
    binaryDataDirectory: "/tmp",
    binaryDataUrlPrefix: "/attachments",
    binaryDataMethod: BinaryDataMethod.FILENAME,
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: false,
    id3v2Version: 4
};

var checkFile = function(f) {
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
    if (!c) throw "Configuration missing";
    if (!c.binaryDataDirectory) throw "Missing configuration.binaryDataDirectory";
    if (!c.binaryDataUrlPrefix) throw "Missing configuration.binaryDataUrlPrefix";
    if (!c.binaryDataMethod) throw "Missing configuration.binaryDataMethod";
    c.binaryDataDirectory = checkDirectory(c.binaryDataDirectory);
    return c;
};

var open = function(filePath, configuration) {
    filePath = checkFile(filePath);
    configuration = checkConfiguration(configuration);
    var ext = path.extname(filePath);
    switch (ext) {
        case ".mp3":
            return new MPEG(filePath, configuration);
        default:
            return new GENERIC(filePath, configuration);
    }
};

module.exports = {
    open: open,
    id3v2: id3v2,
    BinaryDataMethod: BinaryDataMethod
};