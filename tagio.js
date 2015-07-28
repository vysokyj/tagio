var tagio = require('./build/Release/tagio');
var path = require('path');

tagio.BinaryDataMethod = {
    FILENAME: "FILENAME",
    ABSOLUTE_URL: "ABSOLUTE_URL",
    RELATIVE_URL: "RELATIVE_URL"
};

tagio.Configuration = {
    binaryDataDirectory: "/tmp",
    binaryDataContext: "/attachments",
    binaryDataMethod: tagio.BinaryDataMethod.FILENAME,
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: false
};

tagio.open = function(file, configuration) {
    var ext = path.extname(file);
    if (ext === '.mp3') return new tagio.MPEG(file, configuration);
    else return new tagio.GENERIC(file, configuration);
};

module.exports = tagio;