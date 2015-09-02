var tagio = require("./build/Release/tagio");
var fs = require("fs");
var path = require("path");

tagio.genres = require("./genres");

tagio.BinaryDataMethod = {
    FILENAME: "FILENAME",
    ABSOLUTE_URL: "ABSOLUTE_URL",
    RELATIVE_URL: "RELATIVE_URL"
};

tagio.Configuration = {
    binaryDataDirectory: "/tmp",
    binaryDataUrlPrefix: "/attachments",
    binaryDataMethod: tagio.BinaryDataMethod.FILENAME,
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: false
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

//var checkConfiguration = function(c) {
//    if (!c) throw "Configuration missing";
//    if (!c.binaryDataDirectory) throw "Missing configuration.binaryDataDirectory";
//    if (!c.binaryDataUrlPrefix) throw "Missing configuration.binaryDataUrlPrefix";
//    if (!c.binaryDataMethod) throw "Missing configuration.binaryDataMethod";
//    c.binaryDataDirectory = checkDirectory(c.binaryDataDirectory);
//    return c;
//};

tagio.open = function(filePath, configuration) {
    filePath = checkFile(filePath);
    //configuration = checkConfiguration(configuration);
    //console.log(filePath);
    //console.log(configuration);
    var ext = path.extname(filePath);
    if (ext === '.mp3') return new tagio.MPEG(filePath, configuration);
    else return new tagio.GENERIC(filePath, configuration);
};

module.exports = tagio;