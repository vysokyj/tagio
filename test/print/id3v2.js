module.exports = function(file) {
    if (file.hasID3v2Tag()) {
        console.log("ID3v2 tag:");
        var frames = file.getID3v2Tag();
        for (var i = 0, l = frames.length; i < l; i++)
            console.log(" - frame %d: %j", i, frames[i]);
    } else {
        console.log("ID3v2 tag: undefined");
    }
};
