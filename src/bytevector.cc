#include "bytevector.h"
#include "configuration.h"
#include "md5.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace TagIO;
using namespace std;

TagLib::String ByteVector::Export(TagLib::ByteVector byteVector, TagLib::String mimeType) {
    //TODO: Implement other methods
    return ExportFile(byteVector, mimeType);

}

TagLib::ByteVector ByteVector::Import(TagLib::String path) {
    //TODO: Implement other methods
    return ImportFile(path);
}


TagLib::String ByteVector::ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType) {
    string binaryDataDirectory(Configuration::Get().GetBinaryDataDirectory());
    string binaryDataUrlPrefix(Configuration::Get().GetBinaryDataUrlPrefix());
    string fileName = NewFileName(byteVector, mimeType.to8Bit(true));
    string filePath = NewPath(binaryDataDirectory, fileName);
    ofstream ofs;
    ofs.open(filePath, ios::out | ios::binary);
    ofs.write(byteVector.data(), byteVector.size());
    ofs.close();
    string retval = NewRelativeUrl(binaryDataUrlPrefix, fileName);
    cout << "binaryDataDirectory: " << binaryDataDirectory << endl;
    cout << "binaryDataUrlPrefix: " << binaryDataUrlPrefix << endl;
    cout << "retval: " << retval << endl;
    return TagLib::String(retval, TagLib::String::UTF8);
}

TagLib::ByteVector ByteVector::ImportFile(TagLib::String path) {
    string binaryDataUrlPrefix(Configuration::Get().GetBinaryDataUrlPrefix());
    string prefix(Configuration::Get().GetBinaryDataUrlPrefix());
    string fileName = path.to8Bit(true).substr(prefix.length());
    string filePath = NewPath(binaryDataUrlPrefix, fileName);
    ifstream ifs;
    ifs.open(filePath, ios::in | ios::binary);
    ifs.seekg(0, ios::end);
    long length = ifs.tellg();
    char *data = new char[length];
    ifs.seekg(0, ios::beg);
    ifs.read(data, length);
    ifs.close();
    return TagLib::ByteVector(data, (uint) length);
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

string ByteVector::NewPath(string directoryPath, string fileName) {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(directoryPath.begin(), directoryPath.end(), '/', '\\');
    return directoryPath + '\\' + fileName;
#else
    replace(directoryPath.begin(), directoryPath.end(), '\\', '/');
    return directoryPath + '/' + fileName;
#endif
}

string  ByteVector::NewRelativeUrl(string relativeUrl, string fileName) {
    return relativeUrl + '/' + fileName;
}
