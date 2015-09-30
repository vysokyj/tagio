var inherits = require("util").inherits;
var tagio = require("../build/Release/tagio");
var GENERIC = require("./generic");

var MPEG = function(path, config) {
    this._config = config;
    this._file = new tagio.MPEG(path, config);
};

inherits(MPEG, GENERIC);

MPEG.prototype.getIncludedTags = function() {
    return this._file.getIncludedTags();
};

MPEG.prototype.getAPETag = function() {
    return this._file.getAPETag();
};

MPEG.prototype.setAPETag = function(tag) {
    return this._file.setAPETag(tag);
};

MPEG.prototype.getID3v1Tag = function() {
    return this._file.getID3v1Tag();
};

MPEG.prototype.setID3v1Tag = function(tag) {
    return this._file.setID3v1Tag(tag);
};

MPEG.prototype.getID3v2Tag = function() {
    return this._file.getID3v2Tag();
};

MPEG.prototype.setID3v2Tag = function(tag) {
    return this._file.setID3v2Tag(tag);
};

module.exports = MPEG;

