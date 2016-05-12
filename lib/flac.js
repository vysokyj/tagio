var inherits = require("util").inherits;
var tagio = require("../build/Release/tagio");
var GENERIC = require("./generic");

var FLAC = function(path, config) {
    this._config = config;
    this._file = new tagio.FLAC(path, config);
};

inherits(FLAC, GENERIC);

FLAC.prototype.getIncludedTags = function() {
    return this._file.getIncludedTags();
};

FLAC.prototype.getIncludedTags = function() {
    return this._file.getIncludedTags();
};

FLAC.prototype.getXiphComment = function() {
    return this._file.getXiphComment();
};

FLAC.prototype.setXiphComment = function(tag) {
    return this._file.setXiphComment(tag);
};

FLAC.prototype.getID3v1Tag = function() {
    return this._file.getID3v1Tag();
};

FLAC.prototype.setID3v1Tag = function(tag) {
    return this._file.setID3v1Tag(tag);
};

FLAC.prototype.getID3v2Tag = function() {
    return this._file.getID3v2Tag();
};

FLAC.prototype.setID3v2Tag = function(tag) {
    return this._file.setID3v2Tag(tag);
};

module.exports = FLAC;
