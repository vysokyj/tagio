#include "bytevector.h"
#include "configuration.h"
#include "md5.h"
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

using namespace TagIO;
using namespace std;


TagLib::ByteVector ByteVector::Import(TagLib::String pathString) {
    string path(StringToPath(pathString.to8Bit(true)));
    ifstream ifs;
    ifs.open(path, ios::in | ios::binary);
    ifs.seekg(0, ios::end);
    long length = ifs.tellg();
    char *data = new char[length];
    ifs.seekg(0, ios::beg);
    ifs.read(data, length);
    ifs.close();
    return TagLib::ByteVector(data, (uint) length);
}

TagLib::String ByteVector::Export(TagLib::ByteVector byteVector, TagLib::String mimeType) {
    Configuration &cfg = Configuration::Get();
    if (cfg.BinaryDataMethod() == BinaryDataMethod::IGNORE)
        return TagLib::String("IGNORED");
    string binaryDataDirectory(cfg.BinaryDataDirectory());
    string fileName = NewFileName(byteVector, mimeType.to8Bit(true));
    string filePath = NewPath(binaryDataDirectory, fileName);
    TagLib::String filePathString = PathToString(filePath, fileName);
    if (FileExist(filePath)) return filePathString;

    ofstream ofs;
    ofs.open(filePath, ios::out | ios::binary);
    ofs.write(byteVector.data(), byteVector.size());
    ofs.close();

    return filePathString;
}

string ByteVector::NewFileName(TagLib::ByteVector byteVector, string mimeType) {
    string hash = CountMD5(byteVector);
    string ext = (mimeType.empty()) ? "bin" : mimeType.substr(mimeType.find("/") + 1);
    //TODO: Implement complete mime type table...
    if (mimeType.compare("image/jpeg")) ext = "jpg";
    else if (mimeType.compare("image/pjpeg")) ext = "jpg";
    else if (mimeType.compare("image/png")) ext = "png";
    else if (mimeType.compare("text/plain")) ext = "txt";
    return hash + '.' + ext;
}

string ByteVector::CountMD5(TagLib::ByteVector byteVector) {
    string s = string(byteVector.data(), byteVector.size());
    string hash = md5(s);
    return hash;
}

string ByteVector::NormalizePath(string path) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(path.begin(), path.end(), '/', '\\');
    return path;
#else
    replace(path.begin(), path.end(), '\\', '/');
    return path;
#endif
}

string ByteVector::NewPath(string directoryPath, string fileName) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(directoryPath.begin(), directoryPath.end(), '/', '\\');
    return directoryPath + '\\' + fileName;
#else
    replace(directoryPath.begin(), directoryPath.end(), '\\', '/');
    return directoryPath + '/' + fileName;
#endif
}

std::string ByteVector::NewAbsoluteUrl(std::string filePath) {
    replace(filePath.begin(), filePath.end(), '\\', '/');
    return "file://" + filePath;
}

std::string ByteVector::NewRelativeUrl(std::string relativeUrl, std::string fileName) {
    return relativeUrl + '/' + fileName;
}

std::string ByteVector::PathToString(std::string filePath, std::string fileName) {
    Configuration &cfg = Configuration::Get();
    string retval = fileName;
    string binaryDataUrlPrefix(cfg.BinaryDataUrlPrefix());
    if (cfg.BinaryDataMethod() == BinaryDataMethod::PREFIXED_URL)
        retval = NewRelativeUrl(binaryDataUrlPrefix, fileName);
    else if (cfg.BinaryDataMethod() == BinaryDataMethod::ABSOLUTE_URL)
        retval = NewAbsoluteUrl(filePath);
    return retval;
}

std::string ByteVector::StringToPath(std::string str) {
    Configuration &cfg = Configuration::Get();
    string filePrefix = "file://";
    string binaryDataUrlPrefix(cfg.BinaryDataUrlPrefix());
    string binaryDataDirectory(cfg.BinaryDataDirectory());
    if (cfg.BinaryDataMethod() == BinaryDataMethod::PREFIXED_URL) {
        return NewPath(binaryDataDirectory, str.substr(binaryDataUrlPrefix.length() + 1));
    } else if (cfg.BinaryDataMethod() == BinaryDataMethod::ABSOLUTE_URL) {
        return NormalizePath(str.substr(filePrefix.length()));
    } else {
        return NewPath(binaryDataDirectory, str);
    }
}

inline bool ByteVector::FileExist(const std::string &name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}


