var fs = require("fs");
var path = require("path");
var tagio = require("../tagio");

var findDirs = function(dir, done) {
    var results = [];
    fs.readdir(dir, function(err, list) {
        if (err) return done(err);
        var pending = list.length;
        if (!pending) return done(null, results);
        list.forEach(function(file) {
            file = path.resolve(dir, file);
            fs.stat(file, function(err, stat) {
                if (stat && stat.isDirectory()) {
                    findDirs(file, function(err, res) {
                        results = results.concat(res);
                        if (!--pending) done(null, results);
                    });
                    results.push(file);
                } else {
                    if (!--pending) done(null, results);
                }
            });
        });
    });
};

var array = [];
var config = {
    binaryDataDirectory: "/tmp",
    binaryDataUrlPrefix: "/attachments",
    binaryDataMethod: tagio.BinaryDataMethod.ABSOLUTE_URL,
    saveID3v1Tag: false,
    saveID3v2Tag: true,
    saveApeTag: false
};

var processDir = function(dir) {
    console.log(dir);
    var list = fs.readdirSync(dir);
    list.forEach(function(file) {
        if (path.extname(file) ===  ".mp3") {
            var filePath = path.resolve(dir, file);
            //console.log(filePath);
            var mp3 = tagio.open(filePath, config);
            //mp3.log();
            array.push(mp3.getAll());
        }
    });
};

var processDirs = function(err, results) {
    if (err) throw err;
    results.forEach(processDir);
    var json = JSON.stringify(array, null, 2)
    console.log(json);
    fs.writeFileSync("/tmp/tagio.js", json);
};

var main = function(path) {
    findDirs(path, processDirs);
};

main("/home/jirka/Music/cz/folk");

