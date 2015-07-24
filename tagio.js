var tagio = require('./build/Release/tagio');
var path = require('path');

tagio.Configuration = {
    attachmentsDir: ".",
    attachmentsCtx: "/",
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: false
};

tagio.open = function(file, configuration) {
    var ext = path.extname(file);
    if (ext === '.mp3') return new tagio.MPEG(file, configuration);
};

module.exports = tagio;