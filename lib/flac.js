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


module.exports = FLAC;
