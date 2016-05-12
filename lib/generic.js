var tagio = require("../build/Release/tagio");

var GENERIC = function(path, config) {
    this._config = config;
    this._file = new tagio.GENERIC(path, config);
};

GENERIC.prototype.getPath = function() {
    return this._file.getPath();
};

GENERIC.prototype.getConfiguration = function() {
    return this._config;
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
        configuration: null,
        audioProperties: null,
        includedTags: [],
        tags: {}
    };
    all.path = this.getPath();
    all.configuration = this._config;
    all.audioProperties = this.getAudioProperties();
    if (this.getIncludedTags) all.includedTags = this.getIncludedTags();
    all.tags.generic = this.getTag();
    if (this.getAPETag)   all.tags.ape   = this.getAPETag();
    if (this.getID3v1Tag) all.tags.id3v1 = this.getID3v1Tag();
    if (this.getID3v2Tag) all.tags.id3v2 = this.getID3v2Tag();
    if (this.getXiphComment) all.tags.xiphComment = this.getXiphComment();
    return all;
};

GENERIC.prototype.setAll = function(all) {
    var file = this._file;
    if (all.tags.generic) file.setTag(all.tags.generic);
    if (all.tags.ape && file.setAPETag) file.setAPETag(all.tags.ape);
    if (all.tags.id3v1 && file.setID3v1Tag) file.setID3v1Tag(all.tags.id3v1);
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
    var retval = this._file.save();
    this._file = null;
    return retval;
};

module.exports = GENERIC;
