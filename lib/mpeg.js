var inherits = require("util").inherits;
var tagio = require("../build/Release/tagio");
var GENERIC = require("./generic");

var MPEG = function(path, config) {
    this._file = new tagio.MPEG(path, config);
};

inherits(MPEG, GENERIC);

MPEG.prototype.hasAPETag = function() {
    return this._file.hasAPETag();
};

MPEG.prototype.hasID3v1Tag= function() {
    return this._file.hasID3v1Tag();
};

MPEG.prototype.hasID3v2Tag= function() {
    return this._file.hasID3v2Tag();
};

MPEG.prototype.getID3v2Tag= function() {
    return this._file.getID3v2Tag();
};

MPEG.prototype.setID3v2Tag= function(tag) {
    return this._file.setID3v2Tag(tag);
};

module.exports = MPEG;

