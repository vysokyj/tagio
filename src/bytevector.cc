#include "bytevector.h"
#include "configuration.h"
#include "md5.h"
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

using namespace TagIO;
using namespace std;


TagLib::ByteVector ByteVector::Import(TagLib::String string) {
    ifstream ifs;
    ifs.open(StringToPath(string.to8Bit(true)), ios::in | ios::binary);
    ifs.seekg(0, ios::end);
    long length = ifs.tellg();
    char *data = new char[length];
    ifs.seekg(0, ios::beg);
    ifs.read(data, length);
    ifs.close();
    return TagLib::ByteVector(data, (uint) length);
}

TagLib::String ByteVector::Export(TagLib::ByteVector byteVector, TagLib::String mimeType) {
    if (Configuration::Get().GetBinaryDataMethod() == Configuration::BinaryDataMethod::IGNORE)
        return TagLib::String("IGNORED");
    string binaryDataDirectory(Configuration::Get().GetBinaryDataDirectory());
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

string ByteVector::NewPath(std::string directoryPath, std::string fileName) {
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
    string retval = fileName;
    string binaryDataUrlPrefix(Configuration::Get().GetBinaryDataUrlPrefix());
    if (Configuration::Get().GetBinaryDataMethod() == Configuration::BinaryDataMethod::PREFIXED_URL)
        retval = NewRelativeUrl(binaryDataUrlPrefix, fileName);
    else if (Configuration::Get().GetBinaryDataMethod() == Configuration::BinaryDataMethod::ABSOLUTE_URL)
        retval = NewAbsoluteUrl(filePath);

    return retval;
}

std::string ByteVector::StringToPath(std::string str) {
    string fileName = str;
    string filePrefix = "file://";
    string binaryDataUrlPrefix(Configuration::Get().GetBinaryDataUrlPrefix());
    string binaryDataDirectory(Configuration::Get().GetBinaryDataDirectory());

    if (Configuration::Get().GetBinaryDataMethod() == Configuration::BinaryDataMethod::PREFIXED_URL)
    fileName = str.substr(binaryDataUrlPrefix.length() + 1);
    else if (Configuration::Get().GetBinaryDataMethod() == Configuration::BinaryDataMethod::ABSOLUTE_URL)
    fileName = str.substr(filePrefix.length() + binaryDataDirectory.length() + 1);

    return NewPath(binaryDataDirectory, fileName);
}

inline bool ByteVector::FileExist(const std::string &name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
};

