#include "bytevector.h"

#include "md5.h"
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

using namespace std;

static string CountMD5(TagLib::ByteVector byteVector) {
    string s = string(byteVector.data(), byteVector.size());
    string hash = md5(s);
    return hash;
}

static string NewFileName(TagLib::ByteVector byteVector, string mimeType) {
    string hash = CountMD5(byteVector);
    string ext = (mimeType.empty()) ? "bin" : mimeType.substr(mimeType.find("/") + 1);
    //TODO: Implement complete mime type table...
    if (mimeType.compare("image/jpeg")) ext = "jpg";
    else if (mimeType.compare("image/pjpeg")) ext = "jpg";
    else if (mimeType.compare("image/png")) ext = "png";
    else if (mimeType.compare("text/plain")) ext = "txt";
    return hash + '.' + ext;
}

static string NormalizePath(string path) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(path.begin(), path.end(), '/', '\\');
    return path;
#else
    replace(path.begin(), path.end(), '\\', '/');
    return path;
#endif
}

static string NewPath(string directoryPath, string fileName) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(directoryPath.begin(), directoryPath.end(), '/', '\\');
    return directoryPath + '\\' + fileName;
#else
    replace(directoryPath.begin(), directoryPath.end(), '\\', '/');
    return directoryPath + '/' + fileName;
#endif
}

//static inline bool FileExist(const std::string &name) {
//    struct stat buffer;
//    return (stat (name.c_str(), &buffer) == 0);
//}

TagLib::String ExportByteVector(TagLib::ByteVector byteVector, TagLib::String mimeType, Configuration *conf) {
    if (conf->FileExtracted() == FILE_EXTRACTED_IS_IGNORED) return TagLib::String("IGNORED");
    string directory = conf->FileDirectory().to8Bit(true);
    //std::cout << directory << std::endl;
    string fileName = NewFileName(byteVector, mimeType.to8Bit(true));
    string filePath = NewPath(directory, fileName);

    ofstream ofs;
    ofs.open(filePath, ios::out | ios::binary);
    ofs.write(byteVector.data(), byteVector.size());
    ofs.close();

    TagLib::String pathString(filePath);
    return pathString;
}

TagLib::ByteVector ImportByteVector(TagLib::String pathString, Configuration *conf) {
    return ImportByteVector(pathString.to8Bit(true), conf);
}


TagLib::ByteVector ImportByteVector(std::string path, Configuration *conf) {
    ifstream ifs;
    ifs.open(path, ios::in | ios::binary);
    ifs.seekg(0, ios::end);
    long length = ifs.tellg();
    char *data = new char[length];
    ifs.seekg(0, ios::beg);
    ifs.read(data, length);
    ifs.close();
    return TagLib::ByteVector(data, (TagLib::uint) length);
}