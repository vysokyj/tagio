var tagio = require("../build/Release/tagio");
var fs = require("fs");
var path = require("path");

tagio.id3v2 = require("./id3v2");

tagio.BinaryDataMethod = {
    IGNORE: "IGNORE",
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

var checkConfiguration = function(c) {
    if (!c) throw "Configuration missing";
    if (!c.binaryDataDirectory) throw "Missing configuration.binaryDataDirectory";
    if (!c.binaryDataUrlPrefix) throw "Missing configuration.binaryDataUrlPrefix";
    if (!c.binaryDataMethod) throw "Missing configuration.binaryDataMethod";
    c.binaryDataDirectory = checkDirectory(c.binaryDataDirectory);
    return c;
};

var getNative = function(filePath, configuration) {
    var ext = path.extname(filePath);
    switch (ext) {
        case ".mp3":
            return new tagio.MPEG(filePath, configuration);
        default:
            return new tagio.GENERIC(filePath, configuration);
    }
};

var getAll = function() {
    var all = {
        path: null,
        audioProperties: null,
        tags: {}
    };
    all.path = this.getPath();
    all.audioProperties = this.getAudioProperties();
    all.tags.generic = this.getTag();
    if (this.getID3v2Tag) all.tags.id3v2 = this.getID3v2Tag();
    return all;
};

var setAll = function(all) {
    if (all.tags.generic) this.setTag(all.tags.generic);
    if (all.tags.id3v2 && this.setID3v2Tag) this.setID3v2Tag(all.tags.id3v2);
    return getAll();
};

var log = function() {
    console.log(JSON.stringify(this.getAll(), null, 2));
};

tagio.open = function(filePath, configuration) {
    filePath = checkFile(filePath);
    configuration = checkConfiguration(configuration);
    var native = getNative(filePath, configuration);
    native.getAll = getAll;
    native.log = log;
    return native;
};

module.exports = tagio;