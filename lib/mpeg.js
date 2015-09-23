var inherits = require("util").inherits;
var tagio = require("../build/Release/tagio");
var GENERIC = require("./generic");

var MPEG = function(path, config) {
    this._file = new tagio.MPEG(path, config);
};

inherits(MPEG, GENERIC);

MPEG.prototype.getIncludedTags = function() {
    return this._file.getIncludedTags();
};

MPEG.prototype.getID3v2Tag = function() {
    return this._file.getID3v2Tag();
};

MPEG.prototype.setID3v2Tag = function(tag) {
    return this._file.setID3v2Tag(tag);
};

module.exports = MPEG;

