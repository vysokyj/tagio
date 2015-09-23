var tagio = require("../build/Release/tagio");

var GENERIC = function(path, config) {
    this._file = new tagio.GENERIC(path, config);
};

GENERIC.prototype.getPath = function() {
    return this._file.getPath();
};

GENERIC.prototype.getAudioProperties = function() {
    return this._file.getAudioProperties();
};

GENERIC.prototype.getTag = function() {
    return this._file.getTag();
};

GENERIC.prototype.setTag = function(tag) {
    return this._file.setTag(tag);
};

GENERIC.prototype.getAll = function() {
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

GENERIC.prototype.setAll = function(all) {
    var file = this._file;
    if (all.tags.generic) file.setTag(all.tags.generic);
    if (all.tags.id3v2 && file.setID3v2Tag) file.setID3v2Tag(all.tags.id3v2);
    return this.getAll();
};

/**
 * Log all available data.
 */
GENERIC.prototype.log = function() {
    console.log(JSON.stringify(this.getAll(), null, 2));
};

GENERIC.prototype.save = function() {
    return this._file.save();
};

module.exports = GENERIC;