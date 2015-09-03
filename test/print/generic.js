module.exporst = function(file) {
    console.log("Path: %s", file.getPath());
    console.log("Audio properties: %j", file.getAudioProperties());
    console.log("Generic tag: %j", file.getTag());
};