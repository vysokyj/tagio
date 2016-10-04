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

static std::string NewAbsoluteUrl(std::string filePath) {
    replace(filePath.begin(), filePath.end(), '\\', '/');
    return "file://" + filePath;
}

static std::string NewRelativeUrl(std::string relativeUrl, std::string fileName) {
    return relativeUrl + '/' + fileName;
}

static std::string PathToString(std::string filePath, std::string fileName, Configuration *conf) {
    string retval = fileName;
    string binaryDataUrlPrefix(conf->fileUrlPrefix().toCString(true));
    if (conf->fileExtracted() == FILE_EXTRACTED_AS_RELATIVE_URL)
        retval = NewRelativeUrl(binaryDataUrlPrefix, fileName);
    else if (conf->fileExtracted() == FILE_EXTRACTED_AS_ABSOLUTE_URL)
        retval = NewAbsoluteUrl(filePath);
    return retval;
}

static string StringToPath(string str, Configuration *conf) {
    string filePrefix = "file://";
    string binaryDataUrlPrefix(conf->fileUrlPrefix().toCString(true));
    string binaryDataDirectory(conf->fileDirectory().toCString(true));
    if (conf->fileExtracted() == FILE_EXTRACTED_AS_RELATIVE_URL) {
        return NewPath(binaryDataDirectory, str.substr(binaryDataUrlPrefix.length() + 1));
    } else if (conf->fileExtracted() == FILE_EXTRACTED_AS_ABSOLUTE_URL) {
        return NormalizePath(str.substr(filePrefix.length()));
    } else {
        return NewPath(binaryDataDirectory, str);
    }
}

static inline bool FileExist(const std::string &name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

TagLib::String ExportByteVector(TagLib::ByteVector byteVector, TagLib::String mimeType, Configuration *conf) {
    if (conf->fileExtracted() == FILE_EXTRACTED_IS_IGNORED)
        return TagLib::String("IGNORED");
    string binaryDataDirectory(conf->fileDirectory().toCString(true));
    string fileName = NewFileName(byteVector, mimeType.to8Bit(true));
    string filePath = NewPath(binaryDataDirectory, fileName);
    TagLib::String filePathString = PathToString(filePath, fileName, conf);
    if (FileExist(filePath)) return filePathString;

    ofstream ofs;
    ofs.open(filePath, ios::out | ios::binary);
    ofs.write(byteVector.data(), byteVector.size());
    ofs.close();

    return filePathString;
}

TagLib::ByteVector ImportByteVector(TagLib::String pathString, Configuration *conf) {
    string path(StringToPath(pathString.to8Bit(true), conf));
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
